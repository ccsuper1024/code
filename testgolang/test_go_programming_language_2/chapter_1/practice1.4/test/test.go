// dup2 prints the count and text of lines that appear more than once in the input.
// It also shows which file(s) the line appeared in.

package main

import (
    "bufio"
    "fmt"
    "os"
)

func main() {
    counts := make(map[string]int)
    files := make(map[string]map[string]bool)

    if len(os.Args[1:]) == 0 {
        // 如果没有提供文件，使用标准输入
        countLines(os.Stdin, "stdin", counts, files)
    } else {
        for _, filename := range os.Args[1:] {
            f, err := os.Open(filename)
            if err != nil {
                fmt.Fprintf(os.Stderr, "dup2: %v\n", err)
                continue
            }
            countLines(f, filename, counts, files)
            f.Close()
        }
    }

    // 打印重复的行
    for line, n := range counts {
        if n > 1 {
            fmt.Printf("%d\t%s\tfiles: ", n, line)
            for file := range files[line] {
                fmt.Printf("%s ", file)
            }
            fmt.Println()
        }
    }
}

func countLines(f *os.File, filename string, counts map[string]int, files map[string]map[string]bool) {
    scanner := bufio.NewScanner(f)
    for scanner.Scan() {
        line := scanner.Text()
        counts[line]++
        if files[line] == nil {
            files[line] = make(map[string]bool)
        }
        files[line][filename] = true
    }
    // 忽略 scanner.Err() 错误
}
