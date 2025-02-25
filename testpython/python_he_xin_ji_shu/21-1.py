#!/usr/bin/env python
# coding=utf-8
#单线程实现 从指定网站上下载内容
import requests
import time
import os

fileNode = 1

def download_one(url):
    global fileNode
    resp = requests.get(url)
    print(resp.headers)
    
    #检测目录是否存在，若存在则直接写入，不存在则创建
    dir = "content/"
    if not os.path.exists(dir):
        os.mkdir(dir)
        
    filename = dir + str(fileNode)
    with open(filename, "w") as file:
        file.write(resp.text)
        fileNode += 1
    print("Read {} from {}".format(len(resp.content), url))

def download_all(sites):
    for site in sites:
        download_one(site)

def main():
    sites = [
        'https://en.wikipedia.org/wiki/Portal:Arts', 
        'https://en.wikipedia.org/wiki/Portal:History',  
        'https://en.wikipedia.org/wiki/Portal:Society',  
        'https://en.wikipedia.org/wiki/Portal:Biography',  
        'https://en.wikipedia.org/wiki/Portal:Mathematics',  
        'https://en.wikipedia.org/wiki/Portal:Technology',  
        'https://en.wikipedia.org/wiki/Portal:Geography',  
        'https://en.wikipedia.org/wiki/Portal:Science',  
        'https://en.wikipedia.org/wiki/Computer_science',   
        'https://en.wikipedia.org/wiki/Python_(programming_language)',   
        'https://en.wikipedia.org/wiki/Java_(programming_language)',  
        'https://en.wikipedia.org/wiki/PHP',  
        'https://en.wikipedia.org/wiki/Node.js',   
        'https://en.wikipedia.org/wiki/The_C_Programming_Language',
    ]
    start_time = time.perf_counter()
    download_all(sites)
    end_time = time.perf_counter()
    print("Download {} sites in {} seconds".format(len(sites), end_time - start_time))

if __name__ == "__main__":
    main()
