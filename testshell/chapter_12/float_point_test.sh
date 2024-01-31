#!/bin/bash
# bash shell无法处理浮点数，必须要有特殊的程序，如bc。直接比较浮点数回出错
value1=5.555
echo "The test value is $value1"

if [ $value1 -gt 5 ]
then
    echo "The test value $value1 is greater than 5"
fi
