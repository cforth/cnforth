#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define	DEBUG	 1
#define	BUFF_LEN 1024
#define	S_LEN	 100



typedef void(*fnP)();//funcion pointer

//int cell=sizeof(int);
typedef struct Word_
{
	struct Word_ *next; 
	char *name;
	fnP fn;                //核心字
	struct Word_ **wplist;//word类型指针数组(指针的指针),扩展字用
//    int num;              //变量字中保存值
} Word;


static int DS[S_LEN], RS[S_LEN], TS[10];//(data return temp)stack
static int *DP, *RP, *TP;//stack pointer
static Word  **IP;//Word类型指针数组(指针的指针)


//缓存区
char cmdstr[BUFF_LEN];
Word *IP_list[BUFF_LEN/4];   //Word类型指针数组，长度为cmdstr_len/4
Word **IP_list_p=IP_list;        //Word类型指针，指向IP_list[0]

void showDS()
{
	printf("DS> ");
	int *i=DS;
	for (;i<=DP ;i++ )
	{
		printf("%d ",*i);
	}printf("\n");
}

//STACK OPERATE 
void bye()	 {exit(0);}//结束程序
void push(){IP++;DP++;*DP=(int)*IP;}
void dup(){DP++;*DP=*(DP-1);if (DEBUG) printf("[DEBUG]进入dup\n");}
void swap(){int t=*DP; *DP=*(DP-1); *(DP-1)=t;}
void over(){*(DP+1)=*(DP-1);DP++;}
void drop(){DP--;}
//RS
void tor()	{RP++;*RP=*DP;DP--;}
void rto()	{DP++;*DP=*RP;RP--;}
void rat()	{DP++;*DP=*RP;}
//TS
void tot()	{TP++;*TP=*DP;DP--;}
void tto()	{DP++;*DP=*TP;TP--;}
void tat()	{DP++;*DP=*TP;}
//加减乘除
void add(){ *(DP-1)=*(DP-1)+(*DP); DP--;}
void sub(){ *(DP-1)=*(DP-1)-(*DP); DP--;}
void mul(){ *(DP-1)=*(DP-1)*(*DP); DP--;}
void divv(){ *(DP-1)=*(DP-1)/(*DP); DP--;}

//;
void ret()	{IP=(Word**)*RP;	RP--;if (DEBUG) printf("[DEBUG]进入ret\n");}

//if else then语句
void iff() {if(*DP==0){IP = IP + (int)(*(IP+1));}else{IP++;} DP--;}
void elsee() {IP = IP + (int)(*(IP+1));}
void then() {;}


//for next 循环
void forr() {if(*DP<1){IP = IP + (int)(*(IP+1)); DP--;}else{IP++; (*DP)--; tot();}}
void next() {IP = IP - (int)(*(IP+1)); tto();}

//变量存入和取出
void var() {;}
void invar() {;}
void outvar() {;}

Word *dict_head, *pushh;
Word * code(char*s, fnP  fp)
{
	Word *w=(Word*)malloc(sizeof(Word));
	w->next=dict_head;
	dict_head=w;
	w->fn=fp;
	w->wplist=NULL;
//    w->num = NULL;  //实现变量
/*暂定
	w->name=(char*)malloc(strlen(s)+1);
	strcpy(w->name,s);
/*/
	w->name=s;
//*/
	return w;
}

void dolist()
{
	RP++;
	*RP=(int)IP;
	IP=(*IP)->wplist-1;
	if (DEBUG) printf("[DEBUG]进入dolist\n");
}

void colon(char*s)
{
	Word *w=(Word*)malloc(sizeof(Word));
	w->next=dict_head;
	dict_head=w;
	w->fn=dolist;

	w->name=(char*)malloc(strlen(s)+1);
	strcpy(w->name,s);

	int n=(int)IP_list_p-(int)IP_list;
	w->wplist=(Word**)malloc(n);
	memcpy(w->wplist,IP_list, n);
    
    
}


int is_blankchar(char c)	{ return (c==' ' || c=='\t' || c=='\n' ); }


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

int find_Word(char *w)
{
	Word * dict=dict_head;
	while (strcmp(dict->name,w))
	{  
		dict=dict->next;
		if(dict==NULL)    //字典链表搜索不到名字后执行
		{
			if (!is_num(w))	{
                    return 0; //如果不是数字，返回0
            }
            else {               //如果是数字
                if (DEBUG)	printf("[DEBUG]成功找到数字%s\n",w);
                *IP_list_p=pushh;	  //将push核心字指针存入IP_list_p数组        
                IP_list_p++;        //数组指针指向下一个位置
                *IP_list_p=(Word*)atoi(w);	//！！！
                IP_list_p++;

                return 1;
            }			
		}
	}
    
    
    if(!strcmp("if",w))
    {
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        IP_list_p++;
        if_p = IP_list_p;
        IP_list_p++;
    }
    else if(!strcmp("else",w)) 
    {
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;  
        IP_list_p++;
        else_p = IP_list_p;
        *if_p = (Word*)(else_p - if_p + 1);  //+1的意思是越过else字和后面的then偏移量位置
        IP_list_p++;
    }
    else if(!strcmp("then",w))
    {
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
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
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;
        IP_list_p++;
        for_p = IP_list_p;
        IP_list_p++;
        
    }
    else if(!strcmp("next",w))
    {
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
        *IP_list_p=dict;  
        IP_list_p++;
        *for_p = (Word*)(IP_list_p - for_p + 1); 
        *IP_list_p = (Word*)(IP_list_p - for_p + 1); 
        IP_list_p++;
    }
    else if(!strcmp("variable",w))
    {
    }
    else if(!strcmp("!",w)) //未完成
    {
    }
    else if(!strcmp("@",w)) //未完成
    {
    }
    else 
    {
        if (DEBUG)	printf("[DEBUG]成功找到%s字\n",w);
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
	//DS[0]=123;DS[1]=456;
    //DP=DS+1;
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
	s=ignore_blankchar( s);
	
	char *name=NULL;
	if (*s==':')
	{
		s++;
		name=ignore_blankchar(s);
		s=name;
		s=split_Word(s);
	}
	char *w;
    char *w_after;
	while (*s!=0)
	{
		s=ignore_blankchar(s);
		w=s;
		s=split_Word(s);
        w_after = ignore_blankchar(s);

		if(!find_Word(w) )
		{//判断后面一个字是否为！或者@  //未完成
            if((*w_after!=0 && *w_after == '!') || (*w_after!=0 && *w_after == '@'))
            {
                printf("后面一个字是%s\n", w_after);
            }
			else
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

//	printf("%s\n",name);
    if (name!=NULL)
		colon(name);
	else
		explain();

    
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
	

	pushh=code("push",push);
	code("bye",bye);
	code("dup",dup);
	code("swap",swap);
	code("over",over);
	code("drop",drop);
	
	code(">r",tor);
	code("r>",rto);
	code("r@",rat);

	code(">t",tot);
	code("t>",tto);
	code("t@",tat);

	code("+",add);
	code("-",sub);
	code("*",mul);
	code("/",divv);


	code("ret",ret);
	code(";",ret);
	code("dolist",dolist);
    code("if",iff);
    code("else",elsee);
    code("then",then);
    code("for",forr);
    code("next",next);
    code("variable", var);
    code("!", invar);
    code("@", outvar);

/*
	fnP x[]={dup,dup,ret,0};
	colon("x",x);

	fnP y[]={dict_head->wplist,0};colon("y",y);
*/	


	while (1)
	{
		printf(">>>");
		gets(cmdstr);
	//	printf("%s\n",cmdstr);
		compile(cmdstr);
	}
}
