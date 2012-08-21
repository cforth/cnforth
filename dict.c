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

typedef struct DICT_NODE {
	char name[20];
	char defin[100];
	struct DICT_NODE *next;
} DictNode;

static DictNode *dict = NULL;

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

void printf_dict( void )
{
	DictNode *p = dict;
	while(p != NULL) {
		printf("name:%s\ndefin:%s\n\n",p->name,p->defin);
		p = p->next;
	}
	return;
}

int main()
{
	char tmp[20];
	add_word("1", "111");
	add_word("2", "ca adsa asd");
	add_word("3", "333");
	strcpy(tmp, find_word("1"));
	printf("%s\n",tmp);
	strcpy(tmp, find_word("3"));
	printf("%s\n",tmp);
	strcpy(tmp, find_word("2"));
	printf("%s\n",tmp);
	if(find_word("4") == NULL)
		printf("error\n");
	printf_dict();
	return 0;
}
