#!/usr/bin/env python
# coding=utf-8
#变量的引用计数为0时，会被python的垃圾回收机制回收
import os
import psutil

#显式当前python程序占用的内存大小
def show_memory_info(hint):
    pid = os.getpid()
    p = psutil.Process(pid)

    info = p.memory_full_info()
    memory = info.uss / 1024 /1024
    print("{} memory used: {} MB".format(hint, memory))

def func():
    show_memory_info("initial")
    a = [i for i in range(10000000)]
    show_memory_info("after a created")

if __name__ == "__main__":
    func()
    show_memory_info("finished")
