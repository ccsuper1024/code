#!/bin/bash
#case 命令

#在一个方括号中，"="就是等于号，而两个方框中"=="才是等于号
if [ $USER = "chenchao"  ]
then
    echo "Welcome $USER"
    echo "Please enjoy your visit"
elif [ $USER = "barbara"  ]
then
    echo "Welcome $USER"
    echo "Please enjoy your visit"
elif [ $USER = "testing"  ]
then
    echo "Special testing account"
elif [ $USER = "jessica"  ]
then
    echo "Do not forget to logout when you're done"
else
    echo "Sorry, you are not allowed here"
fi
