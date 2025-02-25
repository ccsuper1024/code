#!/usr/bin/env python
# coding=utf-8
#使用Requests库访问google map 获取地址的经纬度
import requests

def geocode(address):
    parameters = {"address": address, "sensor":"false"}
    base = "https://maps.googleapis.com/maps/api/geocode/json"
    response = requests.get(base, params=parameters)        #默认使用http协议访问
    answer = response.json()                                #自动将json对象反序列化成python对象

    #检查返回值
    if "results" in answer and len(answer["results"]) > 0:
        print(answer["results"][0]["geometry"]["location"])
    else:
        print("Error or no results:",answer.get("status", "Unknown Error"))

if __name__ == "__main__":
    geocode("207 N. Defiance St, Archbold, OH")
