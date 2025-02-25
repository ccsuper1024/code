//修改fetch这个范例，如果输入的参数没有http://前缀，则为这个url加上前缀。你可能
//会用到strings.HasPrefix()这个函数
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
)

//Usage: ./1-8 www.baidu.com
func main() {
	//go语言中没有未初始化的变量，newUrl被初始化为空字符串
	var newUrl string
	for _, url := range os.Args[1:] {
		test := strings.HasPrefix(url, "http://")
		if !test {
			newUrl += "http://" + url
		}
		resp, err := http.Get(newUrl)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			os.Exit(1)
		}
		//简短变量声明中至少要有一个新变量
		//此处的err不是一个新变量，所以此处err被赋值，而不是被声明
		bytes, err := io.Copy(os.Stdout, resp.Body)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n", url, err)
			os.Exit(1)
		}
		fmt.Printf("read %d bytes", bytes)
	}
}
