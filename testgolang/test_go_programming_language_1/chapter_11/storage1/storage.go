package storage

import (
	"fmt"
	"log"
	"net/smtp"
)

func bytesInUse(userName string) int64 {
	return 0
}

// Email sender configuration
// Note: Never put passwords in source code!
const sender = "notifications@example.com"
const password = "correcthorsebatterystaple"
const hostname = "smtp.example.com"

const template = `Warning: you are using %d bytes of storage, %d%% of you quota`

func checkQuota(userName string) {
	used := bytesInUse(userName)
	const quota = 100000000
	percent := 100 * used / quota
	if percent < 90 {
		return //ok
	}

	msg := fmt.Sprintf(template, used, percent)
	auth := smtp.PlainAuth("", sender, password, hostname)
	err := smtp.SendMail(hostname+":587", auth, sender, []string{userName}, []byte(msg))
	if err != nil {
		log.Printf("smtp.SendMail(%s) failed: %s", userName, err)
	}
}
