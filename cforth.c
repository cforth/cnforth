/********************** cforth主程序 ****************************/
/*版本号:	0.1.6						*/
/*开发人员：ear & xiaohao                                   	*/
/*现有功能：模拟forth系统的运行，可以正确的进行堆栈操作     	*/
/*缺失功能：冒号定义字功能，将在0.2.0版开发			*/
/*欢迎Forth系统爱好者加入，群号：12781666       	        */
/*cforth版权遵从GPL开源协议，欢迎大家继续开发               	*/
/****************************************************************/     
#include <stdio.h>
#include <string.h>
#include "code_words.h"
#include "colon_words.h" 

/*定义数组inputBuff，申请一段空间用以存放键盘数入的字符串	*/
/*定义数组strBuff，用以存放字符串分解后的每一个cforth词		*/ 
char inputBuff[1024]; 
char strBuff[30]; 

int main()
{
    printf("cforth 0.1.6, Copyright (C) 2008-2012 Free Software Foundation, Inc.\n");
    printf("cforth comes with ABSOLUTELY NO WARRANTY; enjoy it and have a good time!\n");
    printf("Type 'bye' to exit\n");
    
    /*cforth主循环，从键盘接受输入的命令，调用对应的子程序并执行*/ 
    while (1){
        int i=0;
        int j=0;
        int k;
        printf(">>>");
        gets(inputBuff); /*读取键盘输入的一行命令，存入inputBuff*/ 

        /*以下一小段代码，用来解决必须在每行命令后加一个空格的缺陷 */
        for(k=0;inputBuff[k]!='\0';k++);/*取得inputBuff字符串的长度*/ 
        inputBuff[k]=' '; /*将inputBuff字符串结尾的'\0'换成一个空格*/ 
        inputBuff[k+1]='\0'; /*在字符串末尾空格后添一个'\0'*/

        /*分解键盘输入的字符串，并执行每个cforth词，直到遇到inputBuff字符串的结尾'\0'*/ 
        while(inputBuff[j] != '\0') {
            if(inputBuff[j]!= ' ') { 
				strBuff[i] = inputBuff[j]; 
				i++;
				j++;
            }
            
            else {
				strBuff[i]='\0';
				if( isNum() ) chgNum();
				else if( !strcmp(".s",strBuff) )	showDS();
				else if( !strcmp(".rs",strBuff) )	showRS();
				else if( !strcmp(".",strBuff) )		showtopDS();
				else if( !strcmp("swap",strBuff))	swap();
				else if( !strcmp(">r",strBuff) )	tor();
				else if( !strcmp("r>",strBuff) )	rto();
				else if( !strcmp("dup",strBuff) )	dup();
				else if( !strcmp("drop",strBuff) )	drop();
				else if( !strcmp("2drop",strBuff) )	drop2();
				else if( !strcmp("2dup",strBuff) )	dup2();
				else if( !strcmp("over",strBuff) )	over();
				else if( !strcmp("+",strBuff) )		add();
				else if( !strcmp("-",strBuff) )		sub();
				else if( !strcmp("*",strBuff) )		mul();
				else if( !strcmp("/d",strBuff) )	ddiv();
				else if( !strcmp("/",strBuff) )		div();
				else if( !strcmp("%",strBuff) )		mod();
				else if( !strcmp("say",strBuff) )	say();
				else if( !strcmp("bye",strBuff) )	return 0;
				else printf("\n[%s]?\n",strBuff);
          
				for(i=0;i<30;i++) strBuff[i]='\0';
                i=0;
                j++;
            }

        }
        printf("OK\n");
     }
    return 0;
} 


/*判断键盘输入流中是否有数字 */
int isNum() 
{
    char *str=strBuff;
    while(*str) {
        if(*str<'0' || *str>'9') return 0;
        str++;
    }
        return 1;
} 


/*转换键盘输入流中的数字，并压入数据栈DS */
int chgNum() 
{
    char *str=strBuff;
    int sum=0;
    while(*str) {
        sum=10*sum+(int)(*str-'0');
        str++;
    } 
    /*如果这个数字字符串只有一个'\0'，则直接返回0 */
    /*用来解决多余的空格键误操作DS的BUG		  */
    if(str==&strBuff[0]) return 0; 
    push(sum);
    return 0;
} 


/*接收键盘输入的字符串，并打印到屏幕 */
int say() 
{
    gets(strBuff);
    printf("cforth: [%s]\n",strBuff);
    return 0;
}