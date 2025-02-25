#!/usr/bin/env python
# coding=utf-8
# 直接使用套接字与谷歌地图通信

import socket
from urllib.parse import quote_plus

request_text = """"
    GET /maps/api/geocode/json?address={}&sensor=false HTTP/1.1\r\n
    Host: maps.google.com:80\r\n
    User-Agent: search4.py\r\n
    Connection: close\r\n
"""

def geocode(address):
    sock = socket.socket()                                  #默认创建TCPsocket
    sock.connect(('maps.google.com', 80))                   #发起连接 TCP 3次握手
    request = request_text.format(quote_plus(address))      #http请求
    sock.sendall(request.encode("ascii"))                   #向socket内核文件发送数据

    raw_reply = b''                                         #创建对象
    while True:
        more = sock.recv(4096)
        if not more:
            break
        raw_reply += more
    print(raw_reply.decode('utf-8'))

if __name__ == "__main__":
    geocode("207 N. Defiance St, Archbold, OH")
