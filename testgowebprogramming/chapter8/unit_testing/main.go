package main

import (
	"encoding/json"
	"fmt"
	"os"
)

type Comment struct {
	Id      int    `json:"id"`
	Content string `json:"content"`
	Author  string `json:"authot"`
}

type Author struct {
	Id      int    `json:"id"`
	Content string `json:"content"`
	Author  string `json:"author"`
}
type Post struct {
	Id      int    `json:"id"`
	Content string `json:"content"`
	Author  Author `json:"author"`
}

// decode json from file to struct
func decode(filename string) (post Post, err error) { //最后一个括号是返回值和返回值类型
	jsonFile, err := os.Open(filename) //短变量声明
	if err != nil {
		fmt.Println("Error opening JSON file: ", err)
		return
	}
	defer jsonFile.Close()

	//创建解码器
	decoder := json.NewDecoder(jsonFile) //短变量声明用于局部变量，var 变量名 类型名用于创建局部变量和全局变量
	err = decoder.Decode(&post)
	if err != nil {
		fmt.Println("Error decoding JSON:", err)
		return
	}

	return

}

func encode()  {
	
}
