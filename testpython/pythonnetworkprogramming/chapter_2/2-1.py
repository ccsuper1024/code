#!/usr/bin/env python
# coding=utf-8

#使用自环接口的UDP服务器和客户端
import argparse, socket
from datetime import datetime

MAX_BYTES = 65535

def server(port):
    #创建socket内核文件
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    #给socket内核文件绑定IP和端口号
    sock.bind(('127.0.0.1', port))
    print('Listening at {}'.format(sock.getsockname()))
    while True:
        data, address = sock.recvfrom(MAX_BYTES)
        text = data.decode('ascii')
        print('The client at {} says {!r}'.format(address, text))
        text = 'Your data was {} bytes long'.format(len(data))
        data = text.encode('ascii')
        sock.sendto(data, address)

def client(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    text = 'The time is {}'.format(datetime.now())
    data = text.encode('ascii')
    sock.sendto(data, ('127.0.0.1', port))
    print('The OS assigned me the address {}'.format(sock.getsockname()))
    data, address = sock.recvfrom(MAX_BYTES)            #Danger
    text = data.decode('ascii')
    print('The Server {} repied {!r}'.format(address, text))

if __name__ == "__main__":
    choices = {'client':client, 'server':server}
    parser = argparse.ArgumentParser(description='Send and receive UDP locally')
    parser.add_argument('role', choices=choices, help='which role to play')
    parser.add_argument('-p', metavar='PORT', type=int, default=1060, help='UDP port (default 1060)')
    args = parser.parse_args()
    function = choices[args.role]
    function(args.p)