package main

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	//Determine the initial directories
	flag.Parse()
	roots := flag.Args()
	if len(roots) == 0 {
		roots = []string{"."}
	}

	//Traverse the file tree
	//在goroutine中每个文件大小都会从通道中输出到主goroutine
	fileSizes := make(chan int64)
	go func() {
		for _, root := range roots {
			walkDir(root, fileSizes)
		}
		close(fileSizes)
	}()

	//在主goroutine中计算文件数和文件大小总和
	var nfiles int64 = 0
	var nbytes int64 = 0
	for size := range fileSizes {
		nfiles++
		nbytes += size
	}
	printDiskUsage(nfiles, nbytes)
}

func walkDir(dir string, filesize chan<- int64) {
	for _, entry := range dirents(dir) {
		if entry.IsDir() {
			subdir := filepath.Join(dir, entry.Name())
			walkDir(subdir, filesize) //递归调用
		} else {
			fileinfo, _ := entry.Info()
			filesize <- fileinfo.Size()
		}
	}
}
func dirents(dir string) []os.DirEntry {
	entries, err := os.ReadDir(dir)
	if err != nil {
		fmt.Fprintf(os.Stderr, "du1:%v\n", err)
	}
	return entries

}
func printDiskUsage(nfiles, nbytes int64) {
	fmt.Printf("%d files %.1f GB\n", nfiles, float64(nbytes)/1e9)
}
