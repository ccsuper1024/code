/*
实现一个并发FTP服务器。
服务器应解析客户端来的一些命令，比如cd命令，ls命令
get/send命令传输文件，close关闭链。
可以使用标准的ftp命令作为客户端或者自己实现一个

ftp服务器在和ftp客户端交互过程中实际山会使用多个端口，不仅仅是20和21
端口21：控制连接的监听端口(所有客户端都首先连接到这里)
端口20：主动模式下的数据连接源端口，主动模式下也会创建随机端口用于连接
临时端口，被动模式下使用的随机端口
临时端口
*/
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"path"
	"strconv"
	"strings"
)

type FTPSession struct {
	controlConn  net.Conn     //控制连接
	dataConn     net.Conn     //数据连接
	dataListener net.Listener //连接的监听对象
	currentDir   string       //FTP服务器当前目录
	username     string       //用户名
	loggedIn     bool
	transferType string //"A" for ASCII, "I" for binary
	mode         string //主动 or 被动
}

/*向FTP客户端发送响应*/
func (s *FTPSession) sendResponse(code int, message string) {
	response := fmt.Sprintf("%d %s\r\n", code, message)
	s.controlConn.Write([]byte(response))
	log.Printf("Sent: %d %s", code, message)
}

/*处理USER命令 参数为用户名*/
func (s *FTPSession) handleUSER(command string) {
	parts := strings.Fields(command)
	if len(parts) < 2 {
		s.sendResponse(501, "Syntax error in parameters")
		return
	}
	s.username = parts[1]
	s.sendResponse(331, "User name okay, need password")
}

/*输入密码--必须在User命令之后，必须先获得用户名*/
func (s *FTPSession) handlePASS(command string) {
	if s.username == "" {
		s.sendResponse(503, "Login with User First")
		return
	}

	/*获取对端ip与端口号*/
	addr := s.controlConn.RemoteAddr().String()
	addrString := strings.Split(addr, ":")
	ip := addrString[0]
	port, err := strconv.Atoi(addrString[1])
	if err != nil {
		fmt.Println(err)
	}

	commandArr := strings.Fields(command)
	if len(commandArr) != 2 {
		fmt.Printf("客户端ip[%s]port[%d]发来的ftp请求不合法\n", ip, port)
		s.sendResponse(501, "Syntax error in parameters")
	}
	//解析command字符串中携带的信息
	passwd := commandArr[1]
	fmt.Println(passwd)

	//简单认证：允许anonymous或任何用户
	if s.username == "anonymous" {
		s.loggedIn = true
		s.sendResponse(230, "User logged in ,proceed")
	} else if s.username != "anonymous" && passwd == "admin" { //此处的密码应该从配置文件中读取,并支持修改
		s.loggedIn = true
		s.sendResponse(230, "User logged in, proceed")
	} else {
		s.loggedIn = true
		s.sendResponse(230, "User logged in, proceed")
	}

}

/*系统类型 可以输出uname -a 命令中的数据*/
func (s *FTPSession) handleSYST(command string) {
	_ = command
	s.sendResponse(215, "UNIX Type: L8")

}

/*输出当前目录*/
func (s *FTPSession) handlePWD(command string) {
	_ = command
	s.sendResponse(257, fmt.Sprintf("\"%s\" is current directory", s.currentDir))

}

/*设置数据连接的传输类型*/
func (s *FTPSession) handleTYPE(command string) {
	parts := strings.Fields(command)
	if len(parts) < 2 {
		s.sendResponse(501, "Syntax error in parameters")
		return
	}

	switch parts[1] {
	case "A":
		s.transferType = "A"
		s.sendResponse(200, "Type set to ASCII")
	case "I":
		s.transferType = "I"
		s.sendResponse(200, "Type set to binary")
	default:
		s.sendResponse(500, "Unrecognized TYPE command")
	}

}

