/*
** Cforth_alpha 0.3.5
** cforth.c	Cforth主程序
** 作者：	ear & xiaohao
** 版本号：	alpha 0.3.5
** 更新时间：	2012-08-21
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code_words.h"
#include "dict.h"
#include "forth.h"

#define COMPILER	0	/* 设置编译模式 */
#define INTERPRETER	1	/* 设置解释模式 */ 
#define	COMMENT		2	/* 设置注释模式 */
#define MAX_LENGTH	1000	/* 设置字符串处理的最大长度 */
#define CODEWORDS_NUM	21	/* 设置核心字的最大数量 */ 
#define WORD_WIDTH	20	/* 设置单个核心字名字的最大宽度 */

/*
** word_str
** 核心字索引数组，用于解释模式下核心字的搜索执行。 
*/
const char word_str[ CODEWORDS_NUM ][ WORD_WIDTH ] = 
{	".s",	".rs",	".",	"swap",	">r",
	"r>",	"dup",	"drop",	"over",	"+",
	"-",	"*",	"/",	"%",	"--",
	"++",	"rot",	"bye",	"~",	"bool",
	"look"	};

/*
** word_pointer
** 函数指针数组，用于解释模式下核心字的搜索执行。 
*/		
typedef void ( *pType ) ( void ) ;
pType word_pointer[ CODEWORDS_NUM ] = 
{	sh_ds,	sh_rs,	pop,	swap,	tor,
	rto,	dup,	drop,	over,	add,
	sub,	mul,	div_new,mod,	sub1,
	add1,	rot,	bye,	negate,	bool1,
	printf_dict	}; 

/*
** name为冒号字名字缓冲区，defin为冒号字定义缓冲区。
** if_name 为冒号字位置状态，1为名字，0为定义。
*/
static char name[20];
static char defin[100];
static int if_name = 1;


/* 
** Cforth主控制结构，目前只有解释器功能。 
*/  
int main( void )
{
	printf("Cforth_alpha 0.3.5\n"); 

	char *next = NULL;
	char *tmp = NULL;
	char *d = " ";
	int status = INTERPRETER;
	char input_buff[ MAX_LENGTH ];
	
	while(1) { 
		printf(">>>");
		gets_input(input_buff, MAX_LENGTH);		
		next = strtok_r(input_buff, d, &tmp);
		
		while(next != NULL) {
			switch(status) {
			case INTERPRETER:
        		status = interpret_words(next);
				break;
        			
        	case COMPILER:
        		status = compiler_words(next);
				break;
        			
        	case COMMENT:
        		status = ignore_words(next);
				break;
        	}
        	next = strtok_r(NULL, d, &tmp);
    	}
	}	
	return 0;
} 


/*
** gets_input 
*/
int gets_input( char *s, int n )
{
	char c;

	while(--n > 0) {
		if((c = getchar()) == '\n')
			break;
		else
			*s++ = (c == '\t') ? ' ' : c;
	}

	*s = '\0';
	return 0;
}
 

/*
** interpret_words 
*/
int interpret_words( char *s )
{
	int i;
	int status = INTERPRETER;

	switch( what_is(s) ) {

	case 2:
	case 4:
		push( atoi(s) );
		break;
		
	case 5:
		status = COMPILER;
		break;
	
	case 6:
		status = COMMENT;
		break;
	
	case 0:
	case 3: 	
		for (i = 0; i <= CODEWORDS_NUM; i++) {
			if( i == CODEWORDS_NUM ) {
				printf("Undefine word!\n-->%s<--\n", s);
				clean_stack();
				break;
			}
			else if( !strcmp(word_str[i], s) ) {
				word_pointer[i]();
				break;
			}
		}
		break;
	}
				
	return status;
}


/*
** what_is 
*/
char edges[ ][5]  = {
					/*   num  -   :  （ */
	/* state 0 */	{ 0,  0,  0,  0,  0 },
	/* state 1 */	{ 0,  2,  3,  5,  6 },
	/* state 2 */	{ 0,  2,  0,  0,  0 },
	/* state 3 */	{ 0,  4,  0,  0,  0 },
	/* state 4 */	{ 0,  4,  0,  0,  0 },
	/* state 5 */	{ 0,  0,  0,  0,  0 },
	/* state 6 */	{ 0,  0,  0,  0,  0 }
};

int what_is( char* s )
{
	int state, i;
	for(state = 1; *s != '\0'; s++) {
		
		if( *s >= '0' && *s <= '9' ) 
			i = 1;
		else if( *s == '-' )
			i = 2;
		else if( *s == ':' )
			i = 3;
		else if( *s == '(' )
			i = 4;
		else
			i = 0;
			
		state = edges[state][i];
	}
	
	return state;
}


/*
** compiler_words
*/
int compiler_words( char *s )
{

	if( if_name == 1 && !strcmp(";", s) ) { 
		return INTERPRETER;
	}

	if(if_name == 1) {
		strcpy(name, s);
		strcpy(defin, "\0");
		if_name = 0;
	}
	else if ( if_name == 0 && !strcmp(";", s) ) {
		add_word(name, defin);
		if_name = 1;
		return INTERPRETER; 
	}
	else {
		strcat(defin, s);
		strcat(defin, " ");
	}
	
	return COMPILER;
} 

/*
** ignore_words 
*/
int ignore_words( char *s )
{
	int status = COMMENT;
	if( !strcmp(")", s) ) 
		status = INTERPRETER;
	return status;
} 

