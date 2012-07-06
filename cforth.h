/*
** cforth.h
** cforth主程序接口
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_words.h"		/* cforth.c依赖code_words.h */

/*
** gets_input
** 从标准输入流中读取字符串，并储存到*str指向的字符串中。 
*/
int gets_input( char *str ); 

/*
** interpret_words
** 接收字符串作为参数，并执行解释模式。 
*/
int interpret_words( char *str );

/*
** isNum
** 接收字符串作为参数，判断是否为整数型字符串。 
*/
int isNum( char *str ); 

/*
** compiler_words
** 编译器模式，预留了位置，代码未完成。 
*/
int compiler_words( char *str );