/*建立数据连接*/
func (s *FTPSession) setupDataConnection() error {
	if s.dataListener == nil {
		listener, err := net.Listen("tcp", ":0") //随机端口
		if err != nil {
			return err
		}
		s.dataListener = listener
	}

	//获取监听端口
	addr := s.dataListener.Addr().(*net.TCPAddr) //类型断言,接口类型，转换为指向net.TCPAddr的指针
	port := addr.Port

	//格式：h1,h2,h3,h4
	response := fmt.Sprintf("127,0,0,1,%d,%d", port>>8, port&0xFF)
	s.sendResponse(227, "Entering Passive Mode("+response+")")

	//接受数据连接
	conn, err := s.dataListener.Accept()
	if err != nil {
		return err
	}
	s.dataConn = conn
	return nil

}

/*被动模式接受数据，服务器主动打开端口等待*/
func (s *FTPSession) handlePASV(command string) {
	_ = command
	//解析command参数

	//set up data connection
	if err := s.setupDataConnection(); err != nil {
		s.sendResponse(425, "Can't open data connection")
	}

}

/*列出当前目录的所有文件*/
func (s *FTPSession) handleLIST(command string) {
	_ = command
	if !s.loggedIn {
		s.sendResponse(530, "Please log in first")
		return
	}
	s.sendResponse(150, "Opening ASCII mode data connection for file list")

	if s.dataConn == nil {
		if err := s.setupDataConnection(); err != nil {
			s.sendResponse(425, "Can't Open data connection")
			return
		}
	}

	//简单的目录列表
	files, _ := os.ReadDir(s.currentDir)
	var list strings.Builder
	for _, file := range files {
		info, _ := file.Info()
		list.WriteString(fmt.Sprintf("%s\t%d\t%s\n", info.Mode().String(),
			info.Size(), info.Name()))
	}
	s.dataConn.Write([]byte(list.String()))
	s.dataConn.Close()
	s.dataConn = nil
	s.sendResponse(226, "Transfer complete")

}

/*下载文件*/
func (s *FTPSession) handleRETR(command string) {
	if !s.loggedIn {
		s.sendResponse(530, "Please log in first")
		return
	}

	parts := strings.Fields(command)
	if len(parts) < 2 {
		s.sendResponse(501, "Syntax error in parameters")
		return
	}

	filename := parts[1]
	s.sendResponse(150, fmt.Sprintf("Opening data connection for %s", filename))

	if s.dataConn == nil {
		if err := s.setupDataConnection(); err != nil {
			s.sendResponse(425, "Can't open data connection")
			return
		}
	}

	file, err := os.Open(filename)
	if err != nil {
		s.sendResponse(550, "file not found")
		return
	}
	defer file.Close()

	io.Copy(s.dataConn, file)
	s.dataConn.Close()
	s.dataConn = nil
	s.sendResponse(226, "Transfer complete")
}

/*被动模式和主动模式的区别就是，
主动模式下，由客户端打开一个端口并告诉服务器，服务器
主动连接到客户端指定的端口上
被动模式则是服务器主动打开一个端口等待客户端的连接*/
/*上传文件*/
func (s *FTPSession) handleSTOR(command string) {
	if !s.loggedIn {
		s.sendResponse(530, "Not logged in")
		return
	}

	//检查是否有数据连接
	if s.dataConn == nil {
		//如果是主动模式
		if s.mode == "PORT" {

		}
	}
}

/*退出*/
func (s *FTPSession) handleQUIT(command string) {
	_ = command
	s.sendResponse(221, "GoodBye")
	s.controlConn.Close()
}

// 改变当前目录
func (s *FTPSession) handleCWD(command string) {
	if !s.loggedIn {
		s.sendResponse(530, "Please log in first")
		return
	}

	//解析command
	parts := strings.Fields(command)
	if len(parts) < 2 {
		s.sendResponse(501, "Syntax error in parameters")
		return
	}
	//切换话中的当前目录，注意区分绝对路径和相对路径。做好防呆
	/*比如路径中是否有// */

	filePath := parts[1]
	//绝对路径
	if strings.HasPrefix(filePath, "/") {
		s.currentDir = filePath
	} else { //相对路径
		tmpFilePath := s.currentDir
		/* 可能是当前目录，可能是根目录，需要对这些边界情况处理*/
		path := strings.TrimSuffix(tmpFilePath, "/") + filePath
		_, err := os.Stat(path)
		if os.IsNotExist(err) {
			s.sendResponse(501, "Syntax error in parameters")
		}
		s.currentDir = path
	}

}

