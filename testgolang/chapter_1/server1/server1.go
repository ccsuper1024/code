//Server1 is a minimal "echo" server.
//web 服务
package main

import (
	"fmt"
	"log"
	"net/http"
)

//handler echos the Path component of the requst calls handler
func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "URL.Path=%q\n", r.URL.Path)
}
func main() {
	http.HandleFunc("/", handler) //each request calls handler
	log.Fatal(http.ListenAndServe("localhost:8000", nil))
}