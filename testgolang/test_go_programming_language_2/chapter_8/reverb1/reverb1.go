/*
并发的Echo服务
*/
package main

import (
	"log"
	"net"
)

func echo(conn net.Conn) {

}
func main() {
	/*
		在linux中创建socket内核文件，绑定ip，端口到内核文件中并开始监听，默认监听
		的连接队列大小？？
	*/
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}

	/*死循环*/
	for {
		/*当内核监听到上面创建的socket有新的连接后，会重新创建一个socket内核文件
		并将对端的ip和端口绑定到这个内核文件中。并创建输入输出缓冲区
		*/
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal(err)
			continue //跳到下一次循环
		}
		go echo(conn)

	}

}
