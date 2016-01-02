#define     CELL        long  //定义数据类型，在32位与64位系统中与指针类型的宽度相同
#define     STACK_LEN   100   //定义栈的深度
#define     WIDTH       100   //词的名字的最大长度
#define     BUFF_LEN    1024

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


//Forth系统运行时的核心指针
Dict *forth_dict;            //Forth词典指针
CELL DS[STACK_LEN];          //参数栈
CELL RS[STACK_LEN];          //返回栈
CELL *DP, *RP;               //栈指针
Word *IP_list[BUFF_LEN/4];   //指令列表，长度为BUFF_LEN/4   
Word  **IP;                  //指令列表指针(指针的指针)
char next_word[WIDTH] ;      //用来保存一些立即词需要读取的后面的词的名字 


//Forth词的定义函数
Word *code(char*name, fnP  fp);
void dolist();                //用于创建扩展词中的定义
void change_colon(Word *c, Word **list, int n); //修改扩展词中的wplist
Word *colon(char*name, char*str, Word **list, int n);
Word *constant(char*name, Word **list);
Word *variable(char*name, CELL num);

//Forth词典的接口函数
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

//Forth核心词
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
void noequal();   // <>
void morethan();  // >
void lessthan();  // <

void if_branch();  // ?branch
void branch();     // branch

void __do();       // (do)
void __loop();     // (loop)

void tor();      // >r
void rto();      // r>
void rat();      // r@

void emit();      // emit

void myself();    // myself

//Forth核心词中的立即词
void _if();        // if
void _else();      // else
void _then();      // then
void _do();        // do
void _loop();      // loop
void see();        // see
void forget();     // forget
void var();        // variable
void cons();       // constant