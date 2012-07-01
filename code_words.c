/*
** code_words.c
** Cforth原语，核心字文件。
*/

#include <stdio.h>
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
** stack_error
*/
void stack_error( void )
{
	dsp = ds;
	rsp = rs;
	printf("Stack underflower!\n");
	return;
}


/*
** clean_ds
*/
void clean_ds( void )
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
		stack_error();
		return ;
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
	printf("[%d] ",ds_len());
	STACK_TYPE *tmp;
	for(tmp=ds+1; tmp<=dsp; tmp++)
		printf("%d ",*tmp);
	printf("<DS]\n");
	return;
} 


/*
** sh_rs
*/
void sh_rs( void )
{
	printf("[%d] ",rs_len());
	STACK_TYPE *tmp;
	for(tmp=rs+1; tmp<=rsp; tmp++)
		printf("%d ",*tmp);
	printf("<RS]\n");
	return;
} 


/*
** pop
*/
void pop( void )
{
	if(ds_len() < 1){
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
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
		stack_error();
		return;
	}
	tmp = *(dsp-2);
	*(dsp-2) = *(dsp-1);
	*(dsp-1) = *dsp;
	*dsp = tmp;
	return;
}
