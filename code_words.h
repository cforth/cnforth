/*
** code_words.h
** 核心字模块的接口
*/

#define	DEBUG		1 /* 开启DEBUG模式 */
#define STACK_TYPE	int /*堆栈所存储的值的类型*/

#if	DEBUG
#	define dprintf(args) printf args
#else
#	define dprintf(args)
#endif


/*
** ds_len
** 返回数据栈中值的数量 
*/
int ds_len( void );


/*
** rs_len
** 返回返回栈中值的数量
*/
int rs_len( void );


/*
** clean_stack
** 清空数据栈和返回栈
*/
void clean_stack( void );


/*
** push
** 把一个新值压入到数据栈中。参数是被压入的值。
*/
void push( STACK_TYPE value );


/*
** tor
** 弹出数据栈顶值，将值压入返回栈。
*/
void tor( void );


/*
** rto
** 弹出返回栈顶值，将值压入数据栈。
*/
void rto( void );


/*
** drop
** 弹出数据栈顶值，并丢弃掉。
*/
void drop( void );


/*
** dup
** 复制数据栈顶值，并压入数据栈。
*/
void dup( void );


/*
** swap
** 交换数据栈栈顶和次栈顶位置的值。
*/
void swap( void );


/*
** add
** 弹出数据栈栈顶和次栈顶位置的值并相加。
** 结果压入数据栈。
*/
void add( void );


/*
** sub
** 弹出数据栈栈顶和次栈顶位置的值。
** 栈顶值最为减数，次栈顶值作为被减数。
** 相减后将结果压入数据栈。
*/
void sub( void );


/*
** mul
** 弹出数据栈栈顶和次栈顶位置的值。
** 相乘后将结果压入数据栈。
*/
void mul( void );


/*
** div_new
** 弹出数据栈栈顶和次栈顶位置的值。
** 栈顶值最为除数，次栈顶值作为被除数。
** 相除后将结果压入数据栈。
** */
void div_new( void );


/*
** mod
** 弹出数据栈栈顶和次栈顶位置的值。
** 栈顶值最为除数，次栈顶值作为被除数。
** 相除后将余数压入数据栈。
*/
void mod( void );


/*
** sh_ds
** 显示数据栈中的值，不影响数据栈。
*/
void sh_ds( void );


/*
** sh_rs
** 显示返回栈中的值，不影响返回栈。
*/
void sh_rs( void );


/*
** pop
** 弹出数据栈栈顶，并显示到输出。
*/
void pop( void );


/*
** sub1
** 将数据栈栈顶中的值减1。
*/
void sub1( void );


/*
** add1
** 将数据栈栈顶中的值加1。
*/
void add1( void );


/*
** over
** Forth中over的效果。
*/
void over( void );

/*
** rot
** Forth中rot的效果。
*/
void rot( void );
