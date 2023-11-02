#!/usr/bin/python3
# coding=utf-8
#create directories
import os

dstdir = 'day'
for i in range(1,27):
    dstdir += str(i)
    os.makedirs(dstdir)
    dstdir = 'day'
