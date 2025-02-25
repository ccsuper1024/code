package github

import (
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"strings"
)

//SearchIssues queries the Github issue tracker
func SearchIssues(terms []string) (*IssuesSearchResult, error) {
	//func url.QueryEscape(s string) string
	//QueryEscape escapes the string so it can be safely placed inside a URL query.
	q := url.QueryEscape(strings.Join(terms, " "))
	//func http.Get(url string) (resp *http.Response, err error)
	//所以http.Get()是个函数而不是个方法。
	resp, err := http.Get(IssuesURL + "?q=" + q)
	if err != nil {
		return nil, err
	}
	//!-
	/*
		For long tern stability, instead of http.Get, use the
		variant below which adds an HTTP request header indicating
		that only version 3 of the GitHub API is acceptable.

		req, err = http.NewRequest("GET",IssuesURL + "?q=" + q)
		if err != nil {
			return nil, err
		}
		req.Header.Set(
			"Accept", "application/vnd.github.v3.text-match+json"
		)
		resp, err := http.DeafaultClient.Do(req)
	*/
	//!+

	/*
		We must close resp.Body on all execution paths.
		(chapter 5 presents 'defer', which makes this simpler)
	*/
	if resp.StatusCode != http.StatusOK {
		//var resp *http.Response	resp是指向http.Response的指针
		//http.Response是个结构体，结构体用来描述协议，可以方便的编解码
		resp.Body.Close() //关闭resp响应
		return nil, fmt.Errorf("search query failed: %s", resp.Status)
	}
	//声明一个结构体变量，并被编译器初始化为0值
	var result IssuesSearchResult
	//func json.NewDecoder(r io.Reader) *json.Decoder
	//func (*json.Decoder).Decode(v any) error		//一个方法而非函数
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		resp.Body.Close()
		return nil, err
	}
	resp.Body.Close()
	return &result, nil
}
