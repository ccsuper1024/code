//测试unorder_map的构造函数，尤其是构造自定义类型的unorder_map类型
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
#include <utility>

//自定义类型
struct Key {
    std::string first;
    std::string second;
};

//自定义类型的hash函数对象
struct KeyHash {
    std::size_t operator()(const Key& k/*常量引用*/) const /*常量成员函数，即可被常量对象调用，也可被非常量对象调用*/ {
        return std::hash<std::string>()(k.first) 
        ^/*异或*/ std::hash<std::string>()(k.second);
    }
};

struct KeyEqual{
    bool operator()(const Key& lhs, const Key& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

//自定义类型，内部具有成员==运算符
struct Foo {
    int val_;
    Foo(int val):val_(val) {

    }
    bool operator==( const Foo& rhs) const {
        return this->val_ == rhs.val_;
    }
};

//自定义类型的hash模板特化
//hash函数在std命名空间中，特化需要在std命名空间中声明
namespace std {
    template<>
    struct hash<Foo> {
        std::size_t operator()(const Foo& f) const {
            /*如果Foo类型是class，即默认的访问控制和默认继承方式为private时，要把当前的特化模板类定义为Foo类的友元*/
            return std::hash<int>{}(f.val_);   
        }
    };
}

int main() {
    //默认构造函数，空unordered_map
    std::unordered_map<std::string, std::string> m1;

    //初始化器构造函数
    std::unordered_map<int ,std::string> m2 = {
        {1, "foo"},
        {2, "bar"},
        {3, "baz"},
    };

    //复制构造函数
    std::unordered_map<int, std::string> m3 = m2;

    //移动构造函数
    std::unordered_map<int, std::string> m4 = std::move(m2);    //此时m2中的数据已空

    //范围构造函数
    std::vector<std::pair<std::bitset<8>, int>> v = {   {0x12, 1}, {0x01, -1}};
    std::unordered_map<std::bitset<8>, double> m5(v.begin(), v.end());

    //带定制Key类型，但没有==成员运算符函数的类型
    //定义KeyHash与KeyEqual结构并在模板中使用它们
    std::unordered_map<Key, std::string, KeyHash, KeyEqual> m6 = {
        {{"John", "Doe"}, "example"},
        {{"Mary", "Sue"}, "another"},
    };

    //带定制Key类型的第二种方式构造
    //为class/struct 定义const == 运算符并于std命名空间中特化 std::hash结构体
    std::unordered_map<Foo, std::string> m7 = {
        {Foo(1), "One"},{2/*隐式构造*/, "Two"}, {3,"Three"},
    };

    //第三种方式，lambda
    //注意：必须将初始同数传递给构造函数
    struct Goo{ int val;};  //嵌入类
    auto hash = [](const Goo& g){return std::hash<int>{}(g.val);};
    auto comp = [](const Goo& lhs, const Goo& rhs) { return lhs.val == rhs.val;};
    std::unordered_map<Goo, double, decltype(hash), decltype(comp)> m8(10,hash, comp);

}
