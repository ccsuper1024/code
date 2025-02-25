//字符串一节的代码测试
package main

import "fmt"

func main() {
	s := "hello, world"
	fmt.Println(len(s))
	fmt.Println(s[0], s[7])

	//c := s[len(s)] //panic: index out of range
	//fmt.Println(c)

	s1 := "hello, 世界"
	//range循环式会自动处理unicode字符串，会自动解码UTF-8字符.
	// 注意对于非ASCII字符,索引更新的步长超过1个字节
	for i, r := range s1 {
		fmt.Printf("%d\t%q\t%d\n", i, r, r)
	}
	// 统计UTF-8字符串中字符数目
	n := 0
	for range s1 {
		n++
	}
	fmt.Printf("s1 has %d character\n", n)

	s3 := "プログラム"
	fmt.Printf("% x\n", s3) // "e3 83 97 e3 83 ad e3 82 b0 e3 83 a9 e3 83 a0"
	r := []rune(s3)
	fmt.Printf("%x\n", r) // "[30d7 30ed 30b0 30e9 30e0]"
	fmt.Println(string(1234567)) // "?"
}
