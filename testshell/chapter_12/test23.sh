#!/bin/bash
# 在双括号中可以使用高级数学表达式如自增，自减，求幂，求反，位移
val=10

if (($val ** 2 > 90))
then
    ((val2 = $val ** 2))
    echo "The square of $val is $val2"
fi
