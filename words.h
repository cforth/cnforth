#define     CELL        long  //定义数据类型，在32位与64位系统中与指针类型的宽度相同
#define     STACK_LEN   100   //定义栈的深度

#define DEBUG 1
#if DEBUG
    #define PRINT(fmt, args...) printf(fmt,##args)
#else
    #define PRINT(fmt, args...)
#endif


//函数指针
typedef void(*fnP)();  


//用结构体定义Forth中的基础字，利用链表实现字典
typedef struct Word
{
    struct Word *next;     //指向字典中下个字的指针
    char *name;            //字的名字
    fnP fn;                //字定义，函数指针
    struct Word **wplist;  //扩展字中用的函数指针列表
    CELL num;              //变量字中保存数值
} Word;


//数据栈、返回栈、临时栈
CELL DS[STACK_LEN], RS[STACK_LEN], TS[STACK_LEN];
CELL *DP, *RP, *TP;

//指令指针数组(指针的指针)
Word  **IP;

//核心字、扩展字、变量字的定义函数，执行后加入Forth的字典并返回指向自己的指针
Word *code(char*s, fnP  fp, Word *dict);
void dolist();     //用于创建扩展字中的定义
Word *colon(char*s, Word **list, int n, Word *dict);
Word *variable(char*s, CELL num, Word *list);


//核心字
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