#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "words.h"
#include "forth.h"

char cmdstr[BUFF_LEN];       //输入缓存区
Word *IP_list[BUFF_LEN/4];   //Word类型指针数组，长度为BUFF_LEN/4
Word **IP_list_p=IP_list;    //Word类型指针，指向IP_list[0]

Word *dict_head; //Forth的词典入口指针
Word *pushh;     //push词指针


//判断字符是否为空白字符
int is_blankchar(char c)
{
    return (c==' ' || c=='\t' || c=='\n' );
}


//跳过字符串头部的空白字符后返回指针
char * ignore_blankchar(char *s)
{
    while (is_blankchar(*s))
        s++;
    return s;
}


//跳过字符串中第一个词后返回指针
char * until_Wordend(char *s)
{
    while ( !is_blankchar(*s)  && *s!='\0')
        s++;
    return s;
}


//将字符串中第一个词后的空格变为'\0'，返回当前指针
char * split_Word(char *s)
{
    s=until_Wordend(s);
    if (*s=='\0')
        return s;
    *s='\0';
    s++;
    return s;
}


//判断字符串是否为数字
int is_num(char *s)
{
    while (*s != 0)
    {
        if (!isdigit((CELL)*s)) 
            return 0;
        s++;
    }
    return 1;
}


//定义if词、else词、for词定义时的临时位置指针
Word** if_p = NULL;
Word** else_p = NULL;
Word** for_p = NULL;


//根据Forth代码中的当前词的名字，去执行相应的编译操作
int find_Word(char *w, Word *dict)
{
    while (dict!=NULL && strcmp(dict->name,w))
    {  
        dict=dict->next;   //搜索词典链表
    }
    
    if(dict==NULL)    //词典链表搜索不到名字后，去判断是不是数字
    {
        if (!is_num(w))    
        {
                return 0;    //如果不是数字，返回0
        }
        else 
        {               //如果是数字
            PRINT("[DEBUG]成功找到数字%s\n",w);
            *IP_list_p=pushh;   //将push核心词指针存入IP_list_p数组        
            IP_list_p++;        //数组指针指向下一个位置
            *IP_list_p=(Word*)(CELL)(atoi(w));    //将CELL型数强制转换为Word指针类型
            IP_list_p++;

            return 1;
        }            
    } 
    
    if(dict->fn == NULL)  //在词典链表中搜索到名字后的判断，这个词是否是变量词！！
    {
        *IP_list_p=pushh;
        IP_list_p++;
        *IP_list_p=dict;
        IP_list_p++;
    }                
    else if(!strcmp("if",w))
    {
        *IP_list_p=dict;
        IP_list_p++;
        if_p = IP_list_p;
        IP_list_p++;
    }
    else if(!strcmp("else",w)) 
    {
        *IP_list_p=dict;  
        IP_list_p++;
        else_p = IP_list_p;
        *if_p = (Word*)(else_p - if_p + 1);  //+1的意思是越过else词和后面的then偏移量位置
        IP_list_p++;
    }
    else if(!strcmp("then",w))
    {
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
        *IP_list_p=dict;
        IP_list_p++;
        for_p = IP_list_p;
        IP_list_p++;
        
    }
    else if(!strcmp("next",w))
    {
        *IP_list_p=dict;  
        IP_list_p++;
        *for_p = (Word*)(IP_list_p - for_p + 1); 
        *IP_list_p = (Word*)(IP_list_p - for_p + 1); 
        IP_list_p++;
    }
    else 
    {
        *IP_list_p=dict;    
        IP_list_p++;
    }
    
    PRINT("[DEBUG]成功找到%s词\n",w);
    return 1;
}


//重置数据栈指针、返回栈指针、临时栈指针、指令列表的指针
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


//对指令列表进行解释执行
void explain()
{
    IP=IP_list;
    
    while(IP != IP_list_p)
    {
        PRINT("[DEBUG]解释执行> %s\n", (*IP)->name);
        
        (*IP)->fn();
        ++IP;
    }
}


//将一行Forth代码字符串编译为指令列表
void compile(char *s)
{
    s=ignore_blankchar(s);
    
    char define_word;
    char *define_name;
    //如果第一个词是扩展定义词或是变量定义词，则保存后面一个词为define_name
    if (*s == ':' || *s == '$')
    {
        define_word = *s;
        s++;
        define_name=ignore_blankchar(s);
        s=define_name;
        s=split_Word(s);
    }
    else
    {
        define_word = '\0';
        define_name = NULL;
    }
    
    //在Forth词典中寻找每一个词的定义，转换IP指针列表中的指针
    char *one_word;
    while (*s!=0)
    {
        s=ignore_blankchar(s);
        one_word=s;
        s=split_Word(s);

        if(!find_Word(one_word, dict_head) )
        {
            printf("[%s]?\n",one_word);
            init();
            return;
        }
    }
  
    //DEBUG模式下打印出IP指针列表
    if(DEBUG) {
        printf("[DEBUG]IP指针列表> ");
        Word **j=IP_list;
        for (;j<IP_list_p ;j++ )
        {
            printf("%ld ",(CELL)(*j));       //强制类型转换
        }
        printf("\n");
    }

    //若有定义词则把扩展词或变量词加入Forth词典，若无则执行解释模式
    if(define_word == ':')
    {
        PRINT("[DEBUG]定义扩展词 %s\n", define_name);
        dict_head = colon(define_name, IP_list, (CELL)IP_list_p - (CELL)IP_list, dict_head);
    }
    else if(define_word == '$')
    {
        PRINT("[DEBUG]定义变量词 %s\n", define_name);
        dict_head = variable(define_name, 0, dict_head);
    }
    else
        explain(); 
    
    //复原临时区、if、else和for指针
    IP_list_p=IP_list;
    if_p = NULL; 
    else_p = NULL;
    for_p = NULL;
    
    if(DEBUG) showDS();
}


//主程序入口
int main(int argc, char *argv[]) 
{
    init();
    dict_head=NULL;
    
    //初始化词典
    pushh = code("push",push,dict_head);
    dict_head = code("bye",bye,pushh);
    dict_head = code(".s",showDS,dict_head);
    dict_head = code(".",popDS,dict_head);
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

    FILE *fp; //文件指针
    char c;
    int i = 0;
    if(argc > 1) 
    {
        if((fp = fopen(*++argv, "r")) == NULL)
        {
            printf("Can't open %s\n", *argv);
            return 1;
        }
        
        while((c = getc(fp)) != EOF)
        {
            if(c != '\n')
            {
                cmdstr[i] = c;
                i++;
            }
            else
            {
                cmdstr[i] = '\0';
                compile(cmdstr);
                i = 0;
            }           
        }
        fclose(fp);
    }

    while (1)
    {
        printf(">>>");
        fgets(cmdstr, BUFF_LEN - 1, stdin);     //从标准输入获取一行Forth代码字符串
        compile(cmdstr);  //编译执行
    }

    
    return 0;
}
