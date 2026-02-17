package main
/*
不修改fetch的行为，重写fetch函数，要求使用defer机制关闭文件
*/

import (
	"io"
	"net/http"
	"os"
	"path"
)

func fetch(url string) (filename string, n int64, err error) {
	resp, err := http.Get(url)
	if err != nil {
		return "", 0, err
	}
	defer resp.Body.Close()
	local := path.Base(resp.Request.URL.Path)
	if local == "/" {
		local = "index.html"
	}
	f, err := os.Create(local)
	if err != nil {
		return "", 0, err
	}
	defer f.Close()

	n, err = io.Copy(f, resp.Body)
	if err != nil {
		return f.Name(), n, err
	}
	return local, n, err

}

func main() {
	fetch("www.baidu.com")
}
