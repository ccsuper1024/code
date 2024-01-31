#!/bin/bash
#检查目录。利用test命令的替代语法的方框号条件测试，检测目录是否存在于系统中
#Look befor you leap

jump_directory=/home/chenchao/githubCode
if [ -d $jump_directory  ]
then
    echo "The $jump_directory directory exists"
    cd $jump_directory
    ls
else
    echo "The $jump_directory directory does not exists"
fi
