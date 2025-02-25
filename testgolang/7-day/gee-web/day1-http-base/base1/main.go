package main

import (
	"fmt"
	"log"
	"net/http"
)

func main() {
	//func http.HandleFunc(pattern string, handler func(http.ResponseWriter, *http.Request))
	http.HandleFunc("/", indexHandler)
	http.HandleFunc("/hello", helloHandler)
	//func http.ListenAndServe(addr string, handler http.Handler) error
	//start the server
	//第一个参数是监听的端口号，服务端的地址自己是知道的
	//第二个参数是代表处理所有HTTP请求的实例，nil表示使用标准库中的实例处理
	//同时第二个参数也是我们基于net/http标准库实现Web框架的入口

	log.Fatal(http.ListenAndServe(":9999", nil))
}

//handler echoes r.URL.Path
func indexHandler(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(w, "URL.Path = %q\n", req.URL.Path)
}

//handler echoes r.URL.Header
func helloHandler(w http.ResponseWriter, req *http.Request) {
	for k, v := range req.Header {
		fmt.Fprintf(w, "Header[%q] = %q\n", k, v)
	}
}
