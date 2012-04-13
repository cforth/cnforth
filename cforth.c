/********************** cforth... ************************/
/*....	0.1.7 											*/
/*.....ear & xiaohao									*/
/*.......forth.................		*/
/*...............0.2.0...					*/
/*..Forth...........12781666					*/
/*cforth....GPL.............				*/
/************************************************************/	 
#include <stdio.h>
#include <string.h>
#include "code_words.h"
#include "colon_words.h" 

#define STRCMP(a, R, b) (strcmp(a, b) R 0)

/*....inputBuff...................    */
/*....strBuff...............cforth.      */ 
char inputBuff[1024]; 
char strBuff[100]; 

int main()
{
	printf("cforth 0.1.7, Copyright (C) 2008-2012 Free Software Foundation, Inc.\n");
    printf("cforth comes with ABSOLUTELY NO WARRANTY; enjoy it and have a good time!\n");
    printf("Type 'bye' to exit\n");
    
    /*cforth..........................*/ 
    while (1){
		int i=0;
        int j=0;
        int k;
        printf(">>>");
		gets(inputBuff);       //..............inputBuff 

        /*............................ */
        for(k=0;inputBuff[k]!='\0';k++);//..inputBuff...... 
        inputBuff[k]=' '; //.inputBuff......'\0'...... 
        inputBuff[k+1]='\0'; //............'\0' 

        /*................cforth......inputBuff......'\0'*/ 
        while(inputBuff[j] != '\0') {
			if(inputBuff[j]!= ' ') {    
            strBuff[i] = inputBuff[j]; 
            i++;
            j++;
            }
            
            else {
            strBuff[i]='\0';
            if( isNum() )							chgNum();
            else if STRCMP(strBuff, ==, ".s")		showDS();
            else if STRCMP(strBuff, ==, ".rs")		showRS();
            else if STRCMP(strBuff, ==, ".")		showtopDS();
	    	else if STRCMP(strBuff, ==, "swap")     swap();
            else if STRCMP(strBuff, ==, ">r")		tor();
            else if STRCMP(strBuff, ==, "r>")		rto();
            else if STRCMP(strBuff, ==, "dup")		dup();
            else if STRCMP(strBuff, ==, "drop")		drop();
            else if STRCMP(strBuff, ==, "2drop")	drop2();
            else if STRCMP(strBuff, ==, "2dup")		dup2();
            else if STRCMP(strBuff, ==, "over")		over();
            else if STRCMP(strBuff, ==, "+")		add();
            else if STRCMP(strBuff, ==, "-")		sub();
            else if STRCMP(strBuff, ==, "*")		mul();
            else if STRCMP(strBuff, ==, "/d")		ddiv();
            else if STRCMP(strBuff, ==, "/")		div();
            else if STRCMP(strBuff, ==, "%")		mod();
            else if STRCMP(strBuff, ==, "say")		say();
            else if STRCMP(strBuff, ==, "bye")		return 0;
            else printf("\n[%s]?\n",strBuff);
          
            for(i=0;i<100;i++) strBuff[i]='\0';
            	i=0;
                j++;
            }

        }
        printf("OK\n");
     }
    return 0;
}

int isNum() 
{
	char *str=strBuff;
    while(*str) {
        if(*str<'0' || *str>'9') return 0;
        str++;
    }
        return 1;
} 


 
int chgNum() 
{
    char *str=strBuff;
    int sum=0;
    while(*str) {
        sum=10*sum+(int)(*str-'0');
        str++;
    }    

    if(str==&strBuff[0]) return 0; 
    push(sum);
    return 0;
} 



int say() 
{
    gets(strBuff);
    printf("cforth: [%s]\n",strBuff);
    return 0;
}
 
