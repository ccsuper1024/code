#!/usr/bin/env python
# coding=utf-8
#可视化引用关系
import objgraph

a = [1,2,3]
b = [4,5,6]

a.append(b)
b.append(a)

objgraph.show_refs([a])          #show_refs()函数需要一个可迭代对象(如列表，对象)，而[a]表示一个包含对象a的列表。此处直接传a也可以

objgraph.show_refs(a)      
