/*palindrome:回文*/
package word

import (
	"math/rand"
	"unicode"
)

/*IsPalindrome reports whether s reads the same forward and backward.*/
/*Letter case is ignored, as are non-letters.*/
func IsPalindrome(s string) bool {
	var letters []rune
	for _, r := range s {
		//如果r是字母时
		if unicode.IsLetter(r) {
			//将转成小写字母的字符续接到字符后面
			letters = append(letters, unicode.ToLower(r))
		}
	}

	for i := range letters {
		if letters[i] != letters[len(letters)-1-i] {
			return false
		}
	}
	return true
}

/*
randomPalindrome returns a palindrome whose length and contents are
derived from the pseudo-random number generator rng.
*/
func randomPalindrome(rng *rand.Rand) string {
	n := rng.Intn(25) //random length up to 24
	runes := make([]rune, n)
	for i := 0; i < (n+1)/2; i++ {
		r := rune(rng.Intn(0x1000)) //random rune up to '\u0999'
		runes[i] = r
		runes[n-1-i] = r
	}
	return string(runes)
}
