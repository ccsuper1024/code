#!/bin/bash
#-e 比较 用于文件和目录。如果只想比较对象为文件的，则用-f比较
# check if either a directory or file exists
item_name=$HOME
echo
echo "The item bing checked: $item_name"
echo

if [ -e $item_name ]
then    #item does exist
    echo "The item,$item_name, does exist"
    echo "But is it a file?"

    #比较对象是否为文件
    if [ -f $item_name ]
    then    #Item is a file
        echo "Yes, $item_name is a file."
    else    #Item is not a file
        echo "No, $item_name is not a file"
    fi
else    #Item does not exist
    echo "The item, $item_name, does not exist"
    echo "Nothing to update"
fi