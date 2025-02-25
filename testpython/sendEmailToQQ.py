#!/usr/bin/env python
# coding=utf-8
#通过python的smtplib模块向QQ邮箱发送邮件
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header

"""
启用 QQ 邮箱的 SMTP 服务

登录 QQ 邮箱。
打开邮箱设置 > 账户 > POP3/IMAP/SMTP/Exchange/CardDAV/CalDAV 服务。
启用 SMTP 服务。
获取授权码（用于代替邮箱密码）。
Python 配置
使用 smtplib 模块建立 SMTP 连接，并使用 email 模块构建邮件内容。
"""
#发送者和接受者信息
sender = "102420474@qq.com"
receiever = "102420474@qq.com"
smtp_server = "smtp.qq.com"
smtp_port = 465                     #qq邮箱使用的SSL端口
auth_code = ""                      #qq邮箱的邮箱授权码

#构建邮件内容。 此处用一个类来包裹邮件的所有信息最佳
subject = "测试邮件"                        #主题
content = "这是一封测试邮件，有python发送"  #内容

#创建MIME对象
message = MIMEText()
message["From"] = Header("发件人昵称", "utf-8")       #自定义发件人昵称
message["To"]   = Header("收件人昵称", "utf-8")       #自定义收件人昵称
message["Subject"] = Header(subject, "utf-8")         


#添加邮件正文
message.attach(MIMEText(content, "plain", "utf-8"))

try:
    #建立SMTP连接
    with smtplib.SMTP_SSL(smtp_server, smtp_port) as smtp:      
        smtp.login(sender, auth_code)
        smtp.sendmail(sender, receiever, message.as_string())       #发送邮件
    print("邮件发送")
except Exception as e:
    print("邮件发送失败：{}".format(e))
