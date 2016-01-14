#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "forth.h"


//Forth文本解释器
void interpret(char *s, Dict *dict)
{
    char *now_word;
    char *before_word;
    IP_head = IP_list;
    IP=IP_list;
       
    while (*ignore_blankchar(s) != '\0')
    {
        s=ignore_blankchar(s);  //删除字符串头部空格
        now_word=s;             //将字符串指针赋给one_word
        s=split_Word(s);        //将字符串头部第一个词后的空格换成'\0'，再返回第二个词头的指针
        
        if (!strcmp("constant",now_word)
            || !strcmp("variable",now_word)
            || !strcmp("forget",now_word)
            || !strcmp("see",now_word)
            || !strcmp("load",now_word)
            || !strcmp(":",now_word))
        {
            explain();
            before_word = now_word;
            s=ignore_blankchar(s);
            now_word=s;
            s=split_Word(s);
            strcpy(next_word, now_word);
            find(before_word, dict);
        }
        else if(!strcmp(".\"",now_word))  //如果是." str " 则立即编译其中的字符串str
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
        else if(!strcmp("(",now_word))  //注释模式
        {
            s=ignore_blankchar(s);
            while(*s != ')')
            {
                s++;
            }
            s++;
        }
        else if(!find(now_word, dict) )
        {
            printf("[%s]?\n",now_word);
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
            interpret(cmdstr, forth_dict);
            i = 0;
        }           
    } while(c != EOF);
    fclose(fp);

    return 1;
}


//load词
void load()
{
    load_file(next_word);
}


//主程序入口
int main(int argc, char *argv[]) 
{
    char cmdstr[BUFF_LEN];  //输入缓存区
    empty_stack();
    IP=IP_list;
    forth_dict= dict_init();
    
    //初始化词典
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
    dict_ins_next(forth_dict, code("myself", myself)); immediate();
    dict_ins_next(forth_dict, code(":",defcolon)); immediate();
    dict_ins_next(forth_dict, code(";",endcolon)); immediate();
    dict_ins_next(forth_dict, code("if",_if)); immediate();
    dict_ins_next(forth_dict, code("else",_else)); immediate();
    dict_ins_next(forth_dict, code("then",_then)); immediate();
    dict_ins_next(forth_dict, code("do",_do)); immediate();
    dict_ins_next(forth_dict, code("loop",_loop)); immediate();
    dict_ins_next(forth_dict, code("see",see)); immediate();
    dict_ins_next(forth_dict, code("forget",forget)); immediate();
    dict_ins_next(forth_dict, code("variable",var)); immediate();
    dict_ins_next(forth_dict, code("constant",cons)); immediate();
    dict_ins_next(forth_dict, code("load",load)); immediate();
    
    
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
