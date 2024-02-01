#!/bin/bash
#检查文件是佛可读 -r
# Testing if you can read a file
pwfile=/etc/shadow

#first, test if the file exists, and is a file
if [ -f $pwfile  ]
then    #文件存在时
    if [ -r $pwfile  ]
    then    #文件是否可读
        tail $pwfile
    else
        echo "Sorry, I am unable to read the $pwfile file"
    fi
else
    echo "Sorry , the file $pwfile does not exist"

fi
