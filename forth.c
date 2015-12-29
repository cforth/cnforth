#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "forth.h"

char cmdstr[BUFF_LEN];       //输入缓存区
Word *IP_list[BUFF_LEN/4];   //Word类型指针数组，长度为BUFF_LEN/4
Word **IP_list_p=IP_list;    //Word类型指针，指向IP_list[0]


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


//查看扩展词的定义
int see(char *name, Dict *dict)
{
    Word *word_p;
    word_p = dict_search_name(dict, name);
    
    if(word_p == NULL)
    {
        printf("%s : Can't find!\n",name);
        return 0;
    }
    else
    {
        printf("%s : %s\n",name, word_p->str);
        return 1;
    }
}


//根据Forth代码中的当前词的名字，去执行相应的编译操作
int compile(char *name, Dict *dict)
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
            *IP_list_p=dict_search_name(dict, "push");   //将push核心词指针存入IP_list_p数组        
            IP_list_p++;        //数组指针指向下一个位置
            *IP_list_p=(Word*)(CELL)(atoi(name));    //将CELL型数强制转换为Word指针类型
            IP_list_p++;

            return 1;
        }            
    }
    
    if(word_p->fn == NULL)  //在词典链表中搜索到名字后的判断，这个词是否是变量词！！
    {
        *IP_list_p=dict_search_name(dict, "push");
        IP_list_p++;
        *IP_list_p=word_p;
        IP_list_p++;
    }                
    else if(!strcmp("if",name))
    {
        *IP_list_p=word_p;
        IP_list_p++;
        rs_push((CELL)IP_list_p);
        IP_list_p++;
    }
    else if(!strcmp("else",name)) 
    {
        *IP_list_p=word_p;  
        IP_list_p++;
        Word** else_p = IP_list_p;
        Word** if_p = (Word**)(rs_pop());
        rs_push((CELL)else_p);
        *if_p = (Word*)(IP_list_p - if_p + 1);  //+1的意思是越过else词和后面的then偏移量位置
        IP_list_p++;
    }
    else if(!strcmp("then",name))
    {
        *IP_list_p=word_p;
        Word** branch_p = (Word**)(rs_pop());
        *branch_p = (Word*)(IP_list_p - branch_p + 1); 
        IP_list_p++;
    }
    else if(!strcmp("do",name))
    {
        *IP_list_p=word_p;
        IP_list_p++;
        rs_push((CELL)IP_list_p);
        IP_list_p++;
        
    }
    else if(!strcmp("loop",name))
    {
        *IP_list_p=word_p;  
        IP_list_p++;
        Word** do_p = (Word**)(rs_pop());
        *do_p = (Word*)(IP_list_p - do_p + 1); 
        *IP_list_p = (Word*)(IP_list_p - do_p + 1); 
        IP_list_p++;
    }
    else 
    {
        *IP_list_p=word_p;    
        IP_list_p++;
    }
    
    PRINT("[DEBUG]成功编译%s词\n",name)
    return 1;
}


//指令列表执行
void explain()
{
    IP=IP_list;
    
    while(IP != IP_list_p)
    {
        PRINT("[DEBUG]解释执行> %s\n", (*IP)->name)
        
        (*IP)->fn();
        ++IP;
    }
}


