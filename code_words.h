/**************************** code_words.h ******************************
 * ds[256] ==> DataStack						* 
 * rs[256] ==> ReturnStack						*
 ************************************************************************/
int ds[256],rs[256];
int *dsp=ds,*rsp=rs;

/* Length of DS */
int dsLen()
{
	int *tmp=&ds[1];
	int n;
	for(n=0;tmp<=dsp;n++)
		tmp++;
	return n;
}

/* Length of RS */
int rsLen()
{
	int *tmp=&rs[1];
	int n;
	for(n=0;tmp<=rsp;n++)
		tmp++;
	return n;
}

/* 栈底溢出警告 */
void stackOver()
{
	printf("Stack Overflow!\n");
}

/* 清空DS栈 */
void clean_ds()
{
	dsp=&ds[0];
	return;
}
 
/* push 将数压入数据栈DS栈顶  */
void push(int a)
{ 
	dsp++;
	*dsp=a;
	return;
} 

/* >r 将DS栈顶的数弹出，压入RS栈顶  */
void tor()
{
	if(dsLen() < 1){
		stackOver();
		return ;
	} 
	rsp++;
	*rsp=*dsp;
	dsp--;
	return;
} 

/* r> 将RS栈顶的数弹出，压入DS栈顶  */
void rto()
{ 
	if(rsLen() < 1){
		stackOver();
		return;
	} 
	dsp++; 
	*dsp=*rsp; 
	rsp--; 
	return;
} 

/* drop 丢弃DS栈顶  */
void drop()
{
	if(dsLen() < 1){
		stackOver();
		return;
	} 
	dsp--; 
	return;
} 

/* dup 复制DS栈顶数  */
void dup()
{
	if(dsLen() < 1) {
		stackOver();
		return;
	} 
	dsp++; 
	*dsp=*(dsp-1); 
	return;
} 

/* swap 交换DS栈顶前两个数  */
void swap()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	int tmp; 
	tmp=*dsp;
	*dsp=*(dsp-1);
	*(dsp-1)=tmp;
	return;
} 

/* + 将DS栈顶前两个数相加  */
void add()
{
	if(dsLen() < 2) {
		stackOver();
		return;
	} 
	*(dsp-1)=*(dsp-1) + *dsp; 
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
	*(dsp-1)=*(dsp-1) - *dsp; 
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
	*(dsp-1)=*(dsp-1) * *dsp; 
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
	*(dsp-1)=*(dsp-1) / *dsp; 
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
	*(dsp-1)=*(dsp-1) % *dsp; 
	dsp--;
	return;
} 

/* .s 显示数据栈DS  */
void showDS()
{
	printf("[%d] ",dsLen());
	int *tmp=&ds[1];
	while(tmp<=dsp) {
		printf("%d ",*tmp);
        	tmp++;
	}
	printf("<DS]\n");
	return;
} 

/* .rs 显示返回栈RS  */
void showRS()
{
	printf("[%d] ",rsLen());
	int *tmp=&rs[1];
	while(tmp<=rsp) {
		printf("%d ",*tmp);
		tmp++;
	}
	printf("<RS]\n");
	return;
} 

/* showTop 显示DS栈顶，对DS栈无影响 */
void showTop()
{
	printf("%d\n",*dsp);
	return;
} 

/* ifFlag0 若DS栈顶数为0，则丢弃DS栈顶，用于带余数除法 */
void ifFlag0()
{
	int flag = *dsp;
	if(flag == 0) drop();
}