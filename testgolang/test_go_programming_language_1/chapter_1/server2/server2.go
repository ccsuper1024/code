//Server2 is a minimal "echo" and counter server
package main

//如果只有一个包只能使用import "包名"这种形式。如果用以下形式会出错？
import (
	"fmt"
	"log"
	"net/http"
	"sync"
)

//包级别变量
//共享变量
var mu sync.Mutex
var count int

//handler echoes the Path component of the requested URL
func handler(w http.ResponseWriter, r *http.Request) {
	mu.Lock()
	count++
	mu.Unlock()
	fmt.Fprintf(w, "URL.Path = %q\n", r.URL.Path)
}

//counter echoes the number of calls so far.
func counter(w http.ResponseWriter, r *http.Request) {
	mu.Lock()
	fmt.Fprintf(w, "Count %d\n", count)
	mu.Unlock()
}

/*
服务器每一次接受请求处理时都会另起一个goroutine，这样服务器就可以同一时间处理
多个请求
但同时会出现新的问题。在并发情况下，假如真的有两个请求同一时刻去更新count，那么
这个值会出现混乱。这就是竞态条件。为了避免这个问题，使用mutex互斥锁来保护临界区代码
*/
func main() {
	http.HandleFunc("/", handler)
	http.HandleFunc("/count", counter)
	log.Fatal(http.ListenAndServe("localhost:8000", nil))
}