//Forth文本解释器
void interpret(char *s, Dict *dict)
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
        
        if(!strcmp(".\"",one_word))  //如果是." str " 则立即编译其中的字符串str
        {
            s=ignore_blankchar(s);
            char tempstr[100]; 
            while(*s != '\"')
            {
                sprintf(tempstr, "%ld", (CELL)(*s));
                compile(tempstr, dict);
                compile("emit", dict);
                s++;
            }
            s++;
        }
        else if (!strcmp(":",one_word) || !strcmp("variable",one_word)) //如果是扩展定义词或是变量定义词
        {
            define_word = one_word;
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
            define_name=one_word;  //保存后面一个词为define_name
            s=ignore_blankchar(s);
            
            define_str = (char*)malloc(strlen(s)+1);
            strcpy(define_str, s); //保存扩展字的定义
        }
        else if(!strcmp("(",one_word))  //如果是注释，则忽略注释
        {
            s=ignore_blankchar(s);
            while(*s != ')')
            {
                s++;
            }
            s++;
        }
        else if (!strcmp("see",one_word)) //如果是see则打印扩展词的定义
        {
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
            see(one_word, dict);
            return;
        }
        else if(!compile(one_word, dict) ) //编译词
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
        Word **p=IP_list;
        for (;p<IP_list_p ;p++ )
        {
            printf("%ld ",(CELL)(*p));
        }
        printf("\n");
    }

    //若有定义词则把扩展词或变量词加入Forth词典，若无则执行指令列表
    if(!strcmp(":",define_word))
    {
        PRINT("[DEBUG]定义扩展词 %s\n", define_name)
        int n = (CELL)IP_list_p - (CELL)IP_list;
        dict_ins_next(dict, colon(define_name, define_str, IP_list, n));
        //下面这段代码用于支持递归词myself!!
        Word *myself_p = dict_search_name(dict, "myself");
        Word *colon_p = dict_search_name(dict, define_name);
        Word **p=IP_list;
        for (;p<IP_list_p ;p++ )
        {
            if(*p == myself_p)
            {
                *p = colon_p;
                change_colon(colon_p, IP_list, n); //修改colon词，用于递归定义
            }
        }
    }
    else if(!strcmp("variable",define_word))
    {
        PRINT("[DEBUG]定义变量词 %s\n", define_name)
        dict_ins_next(dict, variable(define_name, define_str, 0));
    }
    else
        explain(); 
    
    //复原IP列表指针
    IP_list_p=IP_list;
  
    if(DEBUG) showds();
}


//主程序入口
int main(int argc, char *argv[]) 
{
    empty_stack();
    IP_list_p=IP_list;
    Dict *forth_dict= dict_init();
    
    //初始化词典
    dict_ins_next(forth_dict, code("dolist",dolist));
    dict_ins_next(forth_dict, code("push",push));
    dict_ins_next(forth_dict, code(".",popds));
    dict_ins_next(forth_dict, code("bye",bye));
    
    dict_ins_next(forth_dict, code("ret",ret));
    dict_ins_next(forth_dict, code(";",ret));
    dict_ins_next(forth_dict, code("cr", putcr));
    
    dict_ins_next(forth_dict, code("depth",depth));
    dict_ins_next(forth_dict, code("+",add));
    dict_ins_next(forth_dict, code("-",sub));
    dict_ins_next(forth_dict, code("*",mul));
    dict_ins_next(forth_dict, code("/",divv));

    dict_ins_next(forth_dict, code("dup",dup));
    dict_ins_next(forth_dict, code("swap",swap));
    dict_ins_next(forth_dict, code("over",over));
    dict_ins_next(forth_dict, code("drop",drop));
    dict_ins_next(forth_dict, code(".s",showds));
    dict_ins_next(forth_dict, code("pick",pick));
    dict_ins_next(forth_dict, code("roll",roll));
    
    dict_ins_next(forth_dict, code("!", invar));
    dict_ins_next(forth_dict, code("@", outvar));
    
    dict_ins_next(forth_dict, code("=",equal));
    dict_ins_next(forth_dict, code(">",morethan));
    dict_ins_next(forth_dict, code("<",lessthan));

    dict_ins_next(forth_dict, code("if",iff));
    dict_ins_next(forth_dict, code("else",elsee));
    dict_ins_next(forth_dict, code("then",then));
    dict_ins_next(forth_dict, code("do",doo));
    dict_ins_next(forth_dict, code("loop",loop));
    
    dict_ins_next(forth_dict, code(">r",tor));
    dict_ins_next(forth_dict, code("r>",rto));
    dict_ins_next(forth_dict, code("r@",rat));
    dict_ins_next(forth_dict, code(">t",tot));
    dict_ins_next(forth_dict, code("t>",tto));
    dict_ins_next(forth_dict, code("t@",tat));

    dict_ins_next(forth_dict, code("emit", emit));
    dict_ins_next(forth_dict, code("myself", myself));

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
                interpret(cmdstr, forth_dict);
                i = 0;
            }           
        }
        fclose(fp);
    }

    while (1)
    {
        printf(">>> ");
        gets(cmdstr);
        interpret(cmdstr, forth_dict);
    }

    
    return 0;
}
