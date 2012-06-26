/**************************** cforth.c **********************************
 * Name: Cforth	0.3.1							*
 * Copyright: ear & xiaohao						*
 * Author: ear & xiaohao						*
 * Date: 25-06-12 19:11							*
 * Description: Cforth is a forth interpreter, using C language		*
 ************************************************************************/	 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_words.h"
#include "colon_words.h" 
#define COREWORDS_NUM 17
#define WORD_WIDTH 20


/*使用函数指针在解释模式下按个搜索字典中的词*/
const char word_str[COREWORDS_NUM][WORD_WIDTH] = 
{	".s",	".rs",	".",	"swap",	">r",
	"r>",	"dup",	"drop",	"2drop","2dup",
	"over",	"+",	"-",	"*",	"/d",
	"/",	"%"				};		
typedef void (*pType) (void) ;
pType arr[COREWORDS_NUM] = 
{	showDS,	showRS,	showtopDS,swap,	tor,
	rto,	dup,	drop,	drop2,	dup2,
	over,	add,	sub,	mul,	ddiv,
	divv,	mod				}; 


/************************************************************************/ 


/************************************************************************/ 
int main()
{
	printf("Cforth 0.3.1, ear & xiaohao Copyright (C) 2008-2012 \n");
	printf("Cforth comes with ABSOLUTELY NO WARRANTY.\n");
	printf("Enjoy it and have a good time! Type 'bye' to exit\n");

	char word_buff[WORD_WIDTH];
	while (1){ 
		scanf("%s", &word_buff);
		interpret_words(word_buff);
	}
	
    return 0;
} 
/************************************************************************/ 

/************************************************************************
 *cforth解释器模式							*/ 
int interpret_words(char *word_buff)
{
	int i = 0;
	while(i <= COREWORDS_NUM) {
		if( isNum(word_buff) ) {
		chgNum(word_buff);
		break;
		}
		else if(!strcmp(word_str[i],word_buff)) {
			arr[i]();
			break;
		}
		else if( !strcmp("bye",word_buff) ) 
			exit(0);
			
		else if(i == COREWORDS_NUM){
			printf("\n[%s]?\n",word_buff);
			clean_ds();
			break;
		}
		i++;
	}
	
	return 0;	
}
/************************************************************************/  
/*cforth解释器数字判断子程序，判断键盘输入流中是否有数字		*/ 
int isNum(char *word_buff) 
{
	while(*word_buff) {
		if(*word_buff<'0' || *word_buff>'9') return 0;
		word_buff++;
	}
        return 1;
} 

/*cforth解释器数字处理子程序，转换键盘输入流中的数字，并压入数据栈DS	*/
int chgNum(char *word_buff) 
{
	char *str=word_buff;
	int sum=0;
	while(*str) {
		sum=10*sum+(int)(*str-'0');
		str++;
	}    
	/*如果这个数字字符串只有一个'\0'，则直接返回0	* 
 	 *用来解决多余的空格键误操作DS的BUG		*/ 
	if(str==&word_buff[0]) return 0; 
	push(sum);
	return 0;
} 
/************************************************************************/ 