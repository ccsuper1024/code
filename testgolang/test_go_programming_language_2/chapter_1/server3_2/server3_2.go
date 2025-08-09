package main

import (
	"fmt"
	"log"
	"net/http"
	"sync"

	"myGo2/chapter_1/server3_2/lissajous"
)

var mtx sync.Mutex
var count uint32

func handler(resp http.ResponseWriter, req *http.Request) {
	lissajous.Lissajous(resp)

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
	http.HandleFunc("/counter", counter)

	log.Fatal(http.ListenAndServe("localhost:9000", nil))

}
