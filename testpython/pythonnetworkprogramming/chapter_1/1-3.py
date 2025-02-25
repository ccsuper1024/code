#!/usr/bin/env python
# coding=utf-8
#使用原始的HTTP操作连接谷歌地图

import http.client
import json
from urllib.parse import quote_plus

base = '/maps/api/geocode/json'

def geocode(address):
    path = '{}?address={}&sensor=false'.format(base, quote_plus(address))
    
    #创建HTTP客户端连接对象
    connection = http.client.HTTPConnection('maps.google.com')
    #连接发起请求消息
    connection.request('GET', path)
    rawreply = connection.getresponse().read()
    #json反序列化一个字符串/字节数组为python对象
    reply = json.loads(rawreply.decode("utf-8"))
    print(reply['result'][0]['geometry']['location'])

if __name__ == "__main__":
    geocode("207 N. Defiance St, Archbold, OH")
    

