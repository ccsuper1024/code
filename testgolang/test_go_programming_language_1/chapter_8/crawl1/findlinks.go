/*
crawl1 crawl web links starting with the command-line argument
this version quickly exhausts available file descriptors
due to excessive concurrent calls to links.Extract.

Also, it never terminates because the worklist is never closed.
*/

package main

import (
	"fmt"
	"log"
	"os"

	"gopl.io/ch5/links"
)

func main() {
	worklist := make(chan []string) //make函数构造一个底层结构为是string切片的channel

	//start with the command-line argument
	go func() {
		worklist <- os.Args[1:]
	}() //go开启一个新的goroutine，并用管道发送命令行参数的数据，最后的圆括号表示调用

	//Crawl the web concurrently.
	seen := make(map[string]bool)
	for list := range worklist {
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				go func(link string) /*防止goroutine中的循环变量问题*/ {
					worklist <- crawl(link) //worklist 管道发送crawl()函数返回的字符串切片
				}(link)
			}
		}
	}
}

func crawl(url string) []string {
	fmt.Println(url)
	/*
		func links.Extract(url string) ([]string, error)
		Extract makes an HTTP GET request to the specified URL,
		parses the response as HTML, and returns the links in the HTML document.
	*/
	list, err := links.Extract(url)
	if err != nil {
		log.Print(err)
	}
	return list
}
