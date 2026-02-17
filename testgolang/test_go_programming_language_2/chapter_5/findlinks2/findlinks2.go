package main

import (
	"fmt"
	"net/http"
	"os"

	"golang.org/x/net/html"
)

func visit(links []string, n *html.Node) []string {
	//当html节点类型为html.ElementNode，标签为"a"时，取a标签的href属性的值
	if n.Type == html.ElementNode && n.Data == "a" {
		for _, a := range n.Attr {
			if a.Key == "href" {
				links = append(links, a.Val)
			}
		}
	}

	for c := n.FirstChild; c != nil; c = n.NextSibling {
		links = visit(links, c)
	}
	return links
}

func findLinks(url string) ([]string, error) {
	resp, err := http.Get(url)
	if err != nil {
		return nil, err
	}
	fmt.Println(resp)
	if resp.StatusCode != http.StatusOK {
		resp.Body.Close()
		return nil, fmt.Errorf("getting %s as HTML:%v", url, resp.Status)
	}

	doc, err := html.Parse(resp.Body)
	resp.Body.Close()
	if err != nil {
		return nil, fmt.Errorf("parsing %s as HTML：%v", url, err)
	}
	return visit(nil, doc), err
}

func main() {
	for _, url := range os.Args[1:] {
		links, err := findLinks(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "findlinks2:%v\n", err)
			continue
		}
		for _, link := range links {
			fmt.Println(link)
		}

	}
}
