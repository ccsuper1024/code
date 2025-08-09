package main

import (
	"fmt"
	"log"
	"myGo2/chapter_1/practice1.12/lissajous"
	"net/http"
	"sync"
)

var mtx sync.Mutex
var count uint32

func handler(resp http.ResponseWriter, req *http.Request) {
	lissajous.Lissajous(resp, req)

	mtx.Lock()
	count++
	mtx.Unlock()
}

func counter(resp http.ResponseWriter, req *http.Request) {
	mtx.Lock()
	fmt.Fprintf(resp, "count = %d\n", count)
	mtx.Unlock()
}

func main() {
	http.HandleFunc("/", handler)
	http.HandleFunc("/count", counter)
	log.Fatal(http.ListenAndServe("localhost:9000", nil))
}
