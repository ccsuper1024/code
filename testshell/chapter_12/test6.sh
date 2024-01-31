#!/bin/bash
#由于if-then 无法测试退出状态码之外的条件，所以shell提供了test命令来测试
#格式：test condition
#如果只写test，而不写condition部分，它会以非零的退出状态码退出，并执行else语句块
#Testing the test command
#if test
#then
#    echo "No expression returns a True"
#else
#    echo "No expression returns a False"
#fi
###########################################################
my_variable="Full"
#
#if test $my_variable
#then
#    echo "The $my_variable expression returns a True"
#else
#    echo "The $my_variable expression returns a False"
#fi

#也可以用方括号代替test命令，但是，要注意要在第一个方括号之后和第二个方括号之前都加上一个空格，否则会出错
if [ $my_variable ]
then
    echo "The $my_variable expression returns a True"
else
    echo "The $my_variable expression returns a False"
fi
