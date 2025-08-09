/*
minimal "echo " server
*/
package main

import (
	"fmt"
	"log"
	"net/http"
)

func handler(resp http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(resp, "URL.Path = %q\n", req.URL.Path)
}

func main() {
	http.HandleFunc("/", handler) //each request calls handler
	log.Fatal(http.ListenAndServe("localhost:9000", nil))
}
