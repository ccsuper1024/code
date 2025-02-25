#!/usr/bin/env python
# coding=utf-8
#解码输入字节，编码输出字符

if __name__ == "__main__":
    #将字节序列转换成Unicode字符
    input_bytes = b'\xff\xfe4\x001\x003\x00 \x00i\x00s\x00n\x00.\x00'
    input_characters = input_bytes.decode('utf-16')
    print(repr(input_characters))

    #在发送字符序列前将字符序列转成字节序列
    output_characters = 'We copy you down, Eagle.\n'
    output_bytes = output_characters.encode("utf-8")
    with open('eagle.txt', 'wb') as f:
        f.write(output_bytes)