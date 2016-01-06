#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "words.h"


Dict *dict_init()
{
    Dict *dict=(Dict*)malloc(sizeof(Dict));
    dict->size = 0;
    dict->head = NULL;
    
    return dict;
}


int dict_ins_next(Dict *dict, Word *word)
{
    word->next = dict->head;
    dict->head = word;
    dict->size++;
    
    return 0;
}


Word *dict_search_name(Dict *dict, char *name)
{
    Word *w = dict->head;
    while (w != NULL && strcmp(w->name,name))
    {  
        w=w->next;
    }
    
    return w;
}


void destroy_word(Word *word)
{
    free(word->name);
    free(word->wplist);
}

int dict_rem_name(Dict *dict, char *name)
{
    Word *w = dict->head;
    Word *w_before;
    Word *w_after;
    while (w != NULL && strcmp(w->name,name))
    {   
        w_before = w;
        w=w->next;
        w_after = (w != NULL) ? w->next : NULL;
    }
    
    if(w->wplist == NULL)
    {
        printf("\tCore Word can't be deleted\n");
        return 0;
    }
    
    if(w != NULL && w != dict->head)
    {
        w_before->next = w_after;
        destroy_word(w);
        free(w);
        dict->size--;
        return 1;
    }
    else if(w == dict->head)
    {
        w_after = (w != NULL) ? w->next : NULL;
        dict->head = w_after;
        destroy_word(w);
        free(w);
        dict->size--;
        return 1;
    }
    return 0;
}


Word *code(char *name, fnP  fp)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=fp;
    w->wplist=NULL;   
    w->name=name;

    return w;
}


void dolist()
{
    RP++;
    *RP=(CELL)IP;
    IP=(*IP)->wplist-1;
    PRINT("[DEBUG]进入子例程\n")
}


void change_colon(Word *c, Word **list, int n)
{
    c->wplist = (Word**)malloc(n);
    memcpy(c->wplist,list, n);
}


Word *colon(char *name)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dolist;

    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);

    w->wplist=NULL;
    
    return w;
}


Word *constant(char *name, Word **list)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dolist;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
    
    w->wplist=(Word**)malloc(sizeof(CELL)*3);
    memcpy(w->wplist,list, sizeof(CELL)*3);
    
    return w;
}


Word *variable(char *name, Word **list, CELL num)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dolist;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
    
    w->wplist=(Word**)malloc(sizeof(CELL)*3);
    memcpy(w->wplist,list, sizeof(CELL)*3);
       
    w->num = num;
    
    return w;
}


//Forth栈操作函数
void empty_stack()
{
    DP=DS-1;
    RP=RS-1;
}


void stack_error(int n)
{
    switch(n)
    {
        case 1: printf("Stack underflow\n"); break;
        case 2: printf("Stack overflow\n"); break;
    }
}


void ds_push(CELL n)
{
    if(DP >= DS+STACK_LEN-1){stack_error(2); exit(0);}
    DP++;
    *DP=n;
}


void rs_push(CELL n)
{
    if(RP >= RS+STACK_LEN-1){stack_error(2); exit(0);}
    RP++;
    *RP=n;
}


CELL ds_pop()
{
    if(DP <= DS-1){stack_error(1); exit(0);}
    DP--;
    return *(DP+1); 
}


CELL rs_pop()
{
    if(RP <= RS-1){stack_error(1); exit(0);}
    RP--;
    return *(RP+1); 
}


CELL ds_top()
{
    if(DP <= DS-1){stack_error(1); exit(0);}
    return *DP;
}


CELL rs_top()
{
    if(RP <= RS-1){stack_error(1); exit(0);}
    return *RP;
}


//Forth核心词
void push()
{
    IP++;
    ds_push((CELL)*IP);
    PRINT("[DEBUG]数%ld压入数据栈\n", (CELL)*IP)
}


void popds()
{
    printf("%ld\n", ds_pop());
}


void bye()
{
    exit(1);
}


void ret()
{
    IP=(Word**)(rs_pop());
    PRINT("[DEBUG]从子例程返回\n")
}


void depth()
{
    ds_push((CELL)(DP-DS+1));
}


void add()
{
    ds_push(ds_pop() + ds_pop());
}


void sub()
{
    CELL d = ds_pop();
    ds_push(ds_pop() - d);
}


void mul()
{
    ds_push(ds_pop() * ds_pop());
}


void divv()
{
    CELL d = ds_pop();
    ds_push(ds_pop() / d);
}


void drop()
{
    ds_pop();
}


void showds()
{
    printf("<%ld> ", (CELL)(DP-DS+1));
    CELL *i=DS;
    for (;i<=DP ;i++ )
    {
        printf("%ld ",*i);
    }
    printf("\n");
}


void pick()
{
    CELL k = ds_pop();
    if(DP-k+1 <= DS-1){stack_error(1); exit(0);}
    ds_push(*(DP-k+1));
}


