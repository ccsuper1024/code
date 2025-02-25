//Netcat is a simple read/write client for TCP servers
package main

import (
	"io"
	"log"
	"net"
	"os"
)

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

func main() {
	conn, err := net.Dial("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	//并发的从连接文件描述符中获取数据到输出
	go mustCopy(os.Stdout, conn)
	//同步的从标准输入中获取数据，并复制到连接文件描述符
	mustCopy(conn, os.Stdin)
}
