package main

import "log1/log1/logger"

func main() {
	log := logger.NewLogger("../log/app.log", true)
	defer log.Close()

	log.Info("服务启动")
	log.Info("连接缓慢")
	log.Info("数据库连接失败")
}