void roll()
{
    CELL k = ds_pop();
    if(DP-k+1 <= DS-1){stack_error(1); exit(0);}
    CELL dk = *(DP-k+1);
    for(; k>1; k--) {
        *(DP-k+1) = *(DP-k+2);
    }
    ds_pop();
    ds_push(dk);
}


void invar()
{
    Word *p = (Word *)(ds_pop());
    p->num = ds_pop();
}


void outvar() 
{
    Word *p = (Word *)(ds_pop());
    ds_push(p->num);
}


void equal()
{
    if(ds_pop() == ds_pop())
    {
        ds_push(-1);
    }
    else
    {
        ds_push(0);
    }
}


void noequal()
{
    if(ds_pop() != ds_pop())
    {
        ds_push(-1);
    }
    else
    {
        ds_push(0);
    }
}


void morethan()
{
    CELL d = ds_pop();
    if(ds_pop() > d)
    {
        ds_push(-1);
    }
    else
    {
        ds_push(0);
    }
}


void lessthan()
{
    CELL d = ds_pop();
    if(ds_pop() < d)
    {
        ds_push(-1);
    }
    else
    {
        ds_push(0);
    }
}


void if_branch()
{
    if(ds_pop() == 0)
    {
        IP = IP + (CELL)(*(IP+1));
    }
    else
    {
        IP++;
    }
}


void branch()
{
    IP = IP + (CELL)(*(IP+1));
}


void __do()
{
    CELL index = ds_pop();
    CELL limit = ds_pop();
    if(limit <= index)
    {
        IP = IP + (CELL)(*(IP+1)); 
    }
    else
    {
        IP++;
        index++;
        rs_push(index);
        rs_push(limit);
    }
}


void __loop() 
{
    IP = IP - (CELL)(*(IP+1)); 
    ds_push(rs_pop());
    ds_push(rs_pop());
}


void tor()
{
    rs_push(ds_pop());
}


void rto()
{
    ds_push(rs_pop());
}


void rat()
{
    ds_push(rs_top());
}


void emit()
{
    putchar((char)(ds_pop()));
}


void words()
{
    Word *w = forth_dict->head;
    while (w != NULL)
    {  
        printf("%s ", w->name);
        w=w->next;
    }
    printf("\n");
}


//Forth立即词
void _if()
{
    *IP = dict_search_name(forth_dict, "?branch");
    IP++;
    rs_push((CELL)IP);
    IP++;
}


void _else()
{
    *IP = dict_search_name(forth_dict, "branch");
    IP++;
    Word** else_p = IP;
    Word** if_p = (Word**)(rs_pop());
    rs_push((CELL)else_p);
    *if_p = (Word*)(IP - if_p + 1);
    IP++;
}


void _then()
{
    Word** branch_p = (Word**)(rs_pop());
    *branch_p = (Word*)(IP - branch_p); 
}


void _do()
{
    *IP = dict_search_name(forth_dict, "(do)");
    IP++;
    rs_push((CELL)IP);
    IP++;
    
}


void _loop()
{
    *IP = dict_search_name(forth_dict, "(loop)"); 
    IP++;
    Word** do_p = (Word**)(rs_pop());
    *do_p = (Word*)(IP - do_p + 1); 
    *IP = (Word*)(IP - do_p + 1); 
    IP++;
}


void see()
{
    Word *word_p = dict_search_name(forth_dict, next_word);
    
    if(word_p == NULL)
    {
        printf("%s :\n\tCan't find!\n", next_word);
    }
    else
    {   //反编译wplist，得出扩展词的字符串定义
        printf("%s :\n\t", next_word);
        if(word_p->wplist != NULL)
        {
            Word **p = word_p->wplist;
            Word *end = dict_search_name(forth_dict, ";");
            Word *dict_p = forth_dict->head;
            for(; *p != end; p++)
            {
                while (dict_p != NULL && dict_p != *p)
                {  
                    dict_p=dict_p->next;
                }

                if(dict_p != NULL)
                    printf("%s ", (*p)->name);
                else
                    printf("%ld ", (CELL)(*p));
                dict_p = forth_dict->head;
            }
            printf(";\n");
        }
        else
        {
            printf("%s\n", word_p->name);
        }
    }
}


void forget()
{
    dict_rem_name(forth_dict, next_word);
}


void var()
{
    Word *variable_IP_list[3] = {NULL, NULL, NULL};
    dict_ins_next(forth_dict, variable(next_word, variable_IP_list, 0));
    Word * v = dict_search_name(forth_dict, next_word);
    variable_IP_list[0] = dict_search_name(forth_dict, "push");
    variable_IP_list[1] = v;
    variable_IP_list[2] = dict_search_name(forth_dict, ";");
    change_colon(v, variable_IP_list, sizeof(CELL)*3);
}


void cons()
{
    Word *constant_IP_list[3];
    constant_IP_list[0] = dict_search_name(forth_dict, "push");
    constant_IP_list[1] = (Word *)(ds_pop());
    constant_IP_list[2] = dict_search_name(forth_dict, ";");
    dict_ins_next(forth_dict, constant(next_word, constant_IP_list));
}


void myself()
{
    *IP = (Word *)define_p;
    IP++;
}