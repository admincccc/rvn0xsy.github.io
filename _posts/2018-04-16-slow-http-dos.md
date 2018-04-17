---
layout: post
title:  "Slow HTTP DOS"
date:   2018-04-16
categories: Web安全测试学习手册
permalink: /archivers/2018-04-16/2
description: "《Web安全测试学习手册》- Slow HTTP DOS"
---
《Web安全测试学习手册》- Slow HTTP DOS
<!--more-->

## 0x00 Slow HTTP DOS - 介绍

### 1）什么是Slow HTTP DOS

Slow HTTP DOS(Slow HTTP Denial of Service Attack)，译为缓慢的HTTP拒绝服务，这类攻击方式出现在许多公开协议中。

### 2）Slow HTTP DOS的特点

Slow HTTP DOS是一个应用层拒绝服务攻击，主要针对协议为HTTP，攻击的成本很低，并且能够消耗服务器端资源，占用客户端连接数，导致正常用户无法连接服务器。

## 0x01 Slow HTTP DOS - 风险等级

**中**

## 0x02 Slow HTTP DOS - 原理

既然是一个HTTP协议的缓慢攻击，这就要从HTTP协议说起了。

首先HTTP协议的报文都是一行一行的，类似于：

```
GET / HTTP/1.1\r\n
Host : payloads.online\r\n
Connection: keep-alive\r\n
Keep-Alive: 900\r\n
Content-Length: 100000000\r\n
Content_Type: application/x-www-form-urlencoded\r\n
Accept: *.*\r\n
\r\n
```

那么报文中的`\r\n`是什么？

`\r\n`代表一行报文的结束也被称为空行（CLRF），而`\r\n\r\n`代表整个报文的结束

从上面贴出的`GET`请求包可以看出，我们的客户端请求到服务器后，告知服务器这个连接需要保留。

通常我们知道HTTP协议采用“请求-应答”模式，当使用普通模式，即非KeepAlive模式时，每个请求/应答客户和服务器都要新建一个连接，完成之后立即断开连接`（HTTP协议为无连接的协议）`；当使用`Keep-Alive模式（又称持久连接、连接重用）`时，Keep-Alive功能使客户端到服 务器端的连接持续有效，当出现对服务器的后继请求时，Keep-Alive功能避免了建立或者重新建立连接。


那么当我们客户端发送一个报文，不以`CRLF`结尾，而是10s发送一行报文，我们的报文需要80s才能发送完毕，这80s内，服务器需要一直等待客户端的CRLF，然后才能解析这个报文。


如果客户端使用更多的程序发送这样的报文，那么服务器端会给客户端留出更多的资源来处理、等待这迟迟不传完的报文。假设服务器端的客户端最大连接数是100个，我们使用测试程序先连接上100次服务器端，并且报文中启用Keep-Alive，那么其他正常用户101、102就无法正常访问网站了。


## 0x03 Slow HTTP DOS - 常见场景

大多出现在默认安装好的Apache Web中，未合理设置客户端连接数导致的。

## 0x04 测试方案

使用Slow HTTP Test 工具进行检测

Kali Linux 安装 ：apt-get install slowhttptest 


SlowHTTPTest是一个可配置的应用层拒绝服务攻击测试攻击，它可以工作在Linux，OSX和Cygwin环境以及Windows命令行接口，可以帮助安全测试人员检验服务器对慢速攻击的处理能力。

这个工具可以模拟低带宽耗费下的DoS攻击，比如慢速攻击，慢速HTTP POST，通过并发连接池进行的慢速读攻击（基于TCP持久时间）等。慢速攻击基于HTTP协议，通过精心的设计和构造，这种特殊的请求包会造成服务器延时，而当服务器负载能力消耗过大即会导致拒绝服务。

### Slow Header

```
slowhttptest -c 65500 -H -i 10 -r 200 -s 8192 -t SLOWHEADER -u http://payloads.online
```

该攻击会像我们刚才讲的慢速传递HTTP报文，占用服务器资源让其等待我们最后的CRLF。


### Slow Read

```
slowhttptest -c 65500 -X -r 1000 -w 10 -y 20 -t SLOWREAD -n 5 -z 32 -u http://payloads.online
```

该攻击会在Web服务器响应内容传输回来的时候，我们客户端缓慢的读取响应报文，这样服务器端也会一直等待客户端来接收完毕。


### Slow Post

```
slowhttptest -c 65500 -B -i 10 -r 200 -s 8192 -t SLOWBODY -u http://payloads.online
```

该攻击会构造一个POST数据包，将数据缓慢传输，使服务器端一直等待接收报文。

## 0x05 修复方案

* 1.设置合适的 timeout 时间（Apache 已默认启用了 reqtimeout 模块），规定了 Header 发送的时间以及频率和 Body 发送的时间以及频率

* 2.增大 MaxClients(MaxRequestWorkers)：增加最大的连接数。根据官方文档，两个参数是一回事，版本不同，MaxRequestWorkers was called MaxClients before version 2.3.13. The old name is still supported.

* 3.默认安装的 Apache 存在 Slow Attack 的威胁，原因就是虽然设置的 timeoute，但是最大连接数不够，如果攻击的请求频率足够大，仍然会占满 Apache 的所有连接
