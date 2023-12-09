/* @author chenchao 
@date 
@file 
@brief 用std::partition()实现重排数组，让奇数位于偶数前
        std::partition()作用是把符合条件的元素放到区间首部，不符合条件的元素放到区间后部
        注意：std::partition()会打乱符合条件元素本来的相对位置
        如果要保证这种相对位置的不变，即奇数元素的相对位置不变，偶数元素的相对位置不变
        则应该选择std::stable_partition()
        注意：std::stable_partition()的复杂度较为特殊：在内存充足的情况下，开辟与
        原数组一样大的空间，复杂度是O(N)时间和O(N)时间；在内存不足的情况下，要做
        in-place位置调换，复杂度是O(NlogN)时间和O(1)空间
        类似的题目还有“调整数组顺序是负数位于非负数前面”等
*/
#include <algorithm>
#include <vector>
#include <iterator>     //迭代器。是指针的泛化。包含各个标准容器元素的迭代器
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

bool isOdd(int x) {
    return x % 2 != 0; //x % 2 == 1 is wrong 要考虑负数。-1 % 2 = -1.是奇数但也不等于1
}

void moveOddsBeforeEvens() {
    int oddeven[] = {1,2,3,4,5,6};
    std::partition(std::begin(oddeven),std::end(oddeven),isOdd);
    std::copy(std::begin(oddeven),std::end(oddeven),
                std::ostream_iterator<int>(std::cout,","));  //函数模板，调用时完整的形式是要传递模板实参的，只是编译期会代劳
    std::cout << std::endl;
}
/* @brief @pram @return */
void test(){
    std::cout << "std::partition()" << std::endl;
    moveOddsBeforeEvens();
    
    std::cout << "std::stable_partition()" << std::endl;
    int oddeven[] = {1,2,3,4,5,6};
    std::stable_partition(std::begin(oddeven),std::end(oddeven),isOdd);
    std::copy(std::begin(oddeven),std::end(oddeven),
                std::ostream_iterator<int>(std::cout,","));
    std::cout << std::endl;
}
int main()
{
    test();
    return 0;
}

