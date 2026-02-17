/*
1、修改clock2支持传入参数作为端口号--服务端
2、写一次clockwall的程序，用于连接不同的clock服务器，并从多个服务器中读取时间
并在一个表格中一次显示所有服务传回的结果，类似机场的时钟墙
*/
package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"time"
)

func main() {
	//定义命令行参数
	city := flag.String("city", "local", "城市名称")
	timezone := flag.String("timezone", "local", "时区(例如:Asia/Shanghai,America/New_York)")
	port := flag.Int("port", 8000, "监听端口号")
	flag.Parse()

	//验证参数
	if *city == "" {
		log.Fatal("必须指定城市名称(-city)")
	}
	if *port <= 1024 || *port > 65535 {
		log.Fatal("端口号必须在1024-65535之间")
	}

	//启动时间服务器
	startTimeServer(*city, *timezone, *port)
}

func startTimeServer(city, timezone string, port int) {
	//设置时区
	loc := time.Local
	if timezone != "Local" {
		var err error
		/*获取时区*/
		loc, err = time.LoadLocation(timezone)
		if err != nil {
			log.Printf("警告：无法加载时区 '%s',使用本地市区：%v", timezone, err)
			loc = time.Local
		}
	}

	//创建监听器
	address := fmt.Sprintf(":%d", port)
	listener, err := net.Listen("tcp", address)
	if err != nil {
		log.Fatalf("%s 服务器启动失败：%v", city, err)
	}
	defer listener.Close()

	log.Printf("%s 时间服务器启动 | 时区：%s | 监听：%s", city, loc, address)

	//处理客户端连接
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Printf("%s 接受连接失败：%v", city, err)
			continue
		}
		go handlerTimeClient(conn, city, loc)
	}
}

// 处理时间客户端连接
func handlerTimeClient(conn net.Conn, city string, loc *time.Location) {
	defer conn.Close()

	log.Printf("%s:客户端连接[%s]", city, conn.RemoteAddr())
	ticker := time.NewTicker(1 * time.Second)
	defer ticker.Stop()
	for t := range ticker.C {
		timeStr := t.In(loc).Format("2006-01-02 15:04:05 MST")
		output := fmt.Sprintf("%-10s | %s \n", city, timeStr)

		_, err := conn.Write([]byte(output)) //将String强制转换为字节切片
		if err != nil {
			log.Printf("%s:客户端断开 [%s]", city, conn.RemoteAddr())
			return
		}
	}
}
