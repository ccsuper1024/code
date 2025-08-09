//许多web服务提供JSON接口，通过HTTP接口发送JSON请求并返回JSON格式的信息。
//通过向github的issue查询服务演示类似的用法
package github

import "time"

const IssuesURL = "https://api.github.com/search/issues"

type IssuesSearchResult struct {
	TotalCount int `json:"total_count"`
	Items      []*Issue
}
type Issue struct {
	Number   int
	HTMLURL  string `json:"html_url"`
	Title    string
	State    string
	User     *User
	CreateAt time.Time `json:"created_at"`
	Body     string    //n Markdown fromat
}

type User struct {
	Login   string
	HTMLURL string `json:"html_url"`
}
