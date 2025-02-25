//clock1 is a Tcp server that periodically writes the time
package main

import (
	"io"
	"log"
	"net"
	"time"
)

func handleConn(c net.Conn) {
	defer c.Close() //延迟调用，关闭连接文件描述符。延迟函数会在return语句后调用
	for {
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return //e.g, client disconnected
		}
		time.Sleep(1 * time.Second) //协程休眠一秒
	}
}

func main() {
	//监听端口
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}
	for { //死循环
		conn, err := listener.Accept() //阻塞等待服务端socket内核文件返回的新连接
		if err != nil {
			log.Print(err) //e.g, connection aborted
			continue
		}
		//处理新连接时的函数
		handleConn(conn) //handle one connection at a time
	}
}
