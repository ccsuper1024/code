#!/bin/bash
#Testing if a file is empty
# -s
file_name=$HOME/sentinel

if [ -f $file_name  ]
then    #文件存在
    if [ -s $file_name  ]
    then    #文件不为空
        echo "The $file_name file exists and has data in it"
        echo "Will not remove this file"
    else    #文件内数据位空
        echo "The $file_name file exists but is empty"
        echo "Deleting empty file..."
        rm $file_name
    fi
else
    echo "File,$file_name, does not exist"

fi
