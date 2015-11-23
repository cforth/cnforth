#define    STACK_LEN     100
#define    DEBUG     1
typedef void(*fnP)();  

//int cell=sizeof(int);
typedef struct Word
{
    struct Word *next; 
    char *name;
    fnP fn;                //核心字
    struct Word **wplist;//word类型指针数组(指针的指针),扩展字用
    int num;              //变量字中保存值
} Word;


int DS[STACK_LEN], RS[STACK_LEN], TS[STACK_LEN];//(data return temp)stack
int *DP, *RP, *TP;//stack pointer
Word  **IP;//Word类型指针数组(指针的指针)

Word *code(char*s, fnP  fp, Word *dict);
void dolist();
Word *colon(char*s, Word **list, int n, Word *dict);
Word *variable(char*s, int num, Word *list);

void bye();
void push();
void dup();
void swap();
void over();
void drop();
//RS
void tor();
void rto();
void rat();
//TS
void tot();
void tto();
void tat();
//加减乘除
void add();
void sub();
void mul();
void divv();

//;
void ret();

//if else then语句
void iff();
void elsee();
void then();


//for next 循环
void forr();
void next();

//变量存入和取出
void invar();
void outvar();