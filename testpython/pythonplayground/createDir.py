#!/usr/bin/env python
# coding=utf-8
#通过读取命令行参数创建从1-N的目录

import sys
import os
#获取命令行参数并检测
if len(sys.argv) != 2:
    print("Usage:python {} [number]".format())
    sys.exit()
else:
    maxdirectnumber = int(sys.argv[1])
    print("maxdirectnumber is {}".format(maxdirectnumber))
#定义字符串
dirstr = "chapter_"

#创建目录
for i in range(1, maxdirectnumber):
    dirstr = dirstr +  str(i)
    if os.path.exists(dirstr):
        print("{} is exist,so we jump".format(dirstr))
        dirstr = "chapter_"
        continue
    else:
        os.mkdir(dirstr)
        dirstr = "chapter_"
