#!/bin/bash
#sort命令处理大写字母的顺序和test命令相反。而条件测试方框号就是test的替代
val1=Testing
val2=testing

if [ $val1 \> $val2  ]
then
    echo "$val1 is greater than $val2"
else
    echo "$val1 is less than $val2"
fi
