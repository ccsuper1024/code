package main

import (
	"crypto/sha256"
	"crypto/sha512"
	"flag"
	"fmt"
	"io"
	"os"
)

func main() {
	//定义命令行flag
	useSha384 := flag.Bool("sha384", false, "Use SHA384 hashing")
	useSha512 := flag.Bool("sha512", false, "Use SHA512 hashing")
	flag.Parse()

	//读取命令行输入
	data, err := io.ReadAll(os.Stdin)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error reading input: %v\n", err)
		os.Exit(1)
	}

	//根据flag 判断使用的哈希算法并打印 go中switch不需要break语句，如果需要case穿透用fallthrough
	switch {
	case *useSha384:
		sum := sha512.Sum384(data)
		fmt.Printf("SHA384:%v\n", sum)
	case *useSha512:
		sum := sha512.Sum512(data)
		fmt.Printf("SHA512:%v\n", sum)
	default:
		sum := sha256.Sum256(data)
		fmt.Printf("SHA256:%v\n", sum)
	}
}
