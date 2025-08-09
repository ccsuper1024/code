package github

import "time"

const IssuesURL = "https://api.github.com/search/issues"

type IssuesSearchResult struct {
	TotalCount int      `json:"total_count"`
	Items      []*Issue //一个指针数组，数组的元素是指向Issue结构的指针
}

type Issue struct {
	Number    int
	HTMLURL   string `json:"html_url"`
	Title     string
	State     string
	User      *User
	CreatedAt time.Time `json:"Created_at"`
	Body      string
}
type User struct {
	Logoin  string
	HTMLURL string `json:"html_url"`
}
