#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

typedef struct STR_NODE {
	int t;
	int l;
	char str[20];
	struct STR_NODE *next;
} StrNode;


static StrNode *str_p = NULL;
static StrNode *str_tail = NULL;

StrNode *create_str( void ) {
	StrNode *p;

	p = malloc(sizeof(StrNode));
	assert(p != NULL);
	p->t = 0;
	p->l = -1;
	p->next = NULL;
	return p;
};

void add_str( int t, int l, char *s )
{
	StrNode *new_node;

	new_node = malloc(sizeof(StrNode));
	assert(new_node != NULL);
	new_node->t = t;
	new_node->l = l;
	strcpy(new_node->str, s);
	str_tail = new_node;
	str_tail->next = NULL;
	str_p->next = str_tail;
	str_p = str_tail;
	return;
}

void printf_str( StrNode *str_head )
{
	StrNode *p = str_head;
	while(p->next != NULL) {
		printf("type:%d\nlength:%d\nstring:%s\n\n",(p->next)->t,(p->next)->l,(p->next)->str);
		p = p->next;
	}
}

void destroy_str( StrNode *str_d )
{
	StrNode *p = str_d->next;
	str_p = str_d;
	while(p != NULL) {
		str_d = p->next;
		free(p);
		p = str_d;
	}
	str_tail = NULL;
	return;
}

int main()
{
	StrNode *str_head;
	str_head = create_str();
	str_p = str_head;
	add_str(1,2,"cf");
	add_str(2,5,"hello");
	add_str(3,3,"cff");
	printf_str(str_head);
	destroy_str(str_head);
	printf_str(str_head);
	add_str(1,2,"cf");
	printf_str(str_head);
	return 0;
}
