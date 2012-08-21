/*
** dict.c
** cforth字典实现
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "dict.h"


/*
** 使用链表实现字典结构
*/
typedef struct DICT_NODE {
	char name[20];
	char defin[100];
	struct DICT_NODE *next;
} DictNode;


/*
** 设置字典链表的初始节点指向NULL。
*/
static DictNode *dict = NULL;


/*
** add_word
*/
void add_word( char *n, char *d )
{
	DictNode *new_node;

	new_node = malloc(sizeof(DictNode));
	assert(new_node != NULL);
	strcpy(new_node->name, n);
	strcpy(new_node->defin, d);
	new_node->next = dict;
	dict = new_node;
	return;
}


/*
** find_word
*/
char *find_word(char *n)
{
	DictNode *p = dict;
	while(p != NULL) {
		if (!strcmp(p->name, n))
			return p->defin;
		p = p->next; 
	}
	return NULL;
}


/*
** printf_dict
*/
void printf_dict( void )
{
	DictNode *p = dict;
	while(p != NULL) {
		printf("name:%s\ndefin:%s\n\n",p->name,p->defin);
		p = p->next;
	}
	return;
}


