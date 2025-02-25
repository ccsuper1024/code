/*
 * 复制时src和dst的重叠导致的内存越界问题
 *
 * */
#include <cstring>
#include <iostream>

int main()
{
    char a[10] = {0};
    for(char c = 0; c < sizeof(a); ++c)
    {
        a[c] = c;
    }
    std::cout << "拷贝的src和dst存在重叠"<<std::endl;
    memcpy(&a[4], &a[0],6);     //内存越界
    return 0;
}

