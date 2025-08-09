//handler函数把请求http头和请求的form数据都打印出来，使得检查和调试更为方便
//golang使用的并发模型和POSIX中的不同。用的是叫做CSP的顺序通信进程
package main

import (
	"fmt"
	"log"
	"net/http"
	"sync"
)

var mu sync.Mutex
var count int

func main() {
	http.HandleFunc("/", handler)
	//counter的类型是个函数。在golang中函数是一等公民，可以作为变量传递，
	//也可以作为函数的返回值.函数不仅可以被调用和定义
	http.HandleFunc("/count", counter)
	log.Fatal(http.ListenAndServe("localhost:8000", nil))
}

////handler echoes the Path component of the requested URL
//func handler(w http.ResponseWriter, r *http.Request) {
//	mu.Lock()
//	count++
//	mu.Unlock()
//	fmt.Fprintf(w, "URL.Path = %q\n", r.URL.Path)
//}

func counter(w http.ResponseWriter, r *http.Request) {
	mu.Lock()
	fmt.Fprintf(w, "Count %d\n", count)
	mu.Unlock()
}

//handler echoes the Http request
//*http.Request是对http.Request的解引用吗?
//不是，这表明r是个指向http.Request的指针
func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "%s %s %s\n", r.Method, r.URL, r.Proto)
	//类似python中的多重赋值。这里使用了短变量声明,短变量声明类似局部变量
	//golang中对于格式非常严格，不允许未使用的变量
	for k, v := range r.Header {
		//range从r.Header这个map类型(存疑)中取值，返回的第二个值为bool类型
		//能从r.Header中取值，则为true,当无法从r.Header中取值时就会变为false
		fmt.Fprintf(w, "Header[%q] = %q\n", k, v)
	}
	fmt.Fprintf(w, "Host = %q\n", r.Host)
	fmt.Fprintf(w, "RemoteAddr = %q\n", r.RemoteAddr)
	//以下形式的if语句限制了err变量的作用域。类似块变量
	if err := r.ParseForm(); err != nil {
		log.Print(err)
	}
	//golang中只有for循环着一种循环语句，格式如下
	//for initialization; condition; post {}
	for k, v := range r.Form {
		//向 io.Writer流类型写入数据
		fmt.Fprintf(w, "Form[%q] = %q\n", k, v)
	}
}
