#!/bin/bash
#用条件测试中的-x 判断文件是佛有执行权限
if [ -x test16.sh  ]
then
    echo "You can run the script: "
    ./test16.sh
else
    echo "Sorry, you are unable to execute the script"
fi
