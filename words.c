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
    free(word->str);
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


Word *code(char*name, fnP  fp)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=fp;
    w->wplist=NULL;   
    w->name=name;
    
    char *str = "( Core Word )";
    w->str=(char*)malloc(strlen(str)+1);
    strcpy(w->str,str);

    return w;
}


void dolist()
{
    RP++;
    *RP=(CELL)IP;
    IP=(*IP)->wplist-1;
    PRINT("[DEBUG]进入dolist\n")
}


void change_colon(Word *c, Word **list, int n)
{
    free(c->wplist);
    c->wplist = (Word**)malloc(n);
    memcpy(c->wplist,list, n);
}


Word *colon(char*name, char*str, Word **list, int n)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dolist;

    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);

    w->wplist=(Word**)malloc(n);
    memcpy(w->wplist,list, n);
    
    w->str=(char*)malloc(strlen(str)+1);
    strcpy(w->str,str);
    
    return w;
}


Word *constant(char*name, Word **list)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=dolist;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
    
    w->wplist=(Word**)malloc(sizeof(CELL)*3);
    memcpy(w->wplist,list, sizeof(CELL)*3);
    
    char *str = "( Constant )";
    w->str=(char*)malloc(strlen(str)+1);
    strcpy(w->str,str);
    
    return w;
}


Word *variable(char*name, CELL num)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->fn=NULL;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
       
    w->num = num;
    
    char *str = "( Variable )";
    w->str=(char*)malloc(strlen(str)+1);
    strcpy(w->str,str);
    
    return w;
}


void empty_stack()
{
    DP=DS-1;
    //*DP=0;
    RP=RS-1;
    //*RP=0;
}

//Forth栈操作词
void ds_push(CELL n)
{
    DP++;
    *DP=n;
}


void rs_push(CELL n)
{
    RP++;
    *RP=n;
}


CELL ds_pop()
{
    DP--;
    return *(DP+1); 
}


CELL rs_pop()
{
    RP--;
    return *(RP+1); 
}


//Forth核心词
void push()
{
    IP++;
    DP++;
    *DP=(CELL)*IP;
}


void popds()
{
    printf("%ld\n", *DP);
    DP--;
}


void bye()
{
    exit(0);
}


void ret()
{
    IP=(Word**)*RP;
    RP--;
}


void putcr()
{
    putchar('\n');
}


void depth()
{
    CELL d = DP - DS + 1;
    DP++;
    *DP = d; 
}


void add()
{
    *(DP-1)=*(DP-1)+(*DP);
    DP--;
}


void sub()
{
    *(DP-1)=*(DP-1)-(*DP);
    DP--;
}


void mul()
{
    *(DP-1)=*(DP-1)*(*DP);
    DP--;
}


void divv()
{
    *(DP-1)=*(DP-1)/(*DP);
    DP--;
}


void dup()
{
    DP++;
    *DP=*(DP-1);
}


void swap()
{
    CELL t=*DP;
    *DP=*(DP-1);
    *(DP-1)=t;
}


void over()
{
    *(DP+1)=*(DP-1);DP++;
}


void drop()
{
    DP--;
}


void showds()
{
    printf("DS> ");
    CELL *i=DS;
    for (;i<=DP ;i++ )
    {
        printf("%ld ",*i);
    }
    printf("\n");
}


void pick()
{
    CELL k = *DP;
    *DP = *(DP-k);
}


void roll()
{
    CELL k = *DP;
    CELL dk = *(DP-k);
    for(; k>1; k--) {
        *(DP-k) = *(DP-k+1);
    }
    DP--;
    *DP = dk;
}


void invar()
{
    ((Word*)*DP)->num = *(DP-1);
    DP-=2;
}


void outvar() 
{
    *DP = ((Word*)*DP)->num;
}


void equal()
{
    if(*(DP-1) == *DP)
    {
        DP--;
        *DP = -1;
    }
    else{
        DP--;
        *DP = 0;
    }
}


void noequal()
{
    if(*(DP-1) != *DP)
    {
        DP--;
        *DP = -1;
    }
    else{
        DP--;
        *DP = 0;
    }
}


void morethan()
{
    if(*(DP-1) > *DP)
    {
        DP--;
        *DP = -1;
    }
    else{
        DP--;
        *DP = 0;
    }
}


void lessthan()
{
    if(*(DP-1) < *DP)
    {
        DP--;
        *DP = -1;
    }
    else{
        DP--;
        *DP = 0;
    }
}


void if_branch()
{
    if(*DP==0)
    {
        IP = IP + (CELL)(*(IP+1));
    }
    else
    {
        IP++;
    }
    DP--;
}


void branch()
{
    IP = IP + (CELL)(*(IP+1));
}


void __do()
{
    if(*(DP-1) <= *DP)
    {
        IP = IP + (CELL)(*(IP+1)); 
        DP--;
        DP--;
    }
    else
    {
        IP++;
        (*DP)++;
        tor();
        tor();
    }
}


void __loop() 
{
    IP = IP - (CELL)(*(IP+1)); 
    rto();
    rto();
}


void tor()
{
    RP++;
    *RP=*DP;
    DP--;
}


void rto()
{
    DP++;
    *DP=*RP;
    RP--;
}


void rat()
{
    DP++;
    *DP=*RP;
}


void emit()
{
    putchar((char)(*DP));
    DP--;
}


void myself()
{
    ;
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
    {
        printf("%s :\n\t%s ;\n", word_p->name, word_p->str);
    }
}


void forget()
{
    dict_rem_name(forth_dict, next_word);
}


void var()
{
    dict_ins_next(forth_dict, variable(next_word, 0));
}


void cons()
{
    Word *constant_IP_list[3];
    constant_IP_list[0] = dict_search_name(forth_dict, "push");
    constant_IP_list[1] = (Word *)(ds_pop());
    constant_IP_list[2] = dict_search_name(forth_dict, "ret");
    dict_ins_next(forth_dict, constant(next_word, constant_IP_list));
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