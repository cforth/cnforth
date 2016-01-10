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
    word->link = dict->head;
    dict->head = word;
    dict->size++;
    
    return 0;
}


Word *dict_search_name(Dict *dict, char *name)
{
    Word *w = dict->head;
    while (w != NULL && strcmp(w->name,name))
    {  
        w=w->link;
    }
    
    return w;
}


void destroy_word(Word *word)
{
    free(word->name);
    free(word->wplist);
    free(word);
}

int dict_rem_after(Dict *dict, char *name)
{
    Word *w = dict->head;
    Word *del_w;
    while (w != NULL && strcmp(w->name,name))
    {   
        w=w->link;
    }
    
    if(w->wplist == NULL)
    {
        printf("\tCore Word can't be deleted\n");
        return 0;
    }
    
    if(w != NULL)
    {
        do
        {
            del_w = dict->head;
            dict->head = dict->head->link;
            destroy_word(del_w);
            dict->size--;
        } while(del_w != w);
        
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


void dolist() //用于创建扩展词中的定义
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


void docons() //处理常数词
{
    ds_push((*IP)->num);
}


Word *constant(char *name, CELL num)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=docons;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
    
    w->num = num;
    
    return w;
}


void dovar()  //处理变量词和数组
{
    ds_push((CELL)*IP);
}


Word *variable(char *name, CELL num)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dovar;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
       
    w->num = num;
    
    return w;
}


//指令列表执行
void explain()
{
    Word  **IP_end = IP;
    IP=IP_head;
    
    while(IP != IP_end)
    {
        PRINT("[DEBUG]解释执行> %s\n", (*IP)->name)
        
        (*IP)->fn();
        ++IP;
    }
}


//判断字符串是否为数字
int is_num(char *s)
{
    if(*s == '-')
        s++;

    while (*s != 0)
    {
        if (!isdigit((CELL)*s)) 
            return 0;
        s++;
    }
    return 1;
}


//根据Forth代码中的当前词的名字，去执行相应的IP列表操作
int find(char *name, Dict *dict)
{
    Word *word_p;
    word_p = dict_search_name(dict, name);
    
    if(word_p==NULL)    //词典链表搜索不到名字后，去判断是不是数字
    {
        if (!is_num(name))    
        {
            return 0;    //如果不是数字，返回0
        }
        else 
        {               //如果是数字
            PRINT("[DEBUG]成功找到数字%s\n",name)
            ip_push(dict_search_name(dict, "push"));   //将push核心词指针存入IP数组        
            ip_push((Word*)(CELL)(atoi(name)));    //将CELL型数强制转换为Word指针类型

            return 1;
        }            
    }
    else 
    {
        ip_push(word_p);
    }
    
    PRINT("[DEBUG]成功找到%s词\n",name)
    return 1;
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


void ip_push(Word *w)
{
    if(IP >= IP_list+BUFF_LEN){stack_error(2); exit(0);}
    *IP=w;
    IP++;
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
        w=w->link;
    }
    printf("\n");
}


//Forth立即词
void defcolon()
{
    define_p = colon(next_word);
}


void endcolon()
{
    ip_push(dict_search_name(forth_dict, "ret"));
    int n = (CELL)IP - (CELL)IP_head;
    dict_ins_next(forth_dict, define_p);
    change_colon(define_p, IP_head, n);
    IP_head = IP;
}


void _if()
{
    ip_push(dict_search_name(forth_dict, "?branch"));
    rs_push((CELL)IP);
    ip_push((Word *)0);
}


void _else()
{
    ip_push(dict_search_name(forth_dict, "branch"));
    Word** else_p = IP;
    Word** if_p = (Word**)(rs_pop());
    rs_push((CELL)else_p);
    *if_p = (Word*)(IP - if_p + 1);
    ip_push((Word *)0);
}


void _then()
{
    Word** branch_p = (Word**)(rs_pop());
    *branch_p = (Word*)(IP - branch_p); 
}


void _do()
{
    ip_push(dict_search_name(forth_dict, "(do)"));
    rs_push((CELL)IP);
    ip_push((Word *)0);
    
}


void _loop()
{
    ip_push(dict_search_name(forth_dict, "(loop)")); 
    Word** do_p = (Word**)(rs_pop());
    *do_p = (Word*)(IP - do_p + 1); 
    ip_push((Word*)(IP - do_p + 1)); 
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
        if(word_p->fn == dolist)
        {
            Word **p = word_p->wplist;
            Word *end = dict_search_name(forth_dict, "ret");
            Word *dict_p = forth_dict->head;
            for(; *p != end; p++)
            {
                while (dict_p != NULL && dict_p != *p)
                {  
                    dict_p=dict_p->link;
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
    dict_rem_after(forth_dict, next_word); //删除当前扩展词以及词典中该词之后定义的所有扩展词
}


void var()
{
    dict_ins_next(forth_dict, variable(next_word, 0));
}


void cons()
{
    dict_ins_next(forth_dict, constant(next_word, ds_pop()));
}


void myself()
{
    ip_push((Word *)define_p);
}