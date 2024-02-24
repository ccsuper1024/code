#!/bin/bash
#复合条件测试，可以使用&& 和||来实现布尔逻辑
if [ -d $HOME  ] && [ -w $HOME/testing  ]
then    #文件目录存在，并且文件可写
    echo "The file exists and you and write to it"
else    #-w选项会检查目录是否存在
    echo "The file does not exists "
fi
