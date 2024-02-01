#!/bin/bash
# 在条件测试中检查文件是否可写
item_name=$HOME/sentinel
echo
echo "The item being checked: $item_name"
echo

if [ -f $item_name  ]
then    #文件收存在
    echo "Yes , $item_name is a file"
    echo "But is it writtable?"
    echo
    
    if [ -w $item_name  ]
    then    #文件是否可写
        echo "Writting current time to $item_name"
        date +%H%M >> $item_name
    else    
        echo "Uable to write to $item_name"
    fi
else    #文件不存在
    echo "No,$item_name is not a file"
fi
