#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "words.h"


Word *code(char*s, fnP  fp, Word *dict)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->next=dict;
    dict=w;
    w->fn=fp;
    w->wplist=NULL;

    w->name=s;

    return w;
}


void dolist()
{
    RP++;
    *RP=(CELL)IP;
    IP=(*IP)->wplist-1;
    PRINT("[DEBUG]进入dolist\n");
}


Word *colon(char*s, Word **list, int n, Word *dict)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->next=dict;
    dict=w;
    w->fn=dolist;

    w->name=(char*)malloc(strlen(s)+1);
    strcpy(w->name,s);

    w->wplist=(Word**)malloc(n);
    memcpy(w->wplist,list, n);
    
    return w;
}


Word *variable(char*s, CELL num, Word *dict)
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->next=dict;
    dict=w;
    w->fn=NULL;
    
    w->name=(char*)malloc(strlen(s)+1);
    strcpy(w->name,s);
       
    w->num = num;
    
    return w;
}


void bye()
{
    exit(0);
}


void push()
{
    IP++;
    DP++;
    *DP=(CELL)*IP;
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


void tot()
{
    TP++;
    *TP=*DP;
    DP--;
}


void tto()
{
    DP++;
    *DP=*TP;
    TP--;
}


void tat()
{
    DP++;
    *DP=*TP;
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


void ret()
{
    IP=(Word**)*RP;
    RP--;
}


void iff()
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


void elsee()
{
    IP = IP + (CELL)(*(IP+1));
}


void then()
{
    ;
}


void forr()
{
    if(*DP<1)
    {
        IP = IP + (CELL)(*(IP+1)); 
        DP--;
    }
    else
    {
        IP++; 
        (*DP)--; 
        tot();
    }
}


void next() 
{
    IP = IP - (CELL)(*(IP+1)); 
    tto();
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