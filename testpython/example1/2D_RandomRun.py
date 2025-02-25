#!/usr/bin/env python
# coding=utf-8

#2D 随机游走
#本例子使用numpy生成随机步长，并使用matplotlib绘制随机游走轨迹
#同时展示多次试验的统计信息和结果图表

import numpy as np
import matplotlib.pyplot as plt

                            #默认参数
def simulate_random_walk(steps=1000):
    """
    模拟一个二维随机游走。
    参数：
        steps：int 模拟步数

    返回：
        x,y: numpy数组， 分别表示每一步的x和y坐标

    """
    #为每一步生成随机角度(2 ~ 2π)

    theta = np.random.uniform(0, 2 * np.pi, steps)

    #步长均为1， 计算想，y方向的变化量
    dx = np.cos(theta)
    dy = np.cos(theta)
    
    #输出dx，和dy，看这两个变量长什么样
    print("dx = {}".format(dx))
    print("dy = {}".format(dy))
    
    #累加计算每一步的位置
    x = np.cumsum(dx)
    y = np.cumsum(dy)

    #输出x,y的值
    print("x = {}".format(x))
    print("y = {}".format(y))


def compute_statistics(x, y):
    """
    计算随机游走的统计信息，包括最终位置，总步数以及起点到终点的直线距离

    返回：
        fina_pos: tuple, 最终位置(x, y)
        total_steps： int, 步数(实际步长数)
        displacement: float, 起点到终点的欧式距离
    """
    final_pos = (x[-1], y[-1])                  #元组，取x,y中的最后一个元素
    total_steps = len(x)
    displacement = np.sqrt(x[-1]**2 + y[-1]**2) #平方和开根号，求欧式距离 勾股定理
    return final_pos, total_steps, displacement #返回一个元组

#默认参数要放在函数参数的最后
def plot_random_walk(x, y, walk_number = 1):
    """
    绘制单个随机游走的轨迹图

    """    
    plt.figure(figsize=(8,5))
    plt.plot(x, y,label="随机游走{}".format(walk_number), marker='o', markersize=2, linestyle='-')
    plt.title()
    

