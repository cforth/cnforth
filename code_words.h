/**************************** code_words.h ******************************
 * ds[256] ==> DataStack						* 
 * rs[256] ==> ReturnStack						*
 ************************************************************************/
int ds[256],rs[256];
int *dsp = ds, *rsp = rs;

/* Length of DS */
int dsLen()
{
	return dsp - ds;
}

/* Length of RS */
int rsLen()
{
	return rsp - rs;
}

/* Warning of stack overflow, clean DS */
void stackOver()
{
	dsp = ds;
	printf("Stack underflower!\n");
	return;
}

/* Clean contents of DS */
void clean_ds()
{
	dsp = ds;
	return;
}
 
/* Push into DS */
void push(int num)
{ 
	dsp++;
	*dsp = num;
	return;
} 

/* >r  Pop from the top of DS, push into RS */
void tor()
{
	if(dsLen() < 1){
		stackOver();
		return ;
	} 
	rsp++;
	*rsp = *dsp;
	dsp--;
	return;
} 

/* r>  Pop from the top of RS, push into DS */
void rto()
{ 
	if(rsLen() < 1){
		stackOver();
		return;
	} 
	dsp++; 
	*dsp = *rsp; 
	rsp--; 
	return;
} 

/* drop  Drop the top of DS */
void drop()
{
	if(dsLen() < 1){
		stackOver();
		return;
	} 
	dsp--; 
	return;
} 

/* dup  Dup the top of DS, push into DS */
void dup()
{
	if(dsLen() < 1) {
		stackOver();
		return;
	} 
	dsp++; 
	*dsp = *(dsp-1); 
	return;
} 

/* swap  Swap of the first two top of DS */
void swap()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	int tmp; 
	tmp = *dsp;
	*dsp = *(dsp-1);
	*(dsp-1) = tmp;
	return;
} 

/* + 将DS栈顶前两个数相加  */
void add()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	*(dsp-1) += *dsp; 
	dsp--; 
	return;
} 

/* - 将DS次栈顶数减DS栈顶数  */
void sub()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	*(dsp-1) -= *dsp; 
	dsp--;
	return;
} 

/* * 将DS次栈顶数乘以DS栈顶数  */
void mul()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	*(dsp-1) *= *dsp; 
	dsp--;
	return;
} 

/* /d 将DS次栈顶数除以DS栈顶数，舍弃余数  */
void ddiv()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	}
	*(dsp-1) /= *dsp; 
	dsp--;
	return;
} 

/* % 将DS次栈顶数除以DS栈顶数，只留余数  */
void mod()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	*(dsp-1) %= *dsp; 
	dsp--;
	return;
} 

/* .s 显示数据栈DS  */
void showDS()
{
	printf("[%d] ",dsLen());
	int *tmp;
	for(tmp=ds+1; tmp<=dsp; tmp++)
		printf("%d ",*tmp);
	printf("<DS]\n");
	return;
} 

/* .rs 显示返回栈RS  */
void showRS()
{
	printf("[%d] ",rsLen());
	int *tmp;
	for(tmp=rs+1; tmp<=rsp; tmp++)
		printf("%d ",*tmp);
	printf("<RS]\n");
	return;
} 

/* . 弹出DS栈顶到显示 */
void showTop()
{
	if(dsLen() < 1){
		stackOver();
		return ;
	} 
	printf("%d\n",*dsp);
	dsp--; 
	return;
} 


/* -- */
void sub1()
{
	if(dsLen() < 1){
		stackOver();
		return ;
	}
	*dsp -= 1;
	return;
}

/* ++ */
void add1()
{
	if(dsLen() < 1){
		stackOver();
		return ;
	}
	*dsp += 1;
	return;
}