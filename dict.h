/*
** dict.h
** 字典接口模块
*/


/*
** add_word
** 再字典链表中添加一个新冒号字。
** n为指向冒号字名字符串的指针。
** d为指向冒号字定义的指针。
*/
void add_word( char *n, char *d );


/*
** find_word
** 搜索字典链表内冒号字的名称。
** 返回该冒号字的定义。
*/
char *find_word(char *n);


/*
** printf_dict
** 打印出字典中所有的冒号字以及定义。
*/
void printf_dict( void );
