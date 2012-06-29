/**************************** cforth.c **********************************
 * Name: Cforth_alpha 0.3.3						*
 * Copyright: ear & xiaohao						*
 * Author: ear & xiaohao						*
 * Date: 28-06-12 16:00							*
 * Description: Cforth is a forth interpreter, using C language		*
 ************************************************************************/	 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_words.h"
#include "colon_words.h"
#define COMPILER	0
#define INTERPRETER	1
#define MAX_LENGTH	1000
#define COREWORDS_NUM	20
#define WORD_WIDTH	20


/*使用函数指针在解释模式下挨个搜索字典中的词*/
const char word_str[COREWORDS_NUM][WORD_WIDTH] = 
{	".s",	".rs",	".",	"swap",	">r",
	"r>",	"dup",	"drop",	"2drop","2dup",
	"over",	"+",	"-",	"*",	"/",
	"/d",	"%",	"--",	"++",	"rot"	};
		
typedef void (*pType) (void) ;
pType word_pointer[COREWORDS_NUM] = 
{	showDS,	showRS,	showTop,swap,	tor,
	rto,	dup,	drop,	drop2,	dup2,
	over,	add,	sub,	mul,	ddiv,
	divv,	mod,	sub1,	add1,	rot	}; 
/************************************************************************/ 


/************************************************************************
 *cforth主控制结构							*/  
int main()
{
	printf("Cforth_alpha 0.3.3\n"); 

	char* token;
	int status = INTERPRETER;
	char input_buff[MAX_LENGTH];
	
	while(1) { 
		printf(">>>");
		gets_input(input_buff);		
		token = strtok(input_buff, " ");
		
		while(token != NULL) {
			if(status == INTERPRETER)
        			status = interpret_words(token);
        			
        		else if(status == COMPILER)
        			status = compiler_words(token);
        			
        		token = strtok(NULL, " ");
    		}
	}	
	return 0;
} 
/************************************************************************/ 

/************************************************************************
 *字符串输入接收函数,可以从键盘或者文本中接收输入字符串 		*/
int gets_input(char* str)
{
	char c;
	for(; (c = getchar()) != '\n'; str++)
		*str = c;
	*str = '\0';
	return 0;
}
/************************************************************************/ 


/************************************************************************
 *cforth解释器函数							*/ 
int interpret_words(char* str)
{
	int i;
	int status = INTERPRETER;
	for (i = 0; i <= COREWORDS_NUM; i++) {
		if( isNum(str) ) {
			push(atoi(str));
			break;
		}
		
		else if( !strcmp(word_str[i], str) ) {
			word_pointer[i]();
			break;
		}
		
		else if( !strcmp(":", str) ) {
			status = COMPILER;
			break;
		}
		
		else if( !strcmp("bye", str) ) 
			exit(0);
			
		else if(i == COREWORDS_NUM) {
			printf("Undefine word!\n-->%s<--\n",str);
			clean_ds();
			break;
		}
	}
	
	return status;
}

/*数值字符串判断函数，是int型则返回1，否则返回0				*/ 
int isNum(char* str) 
{
	if(*str == '-' && *(str+1) == '\0')
		return 0;

	if((*str < '0' || *str > '9') && *str != '-')
		return 0;

	str++;
	for(; *str != '\0'; str++) {
		if(*str < '0' || *str > '9')
			return 0;
	}
	return 1;
}
/************************************************************************/ 


/************************************************************************
 *cforth编译器函数,未完成						*/ 
int compiler_words(char* str)
{
	int status = COMPILER;
	if( !strcmp(";", str) ) 
		status = INTERPRETER;
	return status;
} 
/************************************************************************/ 