#!/bin/bash
#在条件测试中检查对象是否存在
# check if either a directory or file exists

location=$HOME
file_name="sentinel"
echo $HOME

if [ -e $location  ]
then    #Directory does exist
    echo "Ok on the $location directory"
    echo "Now checking on the file, $file_name"

    if [ -e $location/$file_name  ]
    then    #File does exist
        echo "Ok on the file_name"
        echo "Updating Current Date..."
        date >> $location/$file_name
    else    #File does not exist
        echo "File does not exist"
        echo "so ,we make a new file"
        touch $location/$file_name
        date >> $location/$file_name
    fi
else    #Director does not exist
    echo "The $location directory does not exist"
    echo "Nothing to Update"

fi
