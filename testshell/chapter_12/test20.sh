#!/bin/bash
#条件测试的方括号的 -nt用来检测一个文件是否比另一个文件新。而-ot则用来比较一个文件是否
#比另一个旧     newer than older than
#注意：这两个选项都不会检查文件是否真的存在，如果有一个文件不存在，则会出现错误结果
if [ test19.sh -nt test18.sh  ]
then
    echo "The test19 file is newer than test18 file"
else
    echo "The test19 file is older than test18 file"
fi

if [ test17.sh -ot test19.sh  ]
then
    echo "The test19 file is newer than test17 file"
else
    echo "The test19 file is older than test18 file"
fi
