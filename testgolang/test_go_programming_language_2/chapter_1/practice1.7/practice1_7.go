package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

/*go语言中main函数没有参数与返回值*/
func main() {
	for _, url := range os.Args[1:] {
		resp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			os.Exit(1)
		}

		file, err := os.OpenFile("file.txt", os.O_CREATE|os.O_APPEND|os.O_RDWR, 0644)
		if err != nil {
			fmt.Printf("os.OpenFile FAILED!! %v", err)
			os.Exit(1)
		}
		b, err := io.Copy(file, resp.Body)
		if err != nil {
			fmt.Printf("io.Copy FAILED!!: %v", err)
			os.Exit(1)
		}
		resp.Body.Close()
		fmt.Printf("复制成功字节数:%d\n", b)

	}
}
