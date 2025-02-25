//fetchall的特别之处在于它会同时去获取所有的URL，所以这个程序的总执行时间不会超过
//执行时间最长的那一个任务
//fetchall程序只会打印获取的内容大小和经过的时间，不会像之前那样打印获取的内容。
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
	"time"
)

const prefix = "https://"

func fetch(url string, ch chan<- string) {
	start := time.Now()
	var newUrl string
	//检查url是否有指定前缀
	test := strings.HasPrefix(url, prefix)
	if !test {
		newUrl += prefix + url
	}
	//创建http请求
	resp, err := http.Get(newUrl)
	if err != nil {
		ch <- fmt.Sprint(err) //send to channel ch
		return
	}

	//获取resp.Body流中的数据
	data, err := io.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("io.ReadAll wrong:%v \n", err)
		os.Exit(1)
	}
	//打开文件
	file, err := os.OpenFile("file", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		fmt.Printf("os.OpenFile wrong:%v \n", err)
		os.Exit(1)
	}
	defer file.Close() //延迟调用关闭文件流
	//写入文件
	nbytes, err := file.Write(data)
	if err != nil {
		ch <- fmt.Sprintf("while reading %s : %v", url, err)
		return
	}
	secs := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%.2fs  %7d  %s", secs, nbytes, url)

	//关闭流
	resp.Body.Close() //don't leak resource
}

func main() {
	start := time.Now()
	ch := make(chan string)
	for _, url := range os.Args[1:] {
		go fetch(url, ch) //start a goroutine
	}
	for range os.Args[1:] {
		fmt.Println(<-ch) //receive from channel ch
	}
	fmt.Printf("%.2fs elapsed\n", time.Since(start).Seconds())

}
