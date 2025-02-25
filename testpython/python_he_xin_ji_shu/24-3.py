import sys

a = []      #全局变量

#两次引用，一次来自a， 一次来自getrefcount
print(sys.getrefcount(a))

def func(a):
    #4次引用, a , python的函数调用栈，函数参数，和getrefcount
    print(sys.getrefcount(a))

func(a)

#2次引用，1次来自a, 1次来自getrefcount，函数func调用已经不存在
print(sys.getrefcount(a))