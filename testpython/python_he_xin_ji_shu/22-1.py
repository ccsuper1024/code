#!/usr/bin/env python
# coding=utf-8
"""
asyncio 通过事件循环在单线程中管理多个任务，适用于 I/O 密集型任务。它通过 async 和 await 关键字使得代码在执行时可以被暂停，
并在需要时恢复，从而提高程序的效率
"""
import asyncio
import aiohttp
import time

async def download_one(url):
    async with aiohttp.ClientSession() as session:
        async with session.get(url) as resp:
            print("Read {} from {}".format(resp.content_length, url))

async def download_all(sites):
    tasks = [asyncio.create_task(download_one(site)) for site in sites]
    await asyncio.gather(*tasks)

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
    asyncio.run(download_all(sites))
    end_time = time.perf_counter()
    print("Download {} sites in {} seconds".format(len(sites), end_time - start_time))


if __name__ == "__main__":
    main()
