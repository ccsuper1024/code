#!/usr/bin/python3
# coding=utf-8
#create directories
import os

dstdir = 'chapter_'
for i in range(1,14):
    dstdir += str(i)
    os.makedirs(dstdir)
    dstdir = 'chapter_'
