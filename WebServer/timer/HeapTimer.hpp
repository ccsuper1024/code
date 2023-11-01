/* @file HeapTime.hpp
 * @brief 大根堆实现的定时器，用于对一定时间内踢出连接
 */
#ifndef HEAPTIMER_HPP
#define HEAPTIMER_HPP
#include <time.h>
#include <arpa/inet.h>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cassert>
#include <chrono>   //chrono:计时
#include <iostream>

using std::vector;
using std::unordered_map;
using std::queue;

//一个可调用对象
typedef std::function<void()> TimeoutCallback;      
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Ms;
typedef Clock::time_point TimeStamp;

struct TimerNode{   //定时器结点类,struct 定义的类默认访问控制域时公开的
    int id_;
    TimeStamp expires_;
    TimeoutCallback cb_;
    TimerNode(int id,TimeStamp expires, TimeoutCallback cb)
    :id_(id)
     ,expires_(expires)
     ,cb_(cb)
    {

    }
    bool operator< (const TimerNode& t){
        return expires_ < t.expires_;
    }
};

class HeapTimer
{
public:
    HeapTimer() {   
        //预留64个元素空间
        heap_.reserve(64);
    }

    //自动调用
    ~HeapTimer() {
        std::cout << "~HeapTimer()" << std::endl;
        Clear();   
    }

    void Adjust(int id,int newExpires);

    void Add(int id,int timeOut,const TimeoutCallback& cb);

    void DoWork(int id);

    void Clear();

    void Tick();

    void Pop();

    int GetNextTick();

private:
    void Del_(size_t i);

    void SiftUp_(size_t i);

    bool SiftDown_(size_t index,size_t n);

    void SwapNode_(size_t i,size_t j);

    vector<TimerNode> heap_;    
    unordered_map<int,size_t> ref_; //辅助数据结构，用于记录定时器节点id对应的下标
};

#endif  //HEAPTIMER_HPP
