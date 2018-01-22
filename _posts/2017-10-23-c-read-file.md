---
layout: post
title:  "C语言读取文件"
date:   2017-10-23
categories: C++/C
permalink: /archivers/2017-10-23/2
description: "C语言读取文件"
---
如何实现在C++中的getline函数
<!--more-->
## 0x00 问题

如何实现在C++中的getline函数

## 0x01 自行封装

今天下班前被小Lin童鞋叫住了，他要开发一个Python的项目，需要读取文件，我就想我们一块写，看谁写得快！

我信誓旦旦的选择了C语言 :(   -  Python的确不想碰了。

实现之前想了很多，根据自己对C++中读取文件时使用getline的理解，自己写了下面这些代码，但是有一个内存分配无法释放的问题，想留在明天解决


还要看刀剑神域呢 ~  

## 0x02 源代码

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mm_malloc.h>
void error_handle(char * msg);
char * get_str_line(FILE * fp,char * lines);
int get_file_lines(FILE * fp);
int get_str_num(FILE * fp);
/**
 * 获取文件所有行数
 * @param fp
 * @return
 */
int get_file_lines(FILE * fp){
    int l = 0;
    while(!feof(fp)){
        if(fgetc(fp) == '\n'){
            l ++ ;
        }
    }
    fseek(fp,0,SEEK_SET);
    return l;
}

/**
 * 输出错误处理
 * @param msg
 */
void error_handle(char * msg){
    printf("Error Message : %s",msg);
    exit(-1);
}

/**
 * 获取当前行字符数
 * @param fp
 * @return
 */
int get_str_num(FILE * fp){
    int  c_line = 0; //
    while(!feof(fp)){
        if(fgetc(fp) == '\n'){
            break;
        }
        c_line++;
    }
    return c_line;
}

/**
 * 获取文件当前行字符串
 * @param fp
 * @return
 */
char * get_str_line(FILE * fp,char * lines){
    long current_fp = ftell(fp); // 获得当前指针位置
    int  c_line = 0; //
    fseek(fp,current_fp,SEEK_SET); // 将文件指针恢复

    //char * lines = (char *)malloc(c_line * sizeof(char)); // 分配动态内存 、 这个变量如何free ？
    char tmp[100];
    tmp[0] = 'a';
    c_line = 0; // 变量复用
    char ch;    // 临时变量 用于存储字符
    while(!feof(fp)){   // 如果文件还未结束
        ch  = fgetc(fp); // 读取字符
        if(ch==EOF){    // 如果文件到达尾部
            break;          // 退出循环
        }
        if(ch == '\n'){  // 如果遇到换行 退出循环
            break;
        }
        lines[c_line] = ch; // 将取得的字符赋值给当前行变量
        c_line ++;
    }
    return lines; // 返回一行字符
}

int main() {

    // 打开文件
    FILE * fp = fopen("100.txt","r");

    // 将文件指针重置为起始位置
    fseek(fp,0,SEEK_SET);

    // 获取文件行数
    int FILE_LINE = get_file_lines(fp);

    // 分配一个一位数组 元素个数为 FILE_LINE + 1
    char * dic_lines[FILE_LINE+1];

    // 动态分配内存 用于存储每一行内容
    for(int x = 0; x <= FILE_LINE;x++){
        dic_lines[x] = (char *)malloc(sizeof(char)*100); // 分配一个一维数组，每个元素100个字节
    }
    //char lines[100];
    // 将每行放入一维数组
    for(int a = 0;a <= FILE_LINE;a++){
        char * lines = (char *)malloc(sizeof(char)*100);
        dic_lines[a] = get_str_line(fp,lines);
    }

    // 放入内存后，怎么操作都随你了 ~

    // 释放内存
    for(int x = 0; x <= FILE_LINE;x++){
        printf("line : %d,string is %s \n",x,dic_lines[x]); // 萌萌哒的在释放之前输出 ~ 
        free(dic_lines[x]);
    }

    // 关闭文件
    fclose(fp);
    return 0;
}
```

下面在总结一下C语言对文件的操作函数，明天写

## 0x03 C语言文件操作函数



