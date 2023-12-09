/* @author chenchao 
@date 
@file 
@brief 给你一个字符串，要求原地（不申请额外的内存空间）把相邻的多个空格替换成一个，例如
        输入“a  b” 输出“a b”，输入“aaa   bbb  ”，输出“aaa bbb ”
*/

#include <iostream>
#include <cassert>
#include <algorithm>
using std::endl;
using std::cout;
using std::cerr;
using std::cin;

struct AreBothSpaces{
    bool operator()(char x ,char y) const {
        return x == ' ' && y == ' ';
    }
};
void removeContinuousSpaces(std::string& str){
    //对string中的每两个字符都调用AreBothSpaces()可调用对象
    std::string::iterator last 
                = std::unique(str.begin(),str.end(),AreBothSpaces());
    str.erase(last,str.end());
}
/* @brief @pram @return */
void test(){
    std::string inout;

    inout = "";
    removeContinuousSpaces(inout);
    assert(inout=="");

    inout = "a";
    removeContinuousSpaces(inout);
    assert(inout=="a");

    inout = " a";
    removeContinuousSpaces(inout);
    assert(inout==" a");

    inout = "  a";
    removeContinuousSpaces(inout);
    assert(inout==" a");

    inout = "a ";
    removeContinuousSpaces(inout);
    assert(inout=="a ");

    inout = "a  ";
    removeContinuousSpaces(inout);
    assert(inout=="a ");

    inout = "aaa   bbb";
    removeContinuousSpaces(inout);
    assert(inout=="aaa bbb");

    inout = "a b  c d";
    removeContinuousSpaces(inout);
    assert(inout=="a b c d");

    inout = "         ";
    removeContinuousSpaces(inout);
    assert(inout==" ");
}
int main()
{
    test();
    return 0;
}

