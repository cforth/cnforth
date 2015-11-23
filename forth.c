#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "words.h"
#include "forth.h"

char cmdstr[BUFF_LEN];    //输入缓存区
Word *IP_list[BUFF_LEN/4];   //Word类型指针数组，长度为cmdstr_len/4
Word **IP_list_p=IP_list;        //Word类型指针，指向IP_list[0]

Word *dict_head; //字典入口指针
Word *pushh;   //push字指针

void showDS()
{
    printf("DS> ");
    int *i=DS;
    for (;i<=DP ;i++ )
    {
        printf("%d ",*i);
    }
    printf("\n\n");
}


int is_blankchar(char c)
{
    return (c==' ' || c=='\t' || c=='\n' );
}


char * ignore_blankchar(char *s)
{
    while (is_blankchar(*s))
        s++;
    return s;
}
char * until_Wordend(char *s)
{
    while ( !is_blankchar(*s)  && *s!='\0')
        s++;
    return s;
}

char * split_Word(char *s)
{
    s=until_Wordend(s);
    if (*s=='\0')
        return s;
    *s='\0';
    s++;
    return s;
}

int is_num(char *s)
{
    while (*s != 0)
    {
        if (!isdigit((int)*s)) return 0;
        s++;
    }
    return 1;
}

Word** if_p = NULL;
Word** else_p = NULL;
Word** for_p = NULL;

int find_Word(char *w, Word *dict)
{
    while (strcmp(dict->name,w))
    {  
        dict=dict->next;
       
        if(dict==NULL)    //字典链表搜索不到名字后执行
        {
            if (!is_num(w))    {
                    return 0; //如果不是数字，返回0
            }
            else {               //如果是数字
                if (DEBUG)    printf("[DEBUG]成功找到数字%s\n",w);
                *IP_list_p=pushh;      //将push核心字指针存入IP_list_p数组        
                IP_list_p++;        //数组指针指向下一个位置
                *IP_list_p=(Word*)atoi(w);    //将int型数强制转换为Word指针类型
                IP_list_p++;

                return 1;
            }            
        }
    }
 
    if(dict->fn == NULL)  //判断这个字是否是变量字！！
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=pushh;
        IP_list_p++;
        *IP_list_p=dict;
        IP_list_p++;
        
        return 1;
    }
        
    
    if(!strcmp("if",w))
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        IP_list_p++;
        if_p = IP_list_p;
        IP_list_p++;
    }
    else if(!strcmp("else",w)) 
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;  
        IP_list_p++;
        else_p = IP_list_p;
        *if_p = (Word*)(else_p - if_p + 1);  //+1的意思是越过else字和后面的then偏移量位置
        IP_list_p++;
    }
    else if(!strcmp("then",w))
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        if(else_p == NULL)
        {
            else_p = IP_list_p;
            *if_p = (Word*)(else_p - if_p + 1); 
        }
        else
        {
            *else_p = (Word*)(IP_list_p - else_p + 1);
        }
        
        IP_list_p++;
    }
    else if(!strcmp("for",w))
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        IP_list_p++;
        for_p = IP_list_p;
        IP_list_p++;
        
    }
    else if(!strcmp("next",w))
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;  
        IP_list_p++;
        *for_p = (Word*)(IP_list_p - for_p + 1); 
        *IP_list_p = (Word*)(IP_list_p - for_p + 1); 
        IP_list_p++;
    }
/*     else if(!strcmp("variable",w)) //未完成
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        (*IP_list_p)->
        
    } */
/*     else if(!strcmp("!",w)) //未完成
    {
    }
    else if(!strcmp("@",w)) //未完成
    {
    } */
    else 
    {
        if (DEBUG)    printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;    
        IP_list_p++;
    }
    
    return 1;
}

void init()
{
    DP=DS-1;
    //*DP=0;
    RP=RS-1;
    //*RP=0;
    TP=TS-1;
    //*TP=0;
    IP_list_p=IP_list;

}

void explain()
{
    IP=IP_list;
    
    while(IP != IP_list_p)
    {
        if (DEBUG) printf("[DEBUG]解释执行> %s\n", (*IP)->name);
        
        (*IP)->fn();
        ++IP;
    }
}

void compile(char *s)
{
    int n;
    s=ignore_blankchar( s);
    
    char *name=NULL;
    char *var_name=NULL;
    if (*s==':')
    {
        s++;
        name=ignore_blankchar(s);
        s=name;
        s=split_Word(s);
    }
    else if (*s=='$')  //变量定义方式 $ var_name 无法一行定义多个变量
    {
        s++;
        var_name=ignore_blankchar(s);
        s=var_name;
        s=split_Word(s);        
    }
    
    char *w;
    while (*s!=0)
    {
        s=ignore_blankchar(s);
        w=s;
        s=split_Word(s);

        if(!find_Word(w, dict_head) )
        {
            printf("\n[%s]?\n",w);
            init();
            return;
        }
    }
    //*IP_list_p=0;
    
    
    if(DEBUG) {
        printf("[DEBUG]IP指针列表> ");
        Word **j=IP_list;
        for (;j<IP_list_p ;j++ )
        {
            printf("%d ",(int)(*j));       //强制类型转换
        }
        printf("\n");
    }

//    printf("%s\n",name);
    if (name!=NULL) {
        n=(int)IP_list_p-(int)IP_list;
        dict_head = colon(name, IP_list, n, dict_head);
    }
    else if (var_name!=NULL) {
        dict_head = variable(var_name, 0, dict_head);
    }
    else
        explain();  //进入解释模式
    
    IP_list_p=IP_list;//临时区复原
    if_p = NULL;   //复原if和else和for指针
    else_p = NULL;
    for_p = NULL;
    
    showDS();
}


int main() 
{
    init();
    dict_head=NULL;
    

    pushh = code("push",push,dict_head);
    dict_head = code("bye",bye,pushh);
    dict_head = code("dup",dup,dict_head);
    dict_head = code("swap",swap,dict_head);
    dict_head = code("over",over,dict_head);
    dict_head = code("drop",drop,dict_head);
    
    dict_head = code(">r",tor,dict_head);
    dict_head = code("r>",rto,dict_head);
    dict_head = code("r@",rat,dict_head);

    dict_head = code(">t",tot,dict_head);
    dict_head = code("t>",tto,dict_head);
    dict_head = code("t@",tat,dict_head);

    dict_head = code("+",add,dict_head);
    dict_head = code("-",sub,dict_head);
    dict_head = code("*",mul,dict_head);
    dict_head = code("/",divv,dict_head);


    dict_head = code("ret",ret,dict_head);
    dict_head = code(";",ret,dict_head);
    dict_head = code("dolist",dolist,dict_head);
    dict_head = code("if",iff,dict_head);
    dict_head = code("else",elsee,dict_head);
    dict_head = code("then",then,dict_head);
    dict_head = code("for",forr,dict_head);
    dict_head = code("next",next,dict_head);
    
    dict_head = code("!", invar,dict_head);
    dict_head = code("@", outvar,dict_head);


    while (1)
    {
        printf(">>>");
        gets(cmdstr);
    //    printf("%s\n",cmdstr);
        compile(cmdstr);
    }
}
