/********************** cforth核心词 ************************
 * 定义数组ds与rs，申请一段空间用作数据栈和DS返回栈RS       * 
 * 定义指针*dsp与*rsp，分别指向dS数组与rS数组的首地址       */
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

/* push 将数压入数据栈DS栈顶  */
void push(int a)
{ 
	dsp++;
	*dsp=a;
} 

/* >r 将DS栈顶的数弹出，压入RS栈顶  */
int tor()
{
	if(dsLen() < 1){
		stackOver();
		return 0;
	} 
	rsp++;
	*rsp=*dsp;
	dsp--;
	return 0;
} 

/* r> 将RS栈顶的数弹出，压入DS栈顶  */
int rto()
{ 
	if(rsLen() < 1){
		stackOver();
		return 0;
	} 
	dsp++; 
	*dsp=*rsp; 
	rsp--; 
	return 0;
} 

/* drop 丢弃DS栈顶  */
int drop()
{
	if(dsLen() < 1){
		stackOver();
		return 0;
	} 
	dsp--; 
	return 0;
} 

/* dup 复制DS栈顶数  */
int dup()
{
	if(dsLen() < 1) {
		stackOver();
		return 0;
	} 
	dsp++; 
	*dsp=*(dsp-1); 
	return 0;
} 

/* swap 交换DS栈顶前两个数  */
int swap()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	} 
	int tmp; 
	tmp=*dsp;
	*dsp=*(dsp-1);
	*(dsp-1)=tmp;
	return 0;
} 

/* + 将DS栈顶前两个数相加  */
int add()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	} 
	*(dsp-1)=*(dsp-1) + *dsp; 
	dsp--; 
	return 0;
} 

/* - 将DS次栈顶数减DS栈顶数  */
int sub()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	} 
	*(dsp-1)=*(dsp-1) - *dsp; 
	dsp--;
	return 0;
} 

/* * 将DS次栈顶数乘以DS栈顶数  */
int mul()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	} 
	*(dsp-1)=*(dsp-1) * *dsp; 
	dsp--;
	return 0;
} 

/* /d 将DS次栈顶数除以DS栈顶数，舍弃余数  */
int ddiv()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	}
	*(dsp-1)=*(dsp-1) / *dsp; 
	dsp--;
	return 0;
} 

/* % 将DS次栈顶数除以DS栈顶数，只留余数  */
int mod()
{
	if(dsLen() < 2) {
		stackOver();
		return 0;
	} 
	*(dsp-1)=*(dsp-1) % *dsp; 
	dsp--;
	return 0;
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
} 

/* showTop 显示DS栈顶，对DS栈无影响 */
void showTop()
{
	printf("%d\n",*dsp);
} 

/* ifFlag0 若DS栈顶数为0，则丢弃DS栈顶，用于带余数除法 */
void ifFlag0()
{
	int flag = *dsp;
	if(flag == 0) drop();
}
