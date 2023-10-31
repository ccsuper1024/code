#!/usr/bin/python3
# coding=utf-8
#create directories
import os

dstdir = 'chapter'
for i in range(1,13):
    dstdir += str(i)
    os.makedirs(dstdir)
    dstdir = 'chapter'
