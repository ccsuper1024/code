/*
@file
@brief [Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter)
generic implementation in C++
@detail A Bloom filter is a space -efficient probabilistic data structure
a query returns either "possibly in set" or "definitely not in set".
返回可能存在(因为hash函数必然会出现冲突)和必定不存在集合中的判定

More generally, fewer than 10 bits per element are required for a 1% false
positive probability, independant of the size or number of elements in the set.

It helps us to not make an "expensive operations", like disk IO - we can
use bloom filter to check incoming request, and with a good probability
get an answer of bloom filter, that we don't need to make our "expensive opration"

[Very good use case example](https://stackoverflow.com/a/30247022)

Basic bloom filter doesn't support deleting of elements , so
we don't need to implement deletion in bloom filter and bitset in
our case.

*/
#include <cassert>          //for assert断言
#include <functional>       //for list of hash function for bloom filter constructor. 函数对象
#include <initializer_list> //for initializer_list for bloom filter constructor
#include <string>           //for testing on strings
#include <vector>           //for std::vector
#include <iostream>         //for IO operations

/*
 * @namespace data_structure
 * @brief Data Structure algorithm
 */

namespace data_structures
{
    /*
        @brief Simple bitset implementation for bloom filter
    */
    class Bitset
    {
    private:
        // typedef unsigned long std::size_t
        std::vector<std::size_t> data_; // short info of this variable
        // 静态成员，归属于类，在头文件中定义，不要在头文件中初始化，否则会编译出错，因为每个包含该头文件的源文件
        // 都会包含该静态成员的副本，编译时会造成连接错误或不一致行为。
        // 而在实现文件中初始化，则不会出错，因为实现文件只会编译一次
        static const std::size_t blockSize;
        // size of integer type, that we are using in our bitset
    public:
        explicit Bitset(std::size_t); // 强制显式构造
        std::size_t size();
        void add(std::size_t);
        bool contain(std::size_t);
    };

    /*
        @brief Utility function to return the size of the inner array
        @return return the size of inner array
    */
    // 类外实现成员函数要用类名+作用域运算符限定
    std::size_t Bitset::size()
    {
        return data_.size();
    }

    /*
        @brief Bitset class constructor
        @param initSize amount of blocks, each contain sizeof(std::size_t) bits
    */
    Bitset::Bitset(std::size_t initSize)
        : data_(initSize)
    {
    }

    /*
        @brief Turn bit on position x to 1s
        @param x positon to turn bit on
        @return void
        形参名不重要
    */
    void Bitset::add(std::size_t x)
    {
        // 获取索引
        std::size_t blockIndex = x / blockSize;
        if (blockIndex >= data_.size())
        {
            data_.resize(blockIndex + 1); // 重新分配vector容器的内存空间，移动旧的内存到新的内存区域
        }
        data_[blockIndex] |= 1 << (x % blockSize);
    }

    /*
        @brief Doesn't bitset contains element x
        @param x position in biteset to check
        @returns true if bit position x is 1 , false if bit position x is 0
    */
    bool Bitset::contain(std::size_t x)
    {
        std::size_t blockIndex = x / blockSize; // 整数除法
        if (blockIndex >= data_.size())
        {
            return false;
        }
        return data_[blockIndex] & (1 << (x % blockSize));
    }

    /*
        @brief Bloom filter template class
        @params T type of elements that we need to filter
        类模板
    */
    template <typename T>
    class BloomFilter
    {
    private:
        // inner bitset for element
        Bitset set;
        // 函数对象是一个形参类型为T，返回类型为std::size_t的函数
        std::vector<std::function<std::size_t(T)>> hashFuncs;

    public:
        // BloomFilter类的构造函数接受类型为std::size_t和一个初始化列表作为参数
        BloomFilter(std::size_t, std::initializer_list<std::function<std::size_t(T)>>);
        void add(T);
        // 返回的布尔类型若为true则表示可能存在，若为false则表示必定不可能存在
        bool contains(T);
        // 不实现删除和修改操作
    };

    /*
        @brief  Constructor for Bloom filter
        @params size initial size pf Bloom filter
        @param funcs hash functions for T type
        @returns void
    */
    // 类外定义类模板的成员函数，要加上模板参数
    // size表示集合的大小，funcs表示一个hash函数的集合，可以设定为一个默认参数值
    template <typename T>
    BloomFilter<T>::BloomFilter(
        std::size_t size,
        std::initializer_list<std::function<std::size_t(T)>> funcs)
        : set(size) // 构造函数的类成员初始化列表，调用Bitset类的构造函数
          ,
          hashFuncs(funcs)
    {
    }
    /*
        @brief Add function for bloom filter
        @param x element to add to filter
        @return void
    */
    template <typename T>
    void BloomFilter<T>::add(T x)
    {
        for (std::size_t i = 0; i < hashFuncs.size(); ++i)
        {
            // call every hash function
            set.add(hashFuncs[i](x) % (sizeof(std::size_t) * set.size()));
        }
    }

