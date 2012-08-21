/*
** code_words.c
** Cforth原语，核心字文件。
*/

#include <stdio.h>
#include <stdlib.h>
#include "code_words.h"

#define	STACK_SIZE	256 /* 堆栈中值数量的最大限制 */

/*
** 定义数据栈和返回栈。
** 定义数据栈指针和返回栈指针。
*/
static	STACK_TYPE	ds[ STACK_SIZE ];
static	STACK_TYPE	rs[ STACK_SIZE ];
static	STACK_TYPE	*ds_p = ds;
static	STACK_TYPE	*rs_p = rs;


/*
** de_len
*/
int ds_len( void )
{
	return ds_p - ds;
}


/*
** rs_len
*/
int rs_len( void )
{
	return rs_p - rs;
}



/*
** clean_stack
*/
void clean_stack( void )
{
	ds_p = ds;
	rs_p = rs;
	return;
}


/*
** push
*/
void push( STACK_TYPE value )
{ 
	if(ds_len() >= STACK_SIZE){
		error_msg(1);
		clean_stack();
		return;
	}
	ds_p++;
	*ds_p = value;
	return;
} 


/*
** tor
*/
void tor( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return;
	} 
	else if(rs_len() >= STACK_SIZE){
		error_msg(3);
		clean_stack();
		return;
	}

	rs_p++;
	*rs_p = *ds_p;
	ds_p--;
	return;
} 


/*
** rto
*/
void rto( void )
{ 
	if(rs_len() < 1){
		error_msg(4);
		clean_stack();
		return;
	} 
	else if(ds_len() >= STACK_SIZE){
		error_msg(1);
		clean_stack();
		return;
	}

	ds_p++; 
	*ds_p = *rs_p; 
	rs_p--; 
	return;
} 


/*
** drop
*/
void drop( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return;
	} 
	ds_p--; 
	return;
} 


/*
** dup
*/
void dup( void )
{
	if(ds_len() < 1) {
		error_msg(2);
		clean_stack();
		return;
	} 
	ds_p++; 
	*ds_p = *(ds_p-1); 
	return;
} 


/*
** swap
*/
void swap( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	} 
	STACK_TYPE tmp; 
	tmp = *ds_p;
	*ds_p = *(ds_p-1);
	*(ds_p-1) = tmp;
	return;
} 


/*
** add
*/
void add( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	} 
	*(ds_p-1) += *ds_p; 
	ds_p--; 
	return;
} 


/*
** sub
*/
void sub( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	} 
	*(ds_p-1) -= *ds_p; 
	ds_p--;
	return;
} 


/*
** mul
*/
void mul( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	} 
	*(ds_p-1) *= *ds_p; 
	ds_p--;
	return;
} 


/*
** div_new
*/
void div_new( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	}
	else if(*ds_p == 0) {
		error_msg(7);
		clean_stack();
		return;
	}

	*(ds_p-1) /= *ds_p; 
	ds_p--;
	return;
} 


/*
** mod
*/
void mod( void )
{
	if(ds_len() < 2) {
		error_msg(5);
		clean_stack();
		return;
	} 
	else if(*ds_p == 0) {
		error_msg(7);
		clean_stack();
		return;
	}

	*(ds_p-1) %= *ds_p; 
	ds_p--;
	return;
} 


/*
** sh_ds
*/
void sh_ds( void )
{
	printf("<%d> ",ds_len());
	STACK_TYPE *tmp;
	for(tmp=ds+1; tmp<=ds_p; tmp++)
		printf("%d ",*tmp);
	printf("\n");
	return;
} 


/*
** sh_rs
*/
void sh_rs( void )
{
	printf("<%d> ",rs_len());
	STACK_TYPE *tmp;
	for(tmp=rs+1; tmp<=rs_p; tmp++)
		printf("%d ",*tmp);
	printf("[RS]\n");
	return;
} 


/*
** pop
*/
void pop( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return ;
	} 
	printf("%d\n",*ds_p);
	ds_p--; 
	return;
} 


/*
** sub1
*/
void sub1( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return ;
	}
	*ds_p -= 1;
	return;
}


/*
** add1
*/
void add1( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return ;
	}
	*ds_p += 1;
	return;
}


/*
** over
*/
void over( void )
{
	if(ds_len() < 2){
		error_msg(5);
		clean_stack();
		return;
	}
	ds_p++;
	*ds_p = *(ds_p-2);
	return;
}


/*
** rot
*/
void rot( void )
{
	STACK_TYPE tmp;
	if(ds_len() <3){
		error_msg(6);
		clean_stack();
		return;
	}
	tmp = *(ds_p-2);
	*(ds_p-2) = *(ds_p-1);
	*(ds_p-1) = *ds_p;
	*ds_p = tmp;
	return;
}


/*
** bye
*/
void bye( void )
{
	exit( 0 );
}


/*
** negate 
*/
void negate( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return ;
	}
	
	*ds_p = 0 - *ds_p;
	return;
}


/*
** bool1 
*/
void bool1( void )
{
	if(ds_len() < 1){
		error_msg(2);
		clean_stack();
		return ;
	}
	
	if(*ds_p != 0)
		*ds_p = 1; 
	return;
}


/*
** error_msg
*/
void error_msg( int id )
{
	switch(id){
	case 1:
		dprintf(("Stack overflow! Data_stack is full!\n"));
		break;
	case 2:
		dprintf(("Stack underflow! Data_stack is empty!\n"));
		break;
	case 3:
		dprintf(("Stack overflow! Return_stack is full!\n"));
		break;
	case 4:	
		dprintf(("Stack underflow! Return_stack is empty!\n"));
		break;
	case 5:
		dprintf(("Stack underflow! There is not two values in data_stack!\n"));
		break;
	case 6:
		dprintf(("Stack underflow! There is not three values in data_stack!\n"));
		break;
	case 7:
		dprintf(("Error! Divisor is 0!\n"));
		break;
	}
	return;
}
