/*尝试从alexa.com中选择大量的网站并放入文件中，实现从文件中而不是标准输入中读取url 并对这些url发起请求*/
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
)

var fileName string = "./url.txt"

func fetch(url string, ch chan string) (*http.Response, error) {
	fmt.Printf("begin to Get Url")
	resp, err := http.Get(url)
	return resp, err
}

func main() {
	var file *os.File
	var err error
	file, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		fmt.Printf("os.OpenFIle Failed!! %s", "./url.txt")
		os.Exit(1)
	}
	/*会在函数结束是自动调用函数*/
	defer file.Close()

	data, err := io.ReadAll(file)
	if err != nil {
		fmt.Printf("io.ReadAll read %s Failed!!", file.Name())
		os.Exit(1)
	}

	/*也可以用一下函数实现*/
	//bufio.NewScanner()

	/*分隔字符串, 返回一个字符串切片*/
	lines := strings.Split(string(data), "\n")

	for _, url := range lines {
		url := strings.TrimSpace(url) //删除前导符和空格

		ch := make(chan string)
		go fetch(url, ch)
	}
}
