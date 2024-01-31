#!/bin/bash
# mis-using string comparisons

val1=baseball
val2=hockey

#条件测试方括号中不能直接出现>或<,因为shell会把它们认作重定向符
if [ $val1 > $val2 ]
then
    echo "$val1 is greater than $val2"
else
    echo "$val1 is less than $val2"
fi
