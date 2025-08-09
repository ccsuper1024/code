/*
Fetch prints the content found at a URL
*/
package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"runtime"
)

func main() {
	for _, url := range os.Args[1:] {
		resp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
			os.Exit(1)
		}

		b, err := io.ReadAll(resp.Body)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n", url, err)
			return
		}
		resp.Body.Close()

		pc, _, _, _ := runtime.Caller(0)
		fmt.Printf("%s:%s", runtime.FuncForPC(pc).Name(), b)
	}
}
