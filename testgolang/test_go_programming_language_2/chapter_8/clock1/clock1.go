/*创建一个服务器程序，监听端口，向客户端返回当前服务器时间*/
/*一个进程一个连接，只有在上一个连接结束后才能处理下一个连接*/
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"time"
)

func main() {
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		fmt.Printf("net.Listen Failed!!")
		fmt.Print(err)
		log.Fatal(err)
	}

	/*main goroutine阻塞等待连接*/
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal(err)
			continue
		}
		/*handle a connection*/
		handleConn(conn)
	}
}

func handleConn(c net.Conn) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			fmt.Print(err)
			return
		}
		time.Sleep(time.Second * 5)
	}
}
