package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"time"
)

/*go中的chan类型表示管道，有方向之分*/
func fetch(url string, ch chan<- string) {
	start := time.Now()
	resp, err := http.Get(url)
	if err != nil {
		/*当前goroutine有可能在ch <-处阻塞，等待有其他goroutine接收数据*/
		ch <- fmt.Sprint(err) //将fmt.Sprint返回的字符串输入到管道中
		return
	}

	nbytes, err := io.Copy(io.Discard, resp.Body)
	if err != nil {
		ch <- fmt.Sprintf("while reading %s, %v", url, err)
		return
	}
	resp.Body.Close()

	secs := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%2.7f %7d %s", secs, nbytes, url)

}
func main() {
	start := time.Now()
	ch := make(chan string)
	for _, url := range os.Args[1:] {
		/*创建goroutine*/
		go fetch(url, ch)
	}

	for range os.Args[1:] {
		/*将管道ch中输出的数据打印*/
		/*main函数的goroutine在<-ch处阻塞，等待ch代表的管道有其他goroutine发送数据*/
		fmt.Println(<-ch)
	}

	fmt.Printf("%.2f s elapsed\n", time.Since(start).Seconds())
}
