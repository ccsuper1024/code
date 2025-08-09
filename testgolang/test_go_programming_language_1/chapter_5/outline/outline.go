//outline prints the outline of an HTML document tree.
package main

import (
	"fmt"
	"os"

	"golang.org/x/net/html"
)

func outline(stack []string, n *html.Node) { //无返回值
	if n.Type == html.ElementNode {
		stack = append(stack, n.Data) //push tag
		fmt.Println(stack)
	}
	for c := n.FirstChild; c != nil; c = c.NextSibling {
		outline(stack, c) //递归调用
	}
}

func main() {
	doc, err := html.Parse(os.Stdin) //获得一个html页面
	if err != nil {
		fmt.Fprintf(os.Stderr, "outline: %v\n", err)
		os.Exit(1)
	}
	outline(nil, doc)
}
