/*对某个大型网站重复两次http请求 观察响应时间以及响应内容是否相同*/
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"time"
)

func fetch(ch chan<- string, url string) {
	start := time.Now()
	resp, err := http.Get(url)
	if err != nil {
		ch <- fmt.Sprint(err) //send to channel ch
		return
	}

	nbytes, err := io.Copy(io.Discard, resp.Body)
	resp.Body.Close()
	if err != nil {
		ch <- fmt.Sprintf("while reading %s: %v", url, err) //send to channel ch
		return
	}
	secs := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%.2fs %7d %s", secs, nbytes, url)

}

var g_times int = 2

func main() {
	start := time.Now()
	ch := make(chan string) //创建双向的管道变量，输入输出数据为string
	for _, url := range os.Args[1:] {
		for i := 0; i < g_times; i++ { //start a goroutine two times
			go fetch(ch, url)
		}
	}

	total := g_times * len(os.Args[1:])
	for range total {
		fmt.Println(<-ch) //receive from channel ch
	}
	fmt.Printf("%.2fs elapsed \n", time.Since(start).Seconds())
}
