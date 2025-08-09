package main

import "fmt"

// basename removes directory components and a .suffic.
func basename(s string) string {
	//Discard last '/' and everything before
	for i := len(s) - 1; i >= 0; i-- {
		if s[i] == '/' {
			s = s[i+1:]
			fmt.Printf("s = %s\n", s)
			break
		}
	}

	//Preserver everything before last '.'
	for i := len(s) - 1; i >= 0; i-- {
		if s[i] == '.' {
			s = s[:i]
			fmt.Printf("s = %s\n", s)
			break
		}
	}
	return s
}

func main() {
	fmt.Println(basename("a/b/c.go"))
	fmt.Println(basename("c.d.go"))
	fmt.Println(basename("abc"))
}