    /*
        @brief Check element function for bloom filter
        @tparam T type of elements that we need to filter
        @param x element to check in filter
        @return true if the element probably appears in the filter
        @return false if the element certainly does not appear in the filter
    */
    template <typename T>
    bool BloomFilter<T>::contains(T x)
    {
        for (std::size_t i = 0; i < hashFuncs.size(); ++i)
        {
            if (set.contain(hashFuncs[i](x) % (sizeof(std::size_t) * set.size())) == false)
            {
                return false; // definetly not exist
            }
        }
        return true; // probably exist
    }

    // 以下是多个hash函数
    /*
        @brief [Function djb2](http://www.cse.yorku.ca/~oz/hash.html)
        to get hash for the given string.
        @param s string to get hash from
        @returns hash for a string
    */
    // 内部连接，静态函数
    static std::size_t hashDJB2(std::string const &s)
    { // 等于const std::string& s
        std::size_t hash = 5381;
        for (char c : s)
        { // iterator  迭代器
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }

    /**
     * @brief [Hash
     * function](https://stackoverflow.com/questions/8317508/hash-function-for-a-string),
     * to get hash for the given string.
     *
     * @param s string to get hash from
     * @returns hash for the given string
     */
    static std::size_t hashStr(std::string const &s)
    {
        std::size_t hash = 37;
        std::size_t primeNum1 = 54059;
        std::size_t primeNum2 = 76963;
        for (char c : s)
        { // 基于范围的for循环
            hash = (hash * primeNum1) ^ /*异或*/ (c * primeNum2);
        }
        return hash;
    }

    /**
     * @brief [Hash function for
     * test](https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key)
     *
     * @param x to get hash from
     * @returns hash for the `x` parameter
     */
    std::size_t hashInt_1(int x)
    {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> /*逻辑右移*/ 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }
    /**
     * @brief [Hash function for
     * test](https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key)
     *
     * @param x to get hash from
     * @returns hash for the `x` parameter
     */
    std::size_t hashInt_2(int x)
    {
        auto y = static_cast<std::size_t>(x);
        y = (y ^ (y >> 30)) * static_cast<std::size_t>(0xbf58476d1ce4e5b9);
        y = (y ^ (y >> 27)) * static_cast<std::size_t>(0x94d049bb133111eb);
        y = y ^ (y >> 31);
        return y;
    }
} // namespace data_structures

// 静态成员变量初始化
const std::size_t data_structures::Bitset::blockSize = sizeof(std::size_t);

/*
    @brief Test for bloom filter with string as generic type
    @retruns void
*/
static void test_bloom_filter_string()
{
    data_structures::BloomFilter<std::string> filter(
        10, {data_structures::hashDJB2, data_structures::hashStr});
    std::vector<std::string> toCheck{"hello", "world", "!"};
    std::vector<std::string> toFalse{"false", "world2", "!!!"};
    for (auto &x : toCheck)
    {
        filter.add(x);
    }
    for (auto &x : toFalse)
    {
        assert(filter.contains(x) == false); // 断言
    }

    for (auto &x : toCheck)
    {
        assert(filter.contains(x));
    }
}

/*
    @brief Test for bloom filter with int as generic type
    @return void
*/
static void test_bloom_filter_int()
{
    data_structures::BloomFilter<int> filter(
        20, {data_structures::hashInt_1, data_structures::hashInt_2});
    std::vector<int> toCheck{100, 200, 300, 50};
    std::vector<int> toFalse{1, 2, 3, 4, 5, 6, 7, 8};
    for (int x : toCheck)
    {
        filter.add(x);
    }
    for (int x : toFalse)
    {
        assert(filter.contains(x) == false);
    }
    for (int x : toCheck)
    {
        assert(filter.contains(x));
    }
}

/*
    @brief Test fot bitset
    @return void
*/
static void test_bitset()
{
    data_structures::Bitset set(2);
    std::vector<std::size_t> toCheck{0, 1, 5, 8, 63, 64, 67, 127};
    for (auto x : toCheck)
    {
        set.add(x);
        assert(set.contain(x));
    }
    assert(set.contain(128) == false);
    assert(set.contain(256) == false);
}
/*
    @brief Main function
    @return 0 on exit
*/

int main() {
    //run self-test implementations

    test_bitset();
    test_bloom_filter_int();
    test_bloom_filter_string();

    std::cout << "All tests have successfully passed!\n";
    return 0;
}