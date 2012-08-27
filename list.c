#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>


typedef struct List List;
struct List {
	char name[20];
	char type;
	List *next;
};


List *newitem(char *name, char type)
{
	List *newp;

	newp = (List *) malloc(sizeof(List));
	assert(newp != NULL);
	strcpy(newp->name, name);
	newp->type = type;
	newp->next = NULL;
	return newp;
}


List *addfront(List *listp, List *newp)
{
	newp->next = listp;
	return newp;
}


List *addend(List *listp, List *newp)
{
	List *p;

	if (listp == NULL)
		return newp;
	for (p = listp; p->next != NULL; p = p->next)
		;
	p->next = newp;
	return listp;
}


List *lookup(List *listp, char *name)
{
	for (; listp != NULL; listp =listp->next)
		if(strcmp(name, listp->name) == 0)
			return listp;
	return NULL;
}


void apply(List *listp, void (*fn)(List*, void*), void *arg)
{
	for (; listp != NULL; listp = listp->next)
		(*fn)(listp, arg);
}


void printnt(List *p, void *arg)
{
	char *fmt;

	fmt = (char *) arg;
	printf(fmt, p->name, p->type);
}


void inccounter(List *p, void *arg)
{
	int *ip;

	ip = (int *) arg;
	(*ip)++;
}


int main()
{
	List *list;
	int n = 0;
	list = newitem("12", 'n');
	list = addfront(list, newitem("34", 'n'));
	list = addend(list, newitem("+", 'e'));
	apply(list, printnt, "name:%s\ntype:%c\n\n");
	apply(list, inccounter, &n);
	printf("%d elements in list\n",n);
	return 0;
}
