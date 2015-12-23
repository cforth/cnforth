#include <stdio.h>

//判断字符是否为空白字符
int is_blankchar(char c)
{
    return (c==' ' || c=='\t' || c=='\n' );
}


//跳过字符串头部的空白字符后返回指针
char * ignore_blankchar(char *s)
{
    while (is_blankchar(*s))
        s++;
    return s;
}


//跳过字符串中第一个词后返回指针
char * until_Wordend(char *s)
{
    while ( !is_blankchar(*s)  && *s!='\0')
        s++;
    return s;
}


//将字符串中第一个词后的空格变为'\0'，返回当前指针
char * split_Word(char *s)
{
    s=until_Wordend(s);
    if (*s=='\0')
        return s;
    *s='\0';
    s++;
    return s;
}
