#!/bin/bash
# testing string equality
testuser=baduser

#字符串的比较不区分大小写
if [ $USER != $testuser ]
then
    echo "This is not $testuser"
else
    echo "Welcome $testuser"
fi
