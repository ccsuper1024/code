#!/bin/bash
#条件测试方括号中的-G 选项会检查文件的默认组，如果它匹配了用户的默认组，则测试成功。由于
#-G比较只会检查默认组，而非用户所属的所有组

if [ -G $HOME/testing  ]
then
    echo "You are in the same group as the file"
else
    echo "The file is not owned by your group"
fi
