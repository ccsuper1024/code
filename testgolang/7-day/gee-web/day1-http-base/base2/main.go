//实现http标准库中的http.Handle接口
/*
package http

type Handle interface {
	ServeHTTP(w ResponseWriter, r *Request)
}

func ListenAndServe(address string, h Handle) error
只要实现了ServeHTTP方法就算是实现了Handle接口类型，即一种泛型
*/
package main

import (
	"fmt"
	"log"
	"net/http"
)

//Engine is the uni handler for all requests
type Engine struct{}

//方法
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	switch req.URL.Path {
	case "/":
		fmt.Fprintf(w, "URL.Path = %q\n", req.URL.Path)
	case "/hello":
		for k, v := range req.Header {
			fmt.Fprintf(w, "Header[%q]= %q\n", k, v)
		}
	default:
		fmt.Fprintf(w, "404 NOT FOUND: %s\n", req.URL)
	}
}
func main() {
	engine := new(Engine) //创建一个Engine结构体类型的指针
	log.Fatal(http.ListenAndServe(":9999", engine))
}
