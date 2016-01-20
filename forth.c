#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "forth.h"


//判断字符是否为空白字符
int CheckBlank(char c)
{
    return (c==' ' || c=='\t' || c=='\n' );
}


//返回输入流中当前的forth词，并更新text_p指针
char *ParseWord()
{
    char *now;
    while (CheckBlank(*text_p)) //跳过字符串头部的空白字符后返回指针
        text_p++;
    
    now = text_p;
    
    while ( !CheckBlank(*text_p)  && (*text_p)!='\0') //跳过字符串中第一个词
        text_p++;
    
    if(*text_p == '\0') 
        return now;
    *text_p = '\0';   //将字符串中第一个词后的空格变为'\0'
    text_p++;
    return now;
    
}


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
    if(word->code_p == colon_code) free(word->wplist);
    free(word);
}

int dict_rem_after(Dict *dict, char *name)
{
    Word *w = dict_search_name(dict, name);
    Word *del_w;
    
    if(w == NULL)
    {
        printf("%s :\n\tCan't find!\n", name);
    }
    else
    {
        if(w->wplist == NULL)
        {
            printf("\tCore Word can't be deleted\n");
            return 0;
        }
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


Word *create(char *name, fnP  fp) //创建一个Forth词的名字域
{
    Word *w=(Word*)malloc(sizeof(Word));
    w->code_p=fp;
    
    w->name=(char*)malloc(strlen(name)+1);
    strcpy(w->name,name);
    
    w->wplist=NULL;   

    w->type = 0;

    return w;
}


void does(Word *c, Word **list, int n)  //创建一个Forth词的参数域
{
    if(n != 0) {
        c->wplist = (Word**)malloc(n);
        memcpy(c->wplist,list, n);
    } else {
        c->wplist = list;
    }
}


void colon_code() //扩展词的代码域
{
    RP++;
    *RP=(CELL)IP;
    IP=(*IP)->wplist-1;
    PRINT("[DEBUG]进入子例程\n")
}


void cons_code() //常数词的代码域
{
    ds_push((CELL)((*IP)->wplist));
}


void var_code()  //变量词的代码域
{
    ds_push((CELL)*IP);
}


//指令列表执行
void explain()
{
    Word  **IP_end = IP;
    IP=IP_head;
    
    while(IP != IP_end)
    {
        PRINT("[DEBUG]解释执行> %s\n", (*IP)->name)
        
        (*IP)->code_p();
        ++IP;
    }
    IP_head = IP;  //重新设置
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
    else if(word_p->type == 1)  //立即词
    {
        PRINT("[DEBUG]执行立即词 %s\n", name)
        word_p->code_p();
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
    exit(0);
}


void ip_push(Word *w)
{
    if(IP >= IP_list+BUFF_LEN){stack_error(2);}
    *IP=w;
    IP++;
}


void ds_push(CELL n)
{
    if(DP >= DS+STACK_LEN-1){stack_error(2);}
    DP++;
    *DP=n;
}


void rs_push(CELL n)
{
    if(RP >= RS+STACK_LEN-1){stack_error(2);}
    RP++;
    *RP=n;
}


CELL ds_pop()
{
    if(DP <= DS-1){stack_error(1);}
    DP--;
    return *(DP+1); 
}


CELL rs_pop()
{
    if(RP <= RS-1){stack_error(1);}
    RP--;
    return *(RP+1); 
}


CELL ds_top()
{
    if(DP <= DS-1){stack_error(1);}
    return *DP;
}


CELL rs_top()
{
    if(RP <= RS-1){stack_error(1);}
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
    if(DP-k+1 <= DS-1){stack_error(1);}
    ds_push(*(DP-k+1));
}


void roll()
{
    CELL k = ds_pop();
    if(DP-k+1 <= DS-1){stack_error(1);}
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
    p->wplist = (Word **)ds_pop();
}


void outvar() 
{
    Word *p = (Word *)(ds_pop());
    ds_push((CELL)(p->wplist));
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
void immediate()
{
    forth_dict->head->type = 1;
}


void defcolon()
{
    explain();
    current_text = ParseWord();
    define_p = create(current_text, colon_code);
}


void endcolon()
{
    ip_push(dict_search_name(forth_dict, "ret"));
    int n = (CELL)IP - (CELL)IP_head;
    dict_ins_next(forth_dict, define_p);
    does(define_p, IP_head, n);
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
    explain();
    current_text = ParseWord();
    Word *word_p = dict_search_name(forth_dict, current_text);
    
    if(word_p == NULL)
    {
        printf("%s :\n\tCan't find!\n", current_text);
    }
    else
    {   //反编译wplist，得出扩展词的字符串定义
        printf("%s :\n\t", current_text);
        if(word_p->code_p == colon_code)
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
    explain();
    current_text = ParseWord();
    dict_rem_after(forth_dict, current_text); //删除当前扩展词以及词典中该词之后定义的所有扩展词
}


void var()
{
    explain();
    current_text = ParseWord();
    dict_ins_next(forth_dict, create(current_text, var_code));
    does(forth_dict->head, (Word **)0, 0);
}


void cons()
{
    explain();
    current_text = ParseWord();
    dict_ins_next(forth_dict, create(current_text, cons_code));
    does(forth_dict->head, (Word **)ds_pop(), 0);
    
}


void myself()
{
    ip_push((Word *)define_p);
}


//Forth文本解释器
void interpret()
{
    text_p = cmdstr;
    IP_head = IP_list;
    IP=IP_list;
       
    while (*(current_text = ParseWord()) != '\0')
    {
        if(!strcmp(".\"",current_text))  //如果是." str " 则立即编译其中的字符串str
        {
            PRINT("[DEBUG]编译字符串\n")

            char tempstr[BUFF_LEN]; 
            while(*text_p != '\"')
            {
                sprintf(tempstr, "%ld", (CELL)(*text_p));
                find(tempstr, forth_dict);
                find("emit", forth_dict);
                text_p++;
            }
            text_p++;
        }
        else if(!strcmp("(",current_text))  //注释模式
        {

            while(*text_p != ')')
            {
                text_p++;
            }
            text_p++;
        }
        else if(!find(current_text, forth_dict) )
        {
            printf("[%s]?\n",current_text);
            empty_stack();
            IP=IP_list;
            return;
        }
    }
    explain();

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


//从外部文件读取Forth代码
int load_file(char *file_path)
{
    FILE *fp; //文件指针
    char c;
    int i = 0;
    int flag = 0;

    if((fp = fopen(file_path, "r")) == NULL)
    {
        printf("Can't open %s\n", file_path);
        return 0;
    }
    
    do
    {
        c = getc(fp);
        if((c != '\n' && c != EOF) 
            || (c == '\n' && flag == 1))
        {
            if(c == ':') flag = 1;
            else if(c == ';') flag = 0;
            cmdstr[i] = c;
            i++;
        }
        else if((c == '\n' && flag == 0)
            || c == EOF)
        {
            cmdstr[i] = '\0';
            interpret();
            i = 0;
        }           
    } while(c != EOF);
    fclose(fp);

    return 1;
}


//load词
void load()
{
    explain();
    current_text = ParseWord();
    load_file(current_text);
}


//主程序入口
int main(int argc, char *argv[]) 
{
    empty_stack();
    IP=IP_list;
    forth_dict= dict_init();
    
    //初始化词典
    dict_ins_next(forth_dict, create("push",push));
    dict_ins_next(forth_dict, create(".",popds));
    dict_ins_next(forth_dict, create("bye",bye));
    dict_ins_next(forth_dict, create("ret",ret));
    dict_ins_next(forth_dict, create("depth",depth));
    dict_ins_next(forth_dict, create("+",add));
    dict_ins_next(forth_dict, create("-",sub));
    dict_ins_next(forth_dict, create("*",mul));
    dict_ins_next(forth_dict, create("/",divv));
    dict_ins_next(forth_dict, create("drop",drop));
    dict_ins_next(forth_dict, create(".s",showds));
    dict_ins_next(forth_dict, create("pick",pick));
    dict_ins_next(forth_dict, create("roll",roll));
    dict_ins_next(forth_dict, create("!", invar));
    dict_ins_next(forth_dict, create("@", outvar));
    dict_ins_next(forth_dict, create("=",equal));
    dict_ins_next(forth_dict, create("<>",noequal));
    dict_ins_next(forth_dict, create(">",morethan));
    dict_ins_next(forth_dict, create("<",lessthan));
    dict_ins_next(forth_dict, create("?branch",if_branch));
    dict_ins_next(forth_dict, create("branch",branch));
    dict_ins_next(forth_dict, create("(do)",__do));
    dict_ins_next(forth_dict, create("(loop)",__loop));
    dict_ins_next(forth_dict, create(">r",tor));
    dict_ins_next(forth_dict, create("r>",rto));
    dict_ins_next(forth_dict, create("r@",rat));
    dict_ins_next(forth_dict, create("emit", emit));
    dict_ins_next(forth_dict, create("words",words));
    dict_ins_next(forth_dict, create("myself", myself)); immediate();
    dict_ins_next(forth_dict, create(":",defcolon)); immediate();
    dict_ins_next(forth_dict, create(";",endcolon)); immediate();
    dict_ins_next(forth_dict, create("if",_if)); immediate();
    dict_ins_next(forth_dict, create("else",_else)); immediate();
    dict_ins_next(forth_dict, create("then",_then)); immediate();
    dict_ins_next(forth_dict, create("do",_do)); immediate();
    dict_ins_next(forth_dict, create("loop",_loop)); immediate();
    dict_ins_next(forth_dict, create("see",see)); immediate();
    dict_ins_next(forth_dict, create("forget",forget)); immediate();
    dict_ins_next(forth_dict, create("variable",var)); immediate();
    dict_ins_next(forth_dict, create("constant",cons)); immediate();
    dict_ins_next(forth_dict, create("load",load)); immediate();
    
    
    for(; argc > 1; argc--)
        load_file(*++argv);

    while (1)
    {
        printf(">>> ");
        gets(cmdstr);
        interpret();
    }

    
    return 0;
}
