/*
** Cforth_alpha 0.3.3
** cforth.c	Cforth主程序
** 作者：	ear & xiaohao
** 版本号：	alpha 0.3.3
** 更新时间：	2012-07-10
*/
	 
#include "cforth.h"

#define COMPILER	0	/* 设置编译模式 */
#define INTERPRETER	1	/* 设置解释模式 */ 
#define MAX_LENGTH	1000	/* 设置字符串处理的最大长度 */
#define CODEWORDS_NUM	17	/* 设置核心字的最大数量 */ 
#define WORD_WIDTH	20	/* 设置单个核心字名字的最大宽度 */

/*
** word_str
** 核心字索引数组，用于解释模式下核心字的搜索执行。 
*/
const char word_str[ CODEWORDS_NUM ][ WORD_WIDTH ] = 
{	".s",	".rs",	".",	"swap",	">r",
	"r>",	"dup",	"drop",	"over",	"+",
	"-",	"*",	"/",	"%",	"--",
	"++",	"rot"	};

/*
** word_pointer
** 函数指针数组，用于解释模式下核心字的搜索执行。 
*/		
typedef void ( *pType ) ( void ) ;
pType word_pointer[ CODEWORDS_NUM ] = 
{	sh_ds,	sh_rs,	pop,	swap,	tor,
	rto,	dup,	drop,	over,	add,
	sub,	mul,	div_new,mod,	sub1,
	add1,	rot	}; 


/* 
** Cforth主控制结构，目前只有解释器功能。 
*/  
int main( void )
{
	printf("Cforth_alpha 0.3.3\n"); 

	char *token;
	int status = INTERPRETER;
	char input_buff[ MAX_LENGTH ];
	
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


/*
** gets_input 
*/
int gets_input( char *str )
{
	char c;
	for(; (c = getchar()) != '\n'; str++)
		*str = c;
	*str = '\0';
	return 0;
}
 

/*
** interpret_words 
*/
int interpret_words( char *str )
{
	int i;
	int status = INTERPRETER;
	for (i = 0; i <= CODEWORDS_NUM; i++) {
		if( isNum(str) ) {
			push( atoi(str) );
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
			
		else if(i == CODEWORDS_NUM) {
			printf("Undefine word!\n-->%s<--\n", str);
			clean_stack();
			break;
		}
	}
	
	return status;
}


/*
** isNum 
*/
char edges[ ][12] = {	/*   1  2  3  4  5  6  7  8  9  0  - */
	/* state 0 */	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* state 1 */	{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 },
	/* state 2 */	{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 },
	/* state 3 */	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0 },
	/* state 4 */	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0 }
};

int isNum( char* str )
{
	int state, i;
	for(state=1; *str != '\0'; str++) {
		
		switch( *str ) {
			case '1': i = 1; break;
			case '2': i = 2; break;
			case '3': i = 3; break;
			case '4': i = 4; break;
			case '5': i = 5; break;
			case '6': i = 6; break;
			case '7': i = 7; break;
			case '8': i = 8; break;
			case '9': i = 9; break;
			case '0': i = 10; break;
			case '-': i = 11; break;
			default : return 0;
		}
		
		state = edges[state][i];
	}
	
	if(state == 2 || state == 4)
		return 1;
	return 0;
}


/*
** compiler_words
** 编译器模式，预留了位置，代码未完成。 
*/
int compiler_words( char *str )
{
	int status = COMPILER;
	if( !strcmp(";", str) ) 
		status = INTERPRETER;
	return status;
} 
