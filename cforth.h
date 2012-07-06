/*
** cforth.h
** cforth主程序接口
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_words.h"		/* cforth.c依赖code_words.h */

#define COMPILER	0	/* 设置编译模式 */
#define INTERPRETER	1	/* 设置解释模式 */ 
#define MAX_LENGTH	1000	/* 设置字符串处理的最大长度 */
#define CODEWORDS_NUM	17	/* 设置核心字的最大数量 */ 
#define WORD_WIDTH	20	/* 设置单个核心字名字的最大宽度 */

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


