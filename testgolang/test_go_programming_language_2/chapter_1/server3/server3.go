/*
go语言中同一级目录下的文件共享同一个包作用域，只要文件中的函数的首字母大写就可以被同一目录下的其他文件调用
go语言中 一个目录==一个包
*/
package main

import (
	"fmt"
	"log"
	"net/http"
	"sync"
)

var mtx sync.Mutex
var count int

func handler(resp http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(resp, "%s %s %s\n", req.Method, req.URL, req.Proto)
	for k, v := range req.Header {
		fmt.Fprintf(resp, "Header[%q] = [%q]\n", k, v)
	}
	fmt.Fprintf(resp, "Host = %q\n", req.Host)
	fmt.Fprintf(resp, "RemoteAddr = %q\n", req.RemoteAddr)

	if err := req.ParseForm(); err != nil {
		log.Print(err)
	}

	for k, v := range req.Form {
		fmt.Fprintf(resp, "Form[%q] = %q\n", k, v)
	}
	mtx.Lock()
	count++
	mtx.Unlock()
}
func counter(resp http.ResponseWriter, req *http.Request) {
	mtx.Lock()
	fmt.Fprintf(resp, "Counter %d\n", count)
	mtx.Unlock()
}

func main() {
	http.HandleFunc("/", handler)
	http.HandleFunc("/counter", counter)
	log.Fatal(http.ListenAndServe("localhost:9000", nil))
}
