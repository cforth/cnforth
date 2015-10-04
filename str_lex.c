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
    p->next = NULL;
    str_prev = p;
    return p;
};


/*
** add_str
*/
void add_str( int t, char *s )
{
    StrNode *new;

    new = malloc(sizeof(StrNode));
    assert(new != NULL);
    new->type = t;
    new->length = strlen(s);
    strcpy(new->str, s);
    str_now = new;
    str_now->next = NULL;
    str_prev->next = str_now;
    str_prev = str_now;
    return;
}


/*
** del_str
*/
void del_str( StrNode *head, char *s )
{
    StrNode *prov = head;
    StrNode *now = head->next;
    while(now != NULL) {
        if(!strcmp(now->str, s)) {
            now = now->next;
            free(prov->next);
            prov->next = now;
        }
        else {
            prov = now;
            now = now->next;
        }
    }
    return;
}


/*
** printf_str
*/
void printf_str( StrNode *head )
{
    StrNode *p = head->next;
    while(p != NULL) {
        printf("type:%d\nlength:%d\nstring:%s\n\n",p->type,p->length,p->str);
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
        
    while(1) {
        add_str(1,"cf");
        add_str(1,"hello");
        add_str(0,"811");
        add_str(1,"hello");
        add_str(0,"811");
        add_str(1,"tail");
    
        del_str(str_head, "hello");
        del_str(str_head, "811");
        del_str(str_head, "cf");
    
        destroy_str(str_head);
    }
    return 0;
}
