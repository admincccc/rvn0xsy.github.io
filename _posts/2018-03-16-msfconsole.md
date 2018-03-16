---
layout: post
title:  "msfconsole常用命令"
date:   2018-03-16
categories: Program
permalink: /archivers/2018-03-16/2
description: "本文记录一下基础命令"
---
本文记录一下基础命令
<!--more-->

>命令： show exploits

```
列出metasploit框架中的所有渗透攻击模块。
```

> 命令：show payloads

``` 
列出metasploit框架中的所有攻击载荷。
```
>  命令：show auxiliary

``` 
列出metasploit框架中的所有辅助攻击载荷。
``` 

>  命令：search name

``` 
查找metasploit框架中所有的渗透攻击和其他模块。
``` 

>  命令：info

``` 
展示出制定渗透攻击或模块的相关信息。
``` 

>  命令：use name

``` 
装载一个渗透攻击或模块。
``` 

>  命令：LHOST

``` 
你本地可以让目标主机连接的IP地址，通常当目标主机不在同一个局域网内时，就需要是一个公共IP地址，特别为反弹式shell使用。
``` 

>  命令：RHOST

``` 
远程主机或是目标主机。
``` 

>  命令：set function

``` 
设置特定的配置参数（EG：设置本地或远程主机参数）。
``` 

>  命令：setg function

``` 
以全局方式设置特定的配置参数（EG：设置本地或远程主机参数）。
``` 

>  命令：show options

``` 
列出某个渗透攻击或模块中所有的配置参数。
``` 

>  命令：show targets

``` 
列出渗透攻击所有支持的目标平台。
``` 

>  命令：set target num

``` 
指定你所知道的目标的操作系统以及补丁版本类型。
``` 

>  命令：set payload name

``` 
指定想要使用的攻击载荷。
``` 
>  命令：show advanced

``` 
列出所有高级配置选项。
``` 

>  命令：set autorunscript migrate -f.

``` 
在渗透攻击完成后，将自动迁移到另一个进程。
``` 

>  命令：check

``` 
检测目标是否选定渗透攻击存在相应的安全漏洞。
``` 

>  命令：exploit

``` 
执行渗透攻击或模块来攻击目标。
``` 

>  命令：exploit -j

``` 
在计划任务下进行渗透攻击（攻击将在后台进行）。
``` 

>  命令：exploit -z

``` 
渗透攻击完成后不与回话进行交互。
``` 

>  命令：exploit -e encoder

``` 
制定使用的攻击载荷编码方式（EG：exploit -e shikata_ga_nai）。
``` 

>  命令：exploit -h

``` 
列出exploit命令的帮助信息。
``` 

>  命令：sessions -l

``` 
列出可用的交互会话（在处理多个shell时使用）。
``` 

>  命令：sessions -l -v

``` 
列出所有可用的交互会话以及详细信息，EG：攻击系统时使用了哪个安全漏洞。
``` 

>  命令：sessions -s script

``` 
在所有活跃的metasploit会话中运行一个特定的metasploit脚本。
``` 

>  命令：sessions -K

``` 
杀死所有活跃的交互会话。
``` 

>  命令：sessions -c cmd

``` 
在所有活跃的metasploit会话上执行一个命令。
``` 

>  命令：sessions -u sessionID

``` 
升级一个普通的win32 shell到metasploit shell。
``` 

>  命令：db_create name

``` 
创建一个数据库驱动攻击所要使用的数据库（EG：db_create autopwn）。
``` 

>  命令：db_connect name

``` 
创建并连接一个数据库驱动攻击所要使用的数据库（EG：db_connect user:passwd@ip/sqlname）。
``` 

>  命令：db_namp

``` 
利用nmap并把扫描数据存储到数据库中（支持普通的nmap语句，EG：-sT -v -P0）。
``` 

>  命令：db_autopwn -h

``` 
展示出db_autopwn命令的帮助信息。
``` 

>  命令：db_autopwn -p -r -e

``` 
对所有发现的开放端口执行db_autopwn，攻击所有系统，并使用一个反弹式shell。
``` 

>  命令：db_destroy

``` 
删除当前数据库。
``` 

>  命令：db_destroy user：passwd@host：port/database

``` 
使用高级选项来删除数据库。
``` 

## ***metasploit命令***

> help
打开meterpreter使用帮助。

> run scriptname
运行meterpreter脚本，在scripts/meterpreter目录下可查看到所有脚本名。

> sysinfo
列出受控主机的系统信息。

> ls
列出目标主机的文件和文件夹信息。

> use priv
加载特权提升扩展模块，来扩展metasploit库。

> ps
显示所有运行的进程以及相关联的用户账户。

> migrate PID
迁移到一个指定的进程ID（PID号可通过ps命令从主机上获得）。

> use incognito
加载incognito功能（用来盗窃目标主机的令牌或假冒用户）

> list_tokens -u
列出目标主机用户的可用令牌。

> list_tokens -g
列出目标主机用户组的可用令牌。

> impersonate_token DOMAIN_NAME\\USERNAME
假冒目标主机上的可用令牌。

> steal_token PID
盗窃给定进程的可用令牌并进行令牌假冒。

> drop_token
停止假冒当前令牌。

> getsystem
通过各种攻击向量来提升系统用户权限。

> execute -f cmd.exe -i
执行cmd.exe命令并进行交互。

> execute -f cmd.exe -i -t
以所有可用令牌来执行cmd命令并隐藏该进程。

> rev2self
回到控制目标主机的初始用户账户下。

> reg command
在目标主机注册表中进行交互，创建，删除，查询等操作。

> setdesktop number
切换到另一个用户界面（该功能基于那些用户已登录）。

> screenshot
对目标主机的屏幕进行截图。

> upload file
向目标主机上传文件。

> download file
从目标主机下载文件。

> keyscan_start
针对远程目标主机开启键盘记录功能。

> keyscan_dump
存储目标主机上捕获的键盘记录。

> keyscan_stop
停止针对目标主机的键盘记录。

> getprivs
尽可能多的获取目标主机上的特权。

> uictl enable keyboard/mouse
接管目标主机的键盘和鼠标。

> background
将你当前的metasploit shell转为后台执行。

> hashdump
导出目标主机中的口令哈希值。

> use sniffer
加载嗅探模式。

> sniffer_interfaces
列出目标主机所有开放的网络端口。

> sniffer_dump interfaceID pcapname
在目标主机上启动嗅探。

> sniffer_start interfaceID packet-buffer
在目标主机上针对特定范围的数据包缓冲区启动嗅探。

> sniffer_stats interfaceID
获取正在实施嗅探网络接口的统计数据。

> sniffer_stop interfaceID
停止嗅探。

> add_user username password -h ip
在远程目标主机上添加一个用户。

> clearev
清楚目标主机上的日志记录。

> timestomp
修改文件属性，例如修改文件的创建时间（反取证调查）。

> reboot
重启目标主机。


