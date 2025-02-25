#include <functional>
#include <queue>
#include <vector>
#include <iostream>
/*
其实就是堆
*/
template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

int main() 
{
    //基本类型的优先队列
    std::priority_queue<int> q;
    for(int n :{1,8,5,6,3,4,0,9,7,2}) {
        q.push(n);
    }
    print_queue(q);
    //自定义比较操作的优先队列
    std::priority_queue<int, std::vector<int>, std::greater<int>> q2; 
    for(int n : {1,8,5,6,3,4,0,9,7,2}) {
        q2.push(n);
    }

    print_queue(q2);

    //自定义比较操作的优先队列
    //用lambda比较元素
    auto cmp = [](int left, int right) {return (left ^ 1) < (right ^ 1);};
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
    for(int n :{1,8,5,6,3,4,0,9,7,2} ){
        q3.push(n);
    }
    print_queue(q3);

    //自定义类型的优先队列
    //自定义类中定义比较成员函数
    //特化自定义类型的比较算法，如std::greater
    //定义自定义类型的比较函数对象
}