#include <stdio.h>
#include "parse.h"

//判断字符是否为空白字符
int CheckBlank(char c)
{
    return (c==' ' || c=='\t' || c=='\n' );
}


char *ParseWord(char **head)
{
    char *now;
    while (CheckBlank(**head)) //跳过字符串头部的空白字符后返回指针
        (*head)++;
    
    now = *head;
    
    while ( !CheckBlank(**head)  && (**head)!='\0') //跳过字符串中第一个词
        (*head)++;
    
    if(**head == '\0') 
        return now;
    **head = '\0';   //将字符串中第一个词后的空格变为'\0'
    (*head)++;
    return now;
    
}