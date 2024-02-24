#!/bin/bash
#case 命令
# using the case command

case $USER in
    chenchao | barbara)
        echo "Welcom ,$USER"
        echo "Please enjoy your visit";;
    testing)
        echo "Special testing account";;
    jessica)
        echo "Do not forget to log off when you're done";;
    *)
        echo "Sorry, you are not allowed here";;
esac
