#define     CELL        long  //定义数据类型，在32位与64位系统中与指针类型的宽度相同
#define     STACK_LEN   100   //定义栈的深度

#define DEBUG 0
#if DEBUG
    #define PRINT(fmt, args...) printf(fmt,##args);
#else
    #define PRINT(fmt, args...)
#endif


//函数指针
typedef void(*fnP)();  


//用结构体定义Forth中的基础词，利用链表实现词典
typedef struct Word
{
    struct Word *next;     //指向词典中下个词的指针
    char *name;            //词的名字
    fnP fn;                //核心词定义，函数指针
    struct Word **wplist;  //扩展词中用的函数指针列表
    CELL num;              //变量词中保存数值
    char *str;             //保存扩展词的字符串定义
} Word;


//定义字典结构
typedef struct Dict
{
    int size;
    Word *head;
} Dict;


//数据栈、返回栈、临时栈
CELL DS[STACK_LEN], RS[STACK_LEN], TS[STACK_LEN];
CELL *DP, *RP, *TP;

//指令指针数组(指针的指针)
Word  **IP;

//核心词、扩展词、变量词的定义函数，返回指向自己的指针
Word *code(char*name, fnP  fp);
void dolist();     //用于创建扩展词中的定义
void change_colon(Word *c, Word **list, int n); //修改扩展词中的wplist
Word *colon(char*name, char*str, Word **list, int n);
Word *variable(char*name,  char*str, CELL num);

//词典接口定义
Dict *dict_init();
int dict_ins_next(Dict *dict, Word *word);
Word *dict_search_name(Dict *dict, char *name);

//清空三个栈
void empty_stack();

//Forth栈操作词
void ds_push(CELL n);
void rs_push(CELL n);
CELL ds_pop();
CELL rs_pop();

//核心词
void push();     // push
void popds();    // .
void bye();      // bye

void ret();       // ;
void putcr();    // cr

void depth();    // depth
void add();      // +
void sub();      // -
void mul();      // *
void divv();     // /

void dup();      // dup
void swap();     // swap
void over();     // over
void drop();     // drop
void showds();   // .s
void pick();     // pick
void roll();     // roll

void invar();     // !
void outvar();    // @

void equal();     // =
void morethan();  // >
void lessthan();  // <

void iff();       // if
void elsee();     // else
void then();      // then

void doo();       // do
void loop();      // loop

void tor();      // >r
void rto();      // r>
void rat();      // r@
void tot();      // >t
void tto();      // t>
void tat();      // t@

void emit();      // emit

void myself();    // myself
