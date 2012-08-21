/*
** cforth.h
** cforth主程序接口
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code_words.h"    /* cforth.c依赖code_words.h */
#include "dict.h"

/*
** gets_input
** 从标准输入流中读取最多n-1个字符，并储存到*s指向的字符串中。
** 将最后的'\n'字符替换为'\0'
*/
int gets_input( char *s, int n ); 

/*
** interpret_words
** 接收字符串作为参数，并执行解释模式。 
*/
int interpret_words( char *s );

/*
** what_is 
** 接收字符串作为参数，判断是何种类型的词。 
** 整型数字返回1，冒号返回2，其他返回0。 
*/
int what_is ( char *s ); 

/*
** compiler_words
** 编译器模式，预留了位置，代码未完成。 
*/
int compiler_words( char *s );

/*
** ignore_words
** 注释模式，忽略输入的词。 
*/
int ignore_words( char *s );
