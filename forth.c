#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "words.h"
#include "forth.h"

char cmdstr[BUFF_LEN];       //输入缓存区
Word *IP_list[BUFF_LEN/4];   //Word类型指针数组，长度为BUFF_LEN/4
Word **IP_list_p=IP_list;    //Word类型指针，指向IP_list[0]

Dict *forth_dict;

//Word *dict_head; //Forth的词典入口指针
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


//查看扩展词的定义
int see(char *w, Word *dict)
{
    while (dict!=NULL && strcmp(dict->name,w))
    {  
        dict=dict->next;   //搜索词典链表
    }
    
    if(dict == NULL)
    {
        printf("%s : Can't find!\n",w);
        return 0;
    }
    else
    {
        printf("%s : %s\n",w, dict->str);
        return 1;
    }
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
    char *define_word;
    char *define_str;
    char *define_name;
    char *one_word;
       
    while (*ignore_blankchar(s)!=0)
    {
        s=ignore_blankchar(s);  //删除字符串头部空格
        one_word=s;             //将字符串指针赋给one_word
        s=split_Word(s);        //将字符串头部第一个词后的空格换成'\0'，再返回第二个词头的指针
                                //如此一来，one_word其实就只是指向包含了第一个词的字符串

        if(!strcmp(".\"",one_word))  //如果是." str " 则打印其中的字符串str
        {
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
                
            while(strcmp("\"",one_word))
            {
                printf("%s ", one_word);
                
                s=ignore_blankchar(s);
                one_word=s;
                s=split_Word(s);   
            } 
            printf("\n");
            if(!strcmp("\"",one_word)) continue;   //忽略"
        }
        else if (!strcmp(":",one_word) || !strcmp("$",one_word)) //如果是扩展定义词或是变量定义词
        {
            define_word = one_word;
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
            define_name=one_word;  //保存后面一个词为define_name
            s=ignore_blankchar(s);
            
            define_str = (char*)malloc(strlen(s)+1);
            strcpy(define_str, s); //保存扩展字的定义
            
            one_word=s;
            s=split_Word(s); 
        }
        else if (!strcmp("see",one_word)) //如果是see则打印扩展词的定义
        {
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
            see(one_word, forth_dict->head);
            return;
        }
            
        if(!find_Word(one_word, forth_dict->head) ) //在Forth词典中搜索
        {
            printf("[%s]?\n",one_word);
            empty_stack();
            IP_list_p=IP_list;
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
    if(!strcmp(":",define_word))
    {
        PRINT("[DEBUG]定义扩展词 %s\n", define_name);
        dict_ins_next(forth_dict, colon(define_name, define_str, IP_list, (CELL)IP_list_p - (CELL)IP_list));
    }
    else if(!strcmp("$",define_word))
    {
        PRINT("[DEBUG]定义变量词 %s\n", define_name);
        dict_ins_next(forth_dict, variable(define_name, define_str, 0));
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
    empty_stack();
    IP_list_p=IP_list;
    printf("1\n");
forth_dict = dict_init();
    printf("2\n");
    //dict_head=NULL;
    
    //初始化词典
    pushh = code("push",push);
    dict_ins_next(forth_dict, pushh);
    dict_ins_next(forth_dict, code("bye",bye));
    dict_ins_next(forth_dict, code(".s",showDS));
    dict_ins_next(forth_dict, code(".",popDS));
    dict_ins_next(forth_dict, code("dup",dup));
    dict_ins_next(forth_dict, code("swap",swap));
    dict_ins_next(forth_dict, code("over",over));
    dict_ins_next(forth_dict, code("drop",drop));
    
    dict_ins_next(forth_dict, code(">r",tor));
    dict_ins_next(forth_dict, code("r>",rto));
    dict_ins_next(forth_dict, code("r@",rat));

    dict_ins_next(forth_dict, code(">t",tot));
    dict_ins_next(forth_dict, code("t>",tto));
    dict_ins_next(forth_dict, code("t@",tat));

    dict_ins_next(forth_dict, code("+",add));
    dict_ins_next(forth_dict, code("-",sub));
    dict_ins_next(forth_dict, code("*",mul));
    dict_ins_next(forth_dict, code("/",divv));


    dict_ins_next(forth_dict, code("ret",ret));
    dict_ins_next(forth_dict, code(";",ret));
    dict_ins_next(forth_dict, code("dolist",dolist));
    dict_ins_next(forth_dict, code("if",iff));
    dict_ins_next(forth_dict, code("else",elsee));
    dict_ins_next(forth_dict, code("then",then));
    dict_ins_next(forth_dict, code("for",forr));
    dict_ins_next(forth_dict, code("next",next));
    
    dict_ins_next(forth_dict, code("!", invar));
    dict_ins_next(forth_dict, code("@", outvar));

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
