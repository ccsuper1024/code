/*
    @file 
    @author 陈超
    @brief 使用next_permutation()来生成全排列
            排列：从n个不同的元素中取出指定个数的元素并进行排序，取出的有序元素组合就是排列数，即取出1,2,3和3,2,1不是同一个排列数
            组合：从n个不同的元素中取出制定个数的元素，不进行排序。取出的元素泽合就是组合数，即取出1，2,3和3,2,1是同一个组合数            
*/
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main(void) {
    int elements[] = {1,2,3,4};
    //const size_t N = sizeof(elements) / sizeof(elements[0]);
    std::vector<int> vec(std::begin(elements),std::end(elements));

    int count = 0;
    do{
        std::cout << ++count << ": ";
        std::copy(vec.begin(),vec.end(),
                    std::ostream_iterator<int>(std::cout,","));
        std::cout << std::endl; 
    }while (std::next_permutation(vec.begin(),vec.end()));
}