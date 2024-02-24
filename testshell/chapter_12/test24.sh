#!/bin/bash
#双方括号可以使用针对字符串比较的高级特性，如== ，正则表达式等
if [[ $USER == c*  ]]
then
    echo "hello $USER"
else
    echo "Sorry, I do not know you "
fi
