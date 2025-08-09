//调用golang.org/x/net/html的部分API。html.Parse函数读入一组bytes解析后，返回
//html.Node类型的HTML页面数装结构根结点。HTML拥有很多类型的结点如text,comments类型
//在下面的例子中，我们 只关注< name key='value' >形式的结点
//package html
//
//import "io"
//
//func Parse(r io.Reader) (*Node, error)
//
//type Attribute struct {
//	Key, Val string //导出变量
//}
//
//const (
//	ErrorNode NodeType = iota //常量初始化器
//	TextNode
//	DocumentNode
//	ElementNode
//	CommentNode
//	DoctypeNode
//)
//
//type NodeType int32
//
//type Node struct {
//	Type                    NodeType
//	Data                    string
//	Attr                    []Attribute //切片
//	FirstChild, NextSibling *Node
//}

//Findlinks1 print the links an HTML document read from standard input
package main

import (
	"fmt"
	"os"

	"golang.org/x/net/html"
)

//visit appends to links each link found in n and returns the result
func visit(links []string, n *html.Node) []string {
	if n.Type == html.ElementNode && n.Data == "a" {
		for _, a := range n.Attr {
			if a.Key == "href" {
				links = append(links, a.Val)
			}
		}
	}

	for c := n.FirstChild; c != nil; c = c.NextSibling {
		links = visit(links, c)
	}
	return links
}
func main() {
	doc, err := html.Parse(os.Stdin)
	if err != nil {
		fmt.Fprintf(os.Stderr, "findlinks1:%v\n", err)
		os.Exit(1)
	}

	for _, link := range visit(nil, doc) {
		fmt.Println(link)
	}
}