// 转到上级目录
func (s *FTPSession) handleCDUP(command string) {
	if !s.loggedIn {
		s.sendResponse(530, "Please log in first")
		return
	}

	if s.currentDir == "" {
		s.currentDir = "."
	}

	if s.currentDir == "/" {
		s.currentDir = "/"
	}

	filePath := strings.TrimSuffix(s.currentDir, "/")
	parentDir := path.Dir(filePath)
	s.currentDir = parentDir
}
func (s *FTPSession) processCommand(command string) {
	fmt.Printf("Received:%s", command)

	upperCommand := strings.ToUpper(command)
	switch {
	case strings.HasPrefix(upperCommand, "USER"):
		s.handleUSER(command)
	case strings.HasPrefix(upperCommand, "PASS"):
		s.handlePASS(command)
	case strings.HasPrefix(upperCommand, "SYST"):
		s.handleSYST(command)
	case strings.HasPrefix(upperCommand, "PWD"):
		s.handlePWD(command)
	case strings.HasPrefix(upperCommand, "TYPE"):
		s.handleTYPE(command)
	case strings.HasPrefix(upperCommand, "PASV"):
		s.handlePASV(command)
	case strings.HasPrefix(upperCommand, "LIST"):
		s.handleLIST(command)
	case strings.HasPrefix(upperCommand, "RETR"):
		s.handleRETR(command)
	case strings.HasPrefix(upperCommand, "QUIT"):
		s.handleQUIT(command)
	case strings.HasPrefix(upperCommand, "CWD"):
		s.handleCWD(command)
	case strings.HasPrefix(upperCommand, "CDUP"):
		s.handleCDUP(command)
	case strings.HasPrefix(upperCommand, "STOR"):
		s.handleSTOR(command)
	}
}
func handleFTPConnection(conn net.Conn) {
	defer conn.Close()

	/*构造会话，持久保持用户信息*/
	session := &FTPSession{
		controlConn:  conn,
		currentDir:   ".", //要存储绝对路径
		transferType: "A",
	}

	session.sendResponse(220, "FTP Server ready")

	buffer := make([]byte, 1024) //创建元素类型为[]byte，数量为1024的缓存
	for {
		/*go语言中的读写都默认是阻塞的*/
		/*如果需要非阻塞，有多种实现方式*/
		/*
			1、 调用conn.SetDeadline(),设置超时时间，那么读写就不会永久阻塞。但这不是非阻塞读写，只是实现了读写的超时机制而已
			2、 使用unsafe包中和系统直接的接口，底层编程
			3、 由于go优秀的并发机制，协程。所以可以用goroutine和channel配合实现，这也是go语言开发者最常用的方法

		*/
		n, err := conn.Read(buffer)
		if err != nil {
			fmt.Printf("conn.Read() Failed!! Client is %s\n",
				conn.RemoteAddr().String())
			break
		}
		command := strings.TrimSpace(string(buffer[:n])) //移除数据中头和尾部的空格
		session.processCommand(command)
	}
}
func main() {
	/*获取命令行参数*/
	if len(os.Args) != 3 {
		fmt.Printf("")
		return
	}

	var portStr = os.Args[2]
	port, err := strconv.Atoi(portStr)
	if err != nil {
		fmt.Printf("strconv.Atoi Failed!!")
		return
	}
	if port < 0 && port > 65535 {
		fmt.Printf("您输入的端口是无效的")
		return
	}
	if port < 1024 && port > 0 {
		fmt.Printf("请使用一个非常用的端口")
		return
	}
	address := os.Args[1] + ":" + portStr
	/*检查命令行参数是否有效*/
	/*开启监听*/
	listener, err := net.Listen("tcp", address)
	if err != nil {
		fmt.Printf("net.Listen Failed!!")
		return
	}
	defer listener.Close() //自动关闭
	/*等待连接*/
	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Print("listener.Accept() is Failed!!")
			continue //此处不要返回，一次失败程序仍需运作
		}

		//每个用户一个连接，每个连接一个goroutine
		go handleFTPConnection(conn)
	}
}
