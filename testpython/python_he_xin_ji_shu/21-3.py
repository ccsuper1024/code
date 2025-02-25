#!/usr/bin/env python
# coding=utf-8
#使用future 并发.
"""
future 模块在 Python 中用于实现异步编程，并且提供了一些用于兼容不同 Python 版本（特别是 Python 2 和 Python 3）之间的工具。
这个模块本身并不提供多线程或异步任务调度的功能，
而是用于兼容不同版本 Python 代码的运行，特别是通过在 Python 2 中支持一些 Python 3 的新特性，如 async/await。
"""
import concurrent.futures
import requests
import time

def download_one(url):
    resp = requests.get(url)
    print("Read {} from {}".format(len(resp.content), url))
def download_all(sites):
    with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
        to_do = []
        for site in sites:
            future = executor.submit(download_one, site)
            to_do.append(future)                    #将future对象加入数组中
        
        for fu in concurrent.futures.as_completed(to_do):
            future.result()                         #异步获取future对象的结果

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

        'https://en.wikipedia.org/wiki/Go_(programming_language)' 
    ]
    start_time = time.perf_counter()
    download_all(sites)
    end_time = time.perf_counter()
    print("Download {} sites in {} seconds".format(len(sites), end_time - start_time))


if __name__ == "__main__":
    main()
