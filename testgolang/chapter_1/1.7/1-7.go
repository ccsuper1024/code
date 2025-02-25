//函数调用io.Copy(dst, src)会从src中读取内容，并将读取的内容写入到dst中，使用这个
//函数提到例子中的ioutil.ReadAll来拷贝响应结构体到os.Stdout,避免申请一个缓冲区
//来存储，记得处理io.Copy返回结构中的错误
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

func main() {
	for _, url := range os.Args[1:] {
		resp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			os.Exit(1)
		}
		bytes, err := io.Copy(os.Stdout, resp.Body)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n", url, err)
			os.Exit(1)
		}
		fmt.Printf("read %d bytes", bytes)
	}
}
