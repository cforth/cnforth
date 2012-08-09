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
static	STACK_TYPE	*dsp = ds;
static	STACK_TYPE	*rsp = rs;


/*
** de_len
*/
int ds_len( void )
{
	return dsp - ds;
}


/*
** rs_len
*/
int rs_len( void )
{
	return rsp - rs;
}



/*
** clean_stack
*/
void clean_stack( void )
{
	dsp = ds;
	rsp = rs;
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
	dsp++;
	*dsp = value;
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

	rsp++;
	*rsp = *dsp;
	dsp--;
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

	dsp++; 
	*dsp = *rsp; 
	rsp--; 
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
	dsp--; 
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
	dsp++; 
	*dsp = *(dsp-1); 
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
	tmp = *dsp;
	*dsp = *(dsp-1);
	*(dsp-1) = tmp;
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
	*(dsp-1) += *dsp; 
	dsp--; 
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
	*(dsp-1) -= *dsp; 
	dsp--;
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
	*(dsp-1) *= *dsp; 
	dsp--;
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
	else if(*dsp == 0) {
		error_msg(7);
		clean_stack();
		return;
	}

	*(dsp-1) /= *dsp; 
	dsp--;
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
	else if(*dsp == 0) {
		error_msg(7);
		clean_stack();
		return;
	}

	*(dsp-1) %= *dsp; 
	dsp--;
	return;
} 


/*
** sh_ds
*/
void sh_ds( void )
{
	printf("<%d> ",ds_len());
	STACK_TYPE *tmp;
	for(tmp=ds+1; tmp<=dsp; tmp++)
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
	for(tmp=rs+1; tmp<=rsp; tmp++)
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
	printf("%d\n",*dsp);
	dsp--; 
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
	*dsp -= 1;
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
	*dsp += 1;
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
	dsp++;
	*dsp = *(dsp-2);
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
	tmp = *(dsp-2);
	*(dsp-2) = *(dsp-1);
	*(dsp-1) = *dsp;
	*dsp = tmp;
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
	
	*dsp = 0 - *dsp;
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
	
	if(*dsp != 0)
		*dsp = 1; 
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
