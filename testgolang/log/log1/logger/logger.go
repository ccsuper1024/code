package logger

import (
	"fmt"
	"io"
	"log"
	"os"
	"sync"
)

type Level int

const (
	INFO Level = iota
	WARN
	ERROR
)

type Logger struct {
	mtx      sync.Mutex
	outFile  *os.File
	toStdout bool
	logger   *log.Logger //使用标准库中的log库
}

func NewLogger(logPath string, toStdOut bool) *Logger {
	file, err := os.OpenFile(logPath, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0644)
	if err != nil {
		panic(fmt.Sprintf("无法打开日志文件: %v", err))
	}

	var writer io.Writer
	if toStdOut {
		writer = io.MultiWriter(file, os.Stdout)
	} else {
		writer = file
	}
	return &Logger{
		outFile:  file,
		toStdout: toStdOut,
		logger:   log.New(writer, "", log.LstdFlags|log.Lshortfile),
	}
}

/*方法*/
func (l *Logger) Close() {
	l.mtx.Lock()
	defer l.mtx.Unlock()
	if l.outFile != nil {
		l.outFile.Close()
	}
}

func (l *Logger) log(level string, format string, v ...interface{}) {
	l.mtx.Lock()
	defer l.mtx.Unlock()

	msg := fmt.Sprintf("[%s] %s", level, fmt.Sprintf(format, v...))
	l.logger.Output(3, msg) //调用栈向上跳3层定位真实调用处
}

/*首字母大小，是导出的，可以被外部调用*/
func (l *Logger) Info(format string, v ...interface{}) {
	l.log("INFO", format, v...)
}

func (l *Logger) Warn(format string, v ...interface{}) {
	l.log("WARN", format, v...)
}

func (l *Logger) Error(format string, v ...interface{}) {
	l.log("ERROR", format, v...)
}
