#!/bin/bash
echo "启动时钟服务器集群"
./clockserver -city NewYork -timezone America/New_York -port 8010 &
./clockserver -city Tokyo -timezone Asia/Tokyo -port 8020 &
./clockserver -city London -timezone Europe/London -port 8030 &

echo "所有服务器都已启动，等待客户端连接"

