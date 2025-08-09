//Reverb1 is a TCP server that simulates an echo
//非并发的
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
)

func main() {
	//监听127.0.0.1 端口好8000的socket内核文件
	l, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}
	count := 0
	for {
		//阻塞等待获取新的连接文件描述符
		conn, err := l.Accept()
		if err != nil {
			log.Print(err) //e.g, connect aborted 客户端连接终止
			continue       // 跳到当前循环的最后
		}
		count++
		fmt.Printf("count: %d\n", count)
		//并发调用连接处理函数
		go handleConn(conn)
	}
}

func handleConn(c net.Conn) {
	/*
		func bufio.NewScanner(r io.Reader) *bufio.Scanner
		NewScanner returns a new Scanner to read from r. The split function defaults to ScanLines.
	*/
	input := bufio.NewScanner(c)
	for input.Scan() /*方法*/ {
		echo(c, input.Text(), 1*time.Second)
	}
	//Note: ignoring potential errors from input.Err()
	c.Close()
}

func echo(c net.Conn, shout string, delay time.Duration) {
	fmt.Fprintln(c, "\t", strings.ToUpper(shout))
	time.Sleep(delay) //休眠delay秒
	fmt.Fprintln(c, "\t", shout)
	time.Sleep(delay)
	fmt.Fprintln(c, "\t", strings.ToLower(shout))
}
