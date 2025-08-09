/*
编写一个函数计算两个SHA256哈希码中不同bit的数目。
*/
package main

import (
	"crypto/sha256"
	"fmt"
	"myGo2/chapter_4/practice4.1/popcount"
	"os"
)

func main() {
	if len(os.Args) != 3 {
		fmt.Printf("Usage: ./exe parameter1 parameter2")
		os.Exit(1)
	}

	c1 := sha256.Sum256(([]byte(os.Args[1])))
	c2 := sha256.Sum256(([]byte(os.Args[2])))

	fmt.Printf("c1 = %x\n", c1)
	fmt.Printf("c2 = %x\n", c2)

	diffBits := 0
	for i := 0; i < len(c1); i++ {
		diff := c1[i] ^ c2[i] //XOR每个字节,不同为1,c1是一个有32个元素的字节数组，len(c1)返回32
		diffBits += popcount.PopCount(diff)
		fmt.Printf("diffBit = %d\n", diffBits)
	}
	fmt.Printf("diffBits = %d\n", diffBits)
}
