/*
与多个服务器通信，从多个服务器获取服务器时间并以表格的形式输入到终端
设计：从命令行读取参数，根据读取到的参数，通过TCP连接到不同的服务器程序
，不同的连接要求在不同的goroutine中，每个连接接收到服务器发来的时间字符串
并将其同步到主goroutine中创建的map中，主goroutine在创建连接到不同服务器
的连接后，定时(1s)打印map数据，以表格的形式。主goroutine通过通道和子goroutine
通信
*/
package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
	"strings"
	"text/tabwriter"
	"time"
)

type ClockServer struct {
	Name    string
	Address string
	Time    string
	Error   string
}

func main() {
	/*从命令行解析参数*/
	if len(os.Args) < 2 {
		fmt.Println("使用方法：clockwall 名称1=地址1 名称2=地址2 ...")
		fmt.Println("示例：clockwall NewYork=localhost:8010")
		os.Exit(1)
	}

	servers := parseArgs(os.Args[1:])
	if len(servers) == 0 {
		fmt.Println("没有有效的服务器配置")
		os.Exit(1)
	}

	//创建通道接受服务器时间
	timeCh := make(chan ClockServer, len(servers))

	/*与多个服务器程序建立连接*/
	for _, serverAddress := range servers {
		go connectToServer(serverAddress, timeCh)
	}

	/*以表格的形式打印这些服务器的输出*/
	displayClockWall(servers, timeCh)

}

/*显示时钟墙*/
func displayClockWall(servers []ClockServer, timeCh <-chan ClockServer) {
	/*创建服务器状态的映射*/
	serverStatus := make(map[string]ClockServer)
	for _, server := range servers {
		serverStatus[server.Name] = server
	}

	//清屏并隐藏光标
	fmt.Print("\033[2J\033[?25l")
	//程序退出时显示光标
	defer fmt.Print("\033[?25h")

	ticker := time.NewTicker(100 * time.Millisecond)
	defer ticker.Stop()

	for {
		select {
		/*从通道中取出数据*/
		case updateServer := <-timeCh:
			//更新服务器状态
			serverStatus[updateServer.Name] = updateServer
		/*定时器超时*/
		case <-ticker.C:
			//清屏并移动到左上角
			fmt.Print("\033[H]")

			//显示标题
			currentTime := time.Now().Format("2006-01-02 15:04:05 MST")
			fmt.Printf("=== 时钟墙 === (本地时间: %s)\n\n", currentTime)

			//创建表格写入器
			writer := tabwriter.NewWriter(os.Stdout, 0, 0, 2, ' ', 0)

			//表头
			fmt.Fprintln(writer, "城市名称\t服务器地址\t时间\t状态")
			fmt.Fprintln(writer, "--------\t----------\t----\t------")

			//显示每个服务器的时间
			for _, server := range servers {
				status := serverStatus[server.Name]
				timeDisplay := status.Time
				statusDisplay := "✓ 正常"

				if status.Error != "" {
					statusDisplay = fmt.Sprintf("✗ %s", status.Error)
					if timeDisplay == "" {
						timeDisplay = "N/A"
					}
				}

				fmt.Fprintf(writer, "%s\t%s\t%s\t%s\t\n", status.Name, status.Address,
					timeDisplay, statusDisplay)
			}
			writer.Flush()

			//显示帮助信息
			fmt.Printf("\n按 Ctrl+C 退出\n")
		}
	}

}

func parseArgs(param []string) []ClockServer {
	var servers []ClockServer = nil
	var count int = 0

	for _, value := range param {
		fmt.Printf("param[%d] = %s\n", count, value)
		count++
	}

	for _, arg := range param {
		parts := strings.Split(arg, "=")
		if len(parts) != 2 {
			fmt.Fprintf(os.Stderr, "无效的参数格式：%s,应使用名称=地址格式", arg)
			continue
		}

		name := strings.TrimSpace(parts[0])
		address := strings.TrimSpace(parts[1])

		if name == "" || address == "" {
			fmt.Println("名称和地址都不可以为空")
			continue
		}
		servers = append(servers, ClockServer{
			Name:    name,
			Address: address,
		})
	}
	return servers

}

/*连接到一个时间服务器并持续读取时间，知道连接关闭*/
func connectToServer(server ClockServer, ch chan<- ClockServer) {
	var count int
	for {
		conn, err := net.Dial("tcp", server.Address)
		if err != nil {
			fmt.Fprintf(os.Stderr, "net.Dial Failed!! 连接失败[%d]次\n", count)
			server.Error = fmt.Sprintf("连接失败:%v", err)
			server.Time = ""
			/*向主goroutine发送数据*/
			ch <- server
			time.Sleep(5 * time.Second)
			continue
		}
		defer conn.Close()

		reader := bufio.NewReader(conn)
		for {
			timeStr, err := reader.ReadString('\n')
			if err != nil {
				if err == io.EOF {
					server.Error = "连接被服务器关闭"
				} else {
					server.Error = fmt.Sprintf("读取错误:%v", err)
				}
				break
			}
			/**/
			server.Time = strings.TrimSpace(timeStr)
			server.Error = ""
			/*向主goroutine发送数据*/
			ch <- server
		}

		/*连接断开后等待重连*/
		time.Sleep(2 * time.Second)
	}

}
