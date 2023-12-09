/*
    @file 
    @author 陈超
    @brief 使用next_permutation()来生成组合
            排列：从n个不同的元素中取出指定个数的元素并进行排序，取出的有序元素组合就是排列数，即取出1,2,3和3,2,1不是同一个排列数
            组合：从n个不同的元素中取出制定个数的元素，不进行排序。取出的元素泽合就是组合数，即取出1，2,3和3,2,1是同一个组合数            
*/
//从N个元素中取出M个元素
#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main(void) {
    int values[] = {1,2,3,4,5,6,7};
    int elements[] = {1,1,1,0,0,0,0};
    const size_t N = sizeof(elements) / sizeof(elements[0]);
    assert(N==sizeof(values) / sizeof(values[0]));
    std::vector<int> vec(elements,elements+N);

    int count = 0;
    do {
        std::cout << ++count << ": ";
        for(size_t i = 0; i < vec.size(); ++i) {
            if(vec[i]) {
                std::cout << values[i] << ",";
            }
        }
        std::cout << std::endl;
    }while(std::prev_permutation(vec.begin(),vec.end()));
}

