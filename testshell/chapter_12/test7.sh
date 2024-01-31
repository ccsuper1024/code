#!/bin/bash
# 在条件测试(即[]中)中比较字符串
testuser=rich

if [ $USER=$testuser ]          #shell脚本中字符串的比较只需要一个等号,与环境变量比较
then
    echo "Welcome $testuser"
else
    echo "You should go back where you belong to"
fi
