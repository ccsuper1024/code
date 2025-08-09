package github

import (
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"strings"
)

// SearchIssues queries the GitHub issue tracker
func SearchIssues(terms []string) (*IssuesSearchResult, error) {
	q := url.QueryEscape(strings.Join(terms, " "))
	resp, err := http.Get(IssuesURL + "?q=" + q)
	if err != nil {
		return nil, err
	}

	/*
		For long-term stability, instead of http.Get, use the
		variant bylow which adds an HTTP request header indicating
		that only version 3 of the GitHub API is accetable

		req , err := http.NewRequest("GET", IssuesURL+"?q="+q, nil)
		if err != nil {
			return nil, err
		}
		req.Header.Set()
			"Aceept", "application/vnd.github.v3.text-match+json")
		resp,err := htt.DefaultClient.Do(req)
	*/

	//We must close resp.Body on all execution paths
	//(Chapter 5 presents 'defer', which makes this simpler)
	if resp.StatusCode != http.StatusOK {
		resp.Body.Close()
		return nil, fmt.Errorf("search query failed:%s", resp.Status)
	}

	var result IssuesSearchResult
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		resp.Body.Close()
		return nil, err
	}
	resp.Body.Close()
	return &result, nil

}
