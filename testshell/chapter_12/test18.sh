#!/bin/bash
#条件测试中的 -o 用来测试当前程序是否是文件的属主

if [ -o /etc/passwd  ]
then
    echo "You are the owner of the /etc/passwd file"
else
    echo "Sorry,you are not the owner of the /etc/passwd file"
fi
