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
    *RP=(int)IP;
    IP=(*IP)->wplist-1;
    if (DEBUG) printf("[DEBUG]进入dolist\n");
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

Word *variable(char*s, int num, Word *dict)
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


//STACK OPERATE 
void bye()     {exit(0);}//结束程序
void push(){IP++;DP++;*DP=(int)*IP;}
void dup(){DP++;*DP=*(DP-1);if (DEBUG) printf("[DEBUG]进入dup\n");}
void swap(){int t=*DP; *DP=*(DP-1); *(DP-1)=t;}
void over(){*(DP+1)=*(DP-1);DP++;}
void drop(){DP--;}
//RS
void tor()    {RP++;*RP=*DP;DP--;}
void rto()    {DP++;*DP=*RP;RP--;}
void rat()    {DP++;*DP=*RP;}
//TS
void tot()    {TP++;*TP=*DP;DP--;}
void tto()    {DP++;*DP=*TP;TP--;}
void tat()    {DP++;*DP=*TP;}
//加减乘除
void add(){ *(DP-1)=*(DP-1)+(*DP); DP--;}
void sub(){ *(DP-1)=*(DP-1)-(*DP); DP--;}
void mul(){ *(DP-1)=*(DP-1)*(*DP); DP--;}
void divv(){ *(DP-1)=*(DP-1)/(*DP); DP--;}

//;
void ret()    {IP=(Word**)*RP;    RP--;if (DEBUG) printf("[DEBUG]进入ret\n");}

//if else then语句
void iff() {if(*DP==0){IP = IP + (int)(*(IP+1));}else{IP++;} DP--;}
void elsee() {IP = IP + (int)(*(IP+1));}
void then() {;}


//for next 循环
void forr() {if(*DP<1){IP = IP + (int)(*(IP+1)); DP--;}else{IP++; (*DP)--; tot();}}
void next() {IP = IP - (int)(*(IP+1)); tto();}

//变量存入和取出
void invar() {((Word*)*DP)->num = *(DP-1);DP-=2;}
void outvar() {*DP = ((Word*)*DP)->num;}