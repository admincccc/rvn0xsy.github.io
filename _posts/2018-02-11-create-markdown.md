---
layout: post
title: "无聊写个C语言创建Jekyll文章的程序"
date: 2018-02-11
categories: 高效
permalink: /archivers/2018-02-11/1
description: "记录一下写的这个程序"
---
记录一下写的这个程序
<!--more-->

## 0x01 Jekyll

Jekyll没有自己像hexo一样自动创建文章的程序，我就无聊造个轮子，当然还存在许多隐藏的BUG。

文档格式：

```

---
layout: post
title: "标题"
date: 2018-02-11
categories: 高效
permalink: /archivers/2018-02-11/1
description: "描述"
---
记录一下写的这个程序
<!--more-->
```



## 0x02 代码

```c

#include <stdio.h>
#include <string.h>
#include <event.h>
#include <stdlib.h>

typedef struct{
    char DocumentTitle[200];
    struct tm * CurrentDay;
    char Categories[200];
    char Permalink[500];
    char Description[300];
    char CurrentTime[100];
    char CurrentDays[100];
}Document;


int initDocument(Document * Doc){
    memset(Doc,0, sizeof(Document));
    return 0;
};


int getDocumentTitle(Document * Doc){

    printf("Document Title :");

    gets(Doc->DocumentTitle);

    if(Doc->DocumentTitle[0] == 0){
        return -1;
    }
    return 0;
}


int getDocumentCategories(Document * Doc){
    printf("Document Categories :");

    gets(Doc->Categories);

    if(Doc->Categories[0] == 0){
        return -1;
    }

    return 0;
}

int getDocumentDescription(Document * Doc){
    printf("Document Description :");

    gets(Doc->Description);

    if(Doc->Description[0] == 0){
        return -1;
    }

    return 0;
}

int getDocumentPermalink(Document * Doc){
    printf("Document Permalink :");
    char buff[400] = {0x2f,0x61,0x72,0x63,0x68,0x69,0x76,0x65,0x72,0x73,0x2f};
    gets(Doc->Permalink);

    if(Doc->Permalink[0] == 0){
        return -1;
    }

    for (int i = 0; i < strlen(Doc->Permalink); ++i) {
        if(Doc->Permalink[i] == '\x0d' || Doc->Permalink[0] == '\x0a'){
            Doc->Permalink[i] = '\x5f';
        }
    }
    time_t t;
    time(&t);

    Doc->CurrentDay = localtime(&t);

    // strcpy(time,);
    sprintf(Doc->CurrentTime, "%04d-%02d-%02d %02d:%02d:%02d", Doc->CurrentDay->tm_year + 1900, Doc->CurrentDay->tm_mon+1, Doc->CurrentDay->tm_mday, Doc->CurrentDay->tm_hour, Doc->CurrentDay->tm_min, Doc->CurrentDay->tm_sec);

    sprintf(Doc->CurrentDays,"%04d-%02d-%02d",Doc->CurrentDay->tm_year + 1900, Doc->CurrentDay->tm_mon+1, Doc->CurrentDay->tm_mday);
    // printf("Current time : %s \n",Doc->CurrentDays);
    char LF[2] = {'\x2f'};
    strcat(buff,Doc->CurrentDays);
    strcat(buff,LF);
    strcat(buff,Doc->Permalink);
    memset(Doc->Permalink,0, sizeof(Doc->Permalink));
    strcat(Doc->Permalink,buff);
    return 0;
}

int CreateDocument(Document * Doc,char * file){
    char buff[1000];
    sprintf(buff,"---\n"
            "layout: post\n"
            "title: \"%s\"\n"
            "date: %s\n"
            "categories: %s\n"
            "permalink: %s\n"
            "description: \"%s\"\n"
            "---\n%s\n<!--more-->\n\n",
            Doc->DocumentTitle,
            Doc->CurrentDays,
            Doc->Categories,
            Doc->Permalink,
            Doc->Description,
            Doc->Description
    );
    FILE * fp;
    fp = fopen(file,"w+");
    fwrite(buff ,strlen(buff),1,fp);
    fclose(fp);
    return 0;
}


int main(int argc,char * argv[]) {
    char lines[30];
    memset(lines,0, sizeof(lines));
    memset(lines,'\x2d', sizeof(lines)-1);
    lines[sizeof(lines)-1] = '\0';
    // --------------------
    printf("%s \nC Program Create Jekyll Markdown Document\n"
                   "Author:Rvn0xsy\n"
                   "Email:payloads@aliyun.com\n"
                   "Blog:payloads.online\n%s \n",lines,lines);

    Document * Doc = malloc(sizeof(Document));

    initDocument(Doc);

    while(getDocumentTitle(Doc) == -1){
        printf("Please input Document Title ...\n");
    }

    printf("Document Title %s \n",Doc->DocumentTitle);

    while(getDocumentCategories(Doc) == -1){
        printf("Please input Document Categories ...\n");
    }

    printf("Document Categories %s \n",Doc->Categories);

    while(getDocumentPermalink(Doc) == -1){
        printf("Please input Document Permalink ...\n");
    }

    printf("Document Permalink %s \n",Doc->Permalink);

    while(getDocumentDescription(Doc) == -1){
        printf("Please input Document Description ...\n");
    }

    printf("Document Description %s \n",Doc->Description);

    if(CreateDocument(Doc,argv[1]) == 0){
        printf(" Create Success ..\n");
    }else{
        printf(" Create Failed ...\n");
    }

    free(Doc);
    return 0;
}


```