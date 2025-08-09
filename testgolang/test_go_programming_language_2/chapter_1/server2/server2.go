/*
Server2 is a minimal "echo" and counter server
*/
package main

import (
	"fmt"
	"log"
	"net/http"
	"sync"
)

var mu sync.Mutex
var count int

// handle echoes the Path component of the requested URL.
func handler(resp http.ResponseWriter, req *http.Request) {
	mu.Lock()
	count++
	mu.Unlock()
	fmt.Fprintf(resp, "URL.Path = %q\n", req.URL.Path)
}

/*counter echoes the number of calls so far*/
func counter(resp http.ResponseWriter, req *http.Request) {
	mu.Lock()
	fmt.Fprintf(resp, "Count %d\n", count)
	mu.Unlock()
}

func main() {
	http.HandleFunc("/", handler)
	http.HandleFunc("/counter", counter)
	log.Fatal(http.ListenAndServe("localhost:9000", nil))
}
