#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "forth.h"
    
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
            *IP=dict_search_name(dict, "push");   //将push核心词指针存入IP数组        
            IP++;        //数组指针指向下一个位置
            *IP=(Word*)(CELL)(atoi(name));    //将CELL型数强制转换为Word指针类型
            IP++;

            return 1;
        }            
    }
    
    if(word_p->fn == NULL)  //在词典链表中搜索到名字后的判断，这个词是否是变量词！！
    {
        *IP=dict_search_name(dict, "push");
        IP++;
        *IP=word_p;
        IP++;
    }
    else 
    {
        *IP=word_p;    
        IP++;
    }
    
    PRINT("[DEBUG]成功编译%s词\n",name)
    return 1;
}


//指令列表执行
void explain(Word  **IP_head)
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


//Forth文本解释器
void interpret(char *s, Dict *dict)
{
    char *define_name;
    char define_str[BUFF_LEN];
    char *one_word;
    Word *immediate;
    Word  **IP_head = IP_list;
    IP=IP_list;
       
    while (*ignore_blankchar(s) != '\0')
    {
        s=ignore_blankchar(s);  //删除字符串头部空格
        one_word=s;             //将字符串指针赋给one_word
        s=split_Word(s);        //将字符串头部第一个词后的空格换成'\0'，再返回第二个词头的指针
        
        if(!strcmp("if",one_word)
            || !strcmp("else",one_word) 
            || !strcmp("then",one_word)
            || !strcmp("do",one_word)
            || !strcmp("loop",one_word)
            || !strcmp("words",one_word))
        {
            PRINT("[DEBUG]执行立即词 %s\n", one_word)
            immediate = dict_search_name(forth_dict, one_word);
            immediate->fn();
        }
        else if (!strcmp("constant",one_word)
            || !strcmp("variable",one_word)
            || !strcmp("forget",one_word)
            || !strcmp("see",one_word))
        {
            explain(IP_head);
            IP_head = IP;
            
            PRINT("[DEBUG]执行立即词 %s\n", one_word)
            immediate = dict_search_name(forth_dict, one_word);
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s);
            strcpy(next_word, one_word);
            immediate->fn();
            
            IP_head = IP;
        }
        else if(!strcmp(".\"",one_word))  //如果是." str " 则立即编译其中的字符串str
        {
            PRINT("[DEBUG]编译字符串\n")
            s=ignore_blankchar(s);
            char tempstr[BUFF_LEN]; 
            while(*s != '\"')
            {
                sprintf(tempstr, "%ld", (CELL)(*s));
                compile(tempstr, dict);
                compile("emit", dict);
                s++;
            }
            s++;
        }
        else if (!strcmp(":",one_word))  //进入扩展词定义模式
        {
            explain(IP_head);
            IP_head = IP;
            
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s); 
            define_name=one_word;
            PRINT("[DEBUG]定义扩展词 %s\n", define_name)
            s=ignore_blankchar(s);
            
            char *c;
            int i;
            for(c = s, i = 0; *c != ';'; c++, i++)
            {
                define_str[i] = *c;
            }
            define_str[i] = '\0';
        }
        else if(!strcmp(";",one_word))   //结束扩展词定义模式
        {
            compile(one_word, dict);
            int n = (CELL)IP - (CELL)IP_head;
            dict_ins_next(dict, colon(define_name, define_str, IP_head, n));

            Word *myself_p = dict_search_name(dict, "myself");  //支持递归词myself
            Word *colon_p = dict_search_name(dict, define_name);
            Word **p=IP_head;
            for (;p<IP ;p++ )
            {
                if(*p == myself_p)
                {
                    *p = colon_p;
                    change_colon(colon_p, IP_head, n);
                }
            }
            
            IP_head = IP;
        }
        else if(!strcmp("(",one_word))  //注释模式
        {
            s=ignore_blankchar(s);
            while(*s != ')')
            {
                s++;
            }
            s++;
        }
        else if(!compile(one_word, dict) ) //编译词
        {
            printf("[%s]?\n",one_word);
            empty_stack();
            IP=IP_list;
            return;
        }
    }
    explain(IP_head);

    //DEBUG模式下打印出IP指针列表
    if(DEBUG) {
        printf("[DEBUG]IP指针列表> ");
        Word **p=IP_list;
        for (;p<IP ;p++ )
        {
            printf("%ld ",(CELL)(*p));
        }
        printf("\n");
        showds();
    }
}


//主程序入口
int main(int argc, char *argv[]) 
{
    char cmdstr[BUFF_LEN];  //输入缓存区
    empty_stack();
    IP=IP_list;
    forth_dict= dict_init();
    
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
    dict_ins_next(forth_dict, code("<>",noequal));
    dict_ins_next(forth_dict, code(">",morethan));
    dict_ins_next(forth_dict, code("<",lessthan));
    dict_ins_next(forth_dict, code("?branch",if_branch));
    dict_ins_next(forth_dict, code("branch",branch));
    dict_ins_next(forth_dict, code("(do)",__do));
    dict_ins_next(forth_dict, code("(loop)",__loop));
    dict_ins_next(forth_dict, code(">r",tor));
    dict_ins_next(forth_dict, code("r>",rto));
    dict_ins_next(forth_dict, code("r@",rat));
    dict_ins_next(forth_dict, code("emit", emit));
    dict_ins_next(forth_dict, code("myself", myself));
    dict_ins_next(forth_dict, code("if",_if));
    dict_ins_next(forth_dict, code("else",_else));
    dict_ins_next(forth_dict, code("then",_then));
    dict_ins_next(forth_dict, code("do",_do));
    dict_ins_next(forth_dict, code("loop",_loop));
    dict_ins_next(forth_dict, code("see",see));
    dict_ins_next(forth_dict, code("forget",forget));
    dict_ins_next(forth_dict, code("variable",var));
    dict_ins_next(forth_dict, code("constant",cons));
    dict_ins_next(forth_dict, code("words",words));
    
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
