#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "forth.h"


//Forth文本解释器
void interpret(char *s, Dict *dict)
{
    char *one_word;
    Word *immediate;
    IP_head = IP_list;
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
            || !strcmp("myself",one_word)
            || !strcmp(";",one_word))
        {
            PRINT("[DEBUG]执行立即词 %s\n", one_word)
            immediate = dict_search_name(forth_dict, one_word);
            immediate->fn();
        }
        else if (!strcmp("constant",one_word)
            || !strcmp("variable",one_word)
            || !strcmp("forget",one_word)
            || !strcmp("see",one_word)
            || !strcmp("load",one_word)
            || !strcmp(":",one_word))
        {
            explain();
            IP_head = IP;
            
            PRINT("[DEBUG]执行立即词 %s\n", one_word)
            immediate = dict_search_name(forth_dict, one_word);
            s=ignore_blankchar(s);
            one_word=s;
            s=split_Word(s);
            strcpy(next_word, one_word);
            PRINT("[DEBUG]找到名字 %s\n", one_word)
            if(immediate == NULL)  //如果是load词
                load_file(next_word);
            else
                immediate->fn();
        }
        else if(!strcmp(".\"",one_word))  //如果是." str " 则立即编译其中的字符串str
        {
            PRINT("[DEBUG]编译字符串\n")
            s=ignore_blankchar(s);
            char tempstr[BUFF_LEN]; 
            while(*s != '\"')
            {
                sprintf(tempstr, "%ld", (CELL)(*s));
                find(tempstr, dict);
                find("emit", dict);
                s++;
            }
            s++;
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
        else if(!find(one_word, dict) ) //编译词
        {
            printf("[%s]?\n",one_word);
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
    char cmdstr[BUFF_LEN];  //输入缓存区
    FILE *fp; //文件指针
    char c;
    int i = 0;

    if((fp = fopen(file_path, "r")) == NULL)
    {
        printf("Can't open %s\n", file_path);
        return 0;
    }
    
    do
    {
        c = getc(fp);
        if(c != '\n' && c != EOF)
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
    } while(c != EOF);

    fclose(fp);

    return 1;
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
    dict_ins_next(forth_dict, code("depth",depth));
    dict_ins_next(forth_dict, code("+",add));
    dict_ins_next(forth_dict, code("-",sub));
    dict_ins_next(forth_dict, code("*",mul));
    dict_ins_next(forth_dict, code("/",divv));
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
    dict_ins_next(forth_dict, code("words",words));
    dict_ins_next(forth_dict, code(":",defcolon));
    dict_ins_next(forth_dict, code(";",endcolon));
    dict_ins_next(forth_dict, code("if",_if));
    dict_ins_next(forth_dict, code("else",_else));
    dict_ins_next(forth_dict, code("then",_then));
    dict_ins_next(forth_dict, code("do",_do));
    dict_ins_next(forth_dict, code("loop",_loop));
    dict_ins_next(forth_dict, code("see",see));
    dict_ins_next(forth_dict, code("forget",forget));
    dict_ins_next(forth_dict, code("variable",var));
    dict_ins_next(forth_dict, code("constant",cons));
    dict_ins_next(forth_dict, code("myself", myself));
    
    for(; argc > 1; argc--)
        load_file(*++argv);

    while (1)
    {
        printf(">>> ");
        gets(cmdstr);
        interpret(cmdstr, forth_dict);
    }

    
    return 0;
}
