/*生成斐波那契数列，同时在生成的过程中用一个goroutine定时打印简单动画*/
package main

import (
	"fmt"
	"time"
)

// main goroutine结束后 go spinner也就结束了
func main() {
	//每秒打印一次
	go spinner(1000 * time.Millisecond)

	const n = 45
	fibN := fib(n)
	fmt.Printf("\r Fibonacci(%d) = %d\n", n, fibN)
}

func spinner(delay time.Duration) {
	for {
		for _, r := range `-\|/` {
			fmt.Printf("\r%c", r)
			time.Sleep(delay)
		}
	}
}

func fib(n int) int {
	if n == 1 || n == 0 {
		return n
	}

	return fib(n-1) + fib(n-2)
}
