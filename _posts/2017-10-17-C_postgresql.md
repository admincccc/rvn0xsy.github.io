---
layout: post
title:  "使用C++操作Postgresql"
date:   2017-10-18
categories: database C++/C
permalink: /archivers/2017-10-17/2
description: "本文简述一下近期想做的一个项目铺垫，使用C++操作Postgresql"
---
本文简述一下近期想做的一个项目铺垫
<!--more-->
## 0x00 安装扩展库 - libpqxx

C++有个库叫 libpqxx , 可以通过以下操作安装完成：

```
git clone https://github.com/jtv/libpqxx.git # clone软件包
apt-get install pkg-config
apt-get install make # 安装make
apt-get install g++ # 安装g++编译器
apt-get install libpq-dev
cd libpqxx
./configure
make
make install
```

## 0x01 编写测试代码

```c++
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
   try{
      connection C("dbname=testdb user=psqler password=123456 hostaddr=127.0.0.1 port=5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }
      C.disconnect ();
   }catch (const std::exception &e){
      cerr << e.what() << std::endl;
      return 1;
   }
}
```

如果输出：Opened database successfully: testdb

表示连接成功 ~

参考：http://www.yiibai.com/html/postgresql/2013/080894.html



