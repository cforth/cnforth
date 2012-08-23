/*
** str_lex.c
** cforth输入流词法分析模块
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "str_lex.h"

/*
** 初始化str_now指针和str_prev指针。
** str_now用于指向当前队列尾部节点。
** str_prev用于新增节点到队列时，保存临时节点信息。
**
*/
static StrNode *str_prev = NULL;
static StrNode *str_now = NULL;


/*
** create_str
*/
StrNode *create_str( void ) {
	StrNode *p;

	p = malloc(sizeof(StrNode));
	assert(p != NULL);
	p->index = 0;
	p->next = NULL;
	str_prev = p;
	return p;
};


/*
** add_str
*/
void add_str( int t, int l, char *s )
{
	StrNode *new;

	new = malloc(sizeof(StrNode));
	assert(new != NULL);
	new->index = (str_prev->index) + 1;
	new->type = t;
	new->length = l;
	strcpy(new->str, s);
	str_now = new;
	str_now->next = NULL;
	str_prev->next = str_now;
	str_prev = str_now;
	return;
}


/*
** printf_str
*/
void printf_str( StrNode *head )
{
	StrNode *p = head->next;
	while(p != NULL) {
		printf("index:%d\ntype:%d\nlength:%d\nstring:%s\n\n",p->index,p->type,p->length,p->str);
		p = p->next;
	}
	return;
}


/*
** destroy_str
*/
void destroy_str( StrNode *head )
{
	str_prev = head;
	StrNode *p = head->next;
	while(p != NULL) {
		head = p->next;
		free(p);
		p = head;
	}
	str_prev->next = NULL;
	str_now = NULL;
	return;
}


/*
** 队列测试用例。
*/
int main()
{
	StrNode *str_head;
	str_head = create_str();
	int i = 10;
	while(i--) {
		add_str(1,2,"cf");
		add_str(1,5,"hello");
		add_str(3,3,"811");
		printf_str(str_head);
		destroy_str(str_head);
		printf_str(str_head);
	}
	printf_str(str_head);
	return 0;
}
