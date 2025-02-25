//Countdown implements the countdown for a rocket launch
package main

import (
	"fmt"
	"os"
	"time"
)

//Note: the ticker goroutine never terminates if the launch is aborted
//This is a "goroutine leak"

func main() {
	abort := make(chan struct{})
	go func() {
		os.Stdin.Read(make([]byte, 1)) //read a single byte
		abort <- struct{}{}            //向abort通道发送一个用字面量初始化语法初始化的struct{}变量
	}()

	fmt.Println("Commencing countdown. Press return to abort")
	tick := time.Tick(1 * time.Second) //创建一个定时发送当前时间消息的通道并返回其引用，这个定时通道会每隔指定时间自动向通道中发送时间消息
	for countdown := 10; countdown > 0; countdown-- {
		fmt.Println(countdown)
		select { //空select{}会无限等待
		case <-tick: //接收tick通道的消息
			//do nothing
		case <-abort: //接收abort通道的消息
			fmt.Println("Launch aborted!")
			return
		}
	}
	launch()
}

func launch() {
	fmt.Println("Lift off!")
}
