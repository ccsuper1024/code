//Fetch prints the content found at a URL
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

//Usage: ./fetch https://www.baidu.com
func main() {
	//基于range的for循环。返回两个值，第一个值是索引，第二个值是索引的值
	for _, url := range os.Args[1:] {
		//发起http请求，获取http响应
		//func http.Get(url string) (resp *http.Response, err error)
		//resp是个指向http.Response的指针
		resp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			os.Exit(1)
		}
		//resp的Body字段包括一个可读的服务器响应流。
		//func io.ReadAll(r io.Reader) ([]byte, error)
		b, err := io.ReadAll(resp.Body)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n", url, err)
			os.Exit(1)
		}
		fmt.Printf("%s", b)
		//func (io.Closer).Close() error
		//resp.Body是对resp指向的结构体中成员的指针
		//但因为go语言中指针访问结构体中字段和结构体变量访问其内部变量都是一样的
		//只要使用.成员运算符即可。Go会自动解引用指针来获得结构体的成员
		resp.Body.Close() //关闭响应
	}
}
