/* @file HeapTimer.cpp
 * @brief 小根堆实现的定时器，用来剔除非活动的连接
 * */
#include <iostream>
#include "HeapTimer.hpp"


using std::cout;
using std::cin;
using std::endl;
using std::cerr;

/* @brief 
 * @param   
 * @return */
void HeapTimer::Clear() {
    ref_.clear();
    heap_.clear();
}

//交换堆中的结点的编号
void HeapTimer::SwapNode_(size_t i , size_t j) {
    //vector的size()是从1开始的
    assert(i >= 0 && i < heap_.size());
    assert(j >= 0 && j < heap_.size());
    std::swap(heap_[i],heap_[j]);
    ref_[heap_[j].id_] = i;
    ref_[heap_[i].id_] = j;
}
//提升
void HeapTimer::SiftUp_(size_t i) 
{
    assert(i >= 0 && i < heap_.size());
    for(size_t j = (i-1)/2 ; j >= 0 ; i=j,j = (i-1)/2){
        if(heap_[j] < heap_[i] ){
            break;
        }
        SwapNode_(i,j);
    }
}
//下降
bool HeapTimer::SiftDown_(size_t index,size_t n) 
{
    assert(index >= 0 && index < heap_.size());
    assert(n >= 0 && heap_.size());
    size_t i = index;
    size_t j = i * 2 + 1;
    while(j < n){
        if(j+1 < n && heap_[j+1] < heap_[j])
            j++;
        if(heap_[i] < heap_[j])
            break;
        SwapNode_(i,j);
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}

//调整堆结点
void HeapTimer::Adjust(int id,int timeOut) 
{
    /*调整指定id的节点*/
    assert(!heap_.empty() && ref_.count(id) > 0);
    heap_[ref_[id]].expires_ = Clock::now() + Ms(timeOut);
}

//添加堆节点
void HeapTimer::Add(int id,int timeOut,const TimeoutCallback& cb) 
{
    assert(id >= 0);
    size_t i;
    if(ref_.count(id) == 0){
        /*新节点：堆尾插入，调整堆*/
        i = heap_.size();
        ref_[id] = i;
        heap_.push_back({id,Clock::now()+Ms(timeOut),cb});  /*调用了TimeNode结点的默认复制构造函数*/
        SiftUp_(i); 
    }else{
        /*已有结点，则调整堆*/
        i = ref_[id];
        heap_[i].expires_ = Clock::now() + Ms(timeOut) ;
        heap_[i].cb_ = cb;
        if(!SiftDown_(i,heap_.size())){
            SiftUp_(i);
        }
    }
}
//将指定节点移动到堆尾，再删除堆尾
void HeapTimer::Del_(size_t index) 
{
    /*删除指定位置的结点*/
    assert(!heap_.empty() && index >= 0 && index < heap_.size());
    /*将要删除的结点换到队尾，然后调整堆*/
    size_t i = index;
    size_t n = heap_.size() - 1;
    assert(i <= n);
    if(i < n){
        //交换指定的结点和最后一个结点
        SwapNode_(i,n);
        if(!SiftDown_(i,n)){
            SiftUp_(i);
        }
    }
    /*队尾元素删除*/
    ref_.erase(heap_.back().id_);
    heap_.pop_back();
}
void HeapTimer::DoWork(int id) 
{
    /*删除指定id结点，并触发回调函数*///ref_.count()返回匹配指定参数键的元素
    if(heap_.empty() || ref_.count(id) == 0){
        return;
    } 
    size_t i = ref_[id];
    TimerNode node = heap_[i];
    node.cb_();
    Del_(i);
}
void HeapTimer::Pop() 
{
    assert(!heap_.empty());
    Del_(0);
}

void HeapTimer::Tick() 
{
    /*清理超时节点*/
    if(heap_.empty()){
        return;
    }
    while(!heap_.empty()){
        TimerNode node = heap_.front();
        if(std::chrono::duration_cast<Ms>(node.expires_ - Clock::now()).count() > 0){
            break;
        }
        //回调函数
        node.cb_();
        Pop();
    }
}


int  HeapTimer::GetNextTick() 
{
    Tick();
    size_t res = -1;
    if(!heap_.empty()){
        res = std::chrono::duration_cast<Ms>(heap_.front().expires_ - Clock::now()).count();
        if(res < 0){
            res = 0;
        }
    }
    return res;
}


