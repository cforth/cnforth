/********************** cforth... ************************/
/* ....ds.rs.............DS...RS       */ 
/* ....*dsp.*rsp.....dS...rS......       */
int ds[256],rs[256];
int *dsp=ds,*rsp=rs;


int dsLen()
{
	int *tmp=&ds[1];
	int n;
	for(n=0;tmp<=dsp;n++)
		tmp++;

	return n;
}


int rsLen()
{
	int *tmp=&rs[1];
	int n;
	for(n=0;tmp<=rsp;n++)
		tmp++;
	return n;
}


void stackOver()
{
	printf("Stack Overflow!\n");
}


void push(int a)
{ 
	dsp++;
	*dsp=a;
} 


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

int drop()
{
	if(dsLen() < 1){
		stackOver();
		return 0;
	} 
	dsp--; 
	return 0;
} 

 
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

void showTop()
{
    printf("%d\n",*dsp);
} 


void ifFlag0()
{
    int flag = *dsp;
    if(flag == 0) drop();
}

