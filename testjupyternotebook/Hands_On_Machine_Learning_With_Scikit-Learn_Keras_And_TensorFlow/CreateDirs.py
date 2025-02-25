#!/usr/bin/env python
# coding=utf-8
import os
import sys


#定义创建目录的函数
def CreateDirs(number):
    dirStr = "Charpter_"
    for value in range(1, number):
        dirStr +=  str(value)
        
        #如果已存在目录则跳过，否则创建
        if os.path.exists(dirStr):
            continue
        else:
            print("创建目录 {}".format(dirStr))
            os.mkdir(dirStr)
        dirStr = "Charpter_"

#调用函数
if __name__ == "__main__":
    #获取命令行输入
    dirargv = sys.argv
    #判断命令行格式或参数个数
    if len(dirargv) != 2 and int(dirargv[1]) > 0:
        print("Wrong Usage; Usage: [Number of Charpter]")
    else:
        print("Create {} dirs".format(int(dirargv[1])))
    CreateDirs(int(dirargv[1]))
