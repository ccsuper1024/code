package main

import "testing"

func TesetDecode(t *testing.T) { //testing.T 结构中包含1个读写锁，所以不要用值传递
	post, err := decode("post.json")
	if err != nil {
		t.Error(err)
	}
	if post.Id != 1 {
		t.Error("Wrong id, was expecting 1 but got", post.Id)
		return
	}

	if post.Content != "hello world!" {
		t.Error("Wrong content, was expecting'hello world!' but got ", post.Content)
		return
	}

}
func TestEncode(t *testing.T) {
	t.Skip("Skipping encoding for now")

}
