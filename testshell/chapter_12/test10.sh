#!/bin/bash
# 条件测试中的-n 和 -z 可以检查一个变量是否含有数据

# testing string length
val1=testing
val2=''
# -n 检查str1的长度是否为非零   -z 检查str1的长度是否为0
if [ -n $val1 ]
then
    echo "The string '$val1' is not empty"
else
    echo "The string '$val1' is empty"
fi

if [ -z $val2  ]
then
    echo "The string '$val2' is empty"
else
    echo "The string '$val2' is not empty"
fi

#如果测试一个未定义的变量，则shell默认它的长度为0，即使它未被定义过
#但最好不要这么写
if [ -z $val3 ]
then
    echo "The string '$val3' is empty"
else
    echo "The string '$val3' is not empty"

fi
