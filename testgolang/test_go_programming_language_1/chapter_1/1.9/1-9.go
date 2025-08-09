//修改fetch打印出HTTP协议的状态码，可以从resp.Status变量得到该状态码
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
)

const prefix = "http://"

func main() {
	var newUrl string
	for _, url := range os.Args[1:] {
		//检查url是否有指定前缀
		test := strings.HasPrefix(url, prefix)
		if !test {
			newUrl += prefix + url
		}
		//发起http请求
		resp, err := http.Get(newUrl)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			//调用系统的退出函数，杀死进程
			os.Exit(1)
		}
		//输出状态码
		fmt.Fprintf(os.Stdout, "the response status is %s\n\n", resp.Status)
		//输出响应报文
		//空白标识符不需要声明。而err变量早已声明。所以只需要使用赋值语句即可
		_, err = io.Copy(os.Stdout, resp.Body)
		if err != nil {
			fmt.Fprintf(os.Stderr, "io.Copy is wrong: %v ", err)
		}
		//关闭流
		resp.Body.Close()
	}
}
