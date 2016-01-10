#define     CELL        long  //定义数据类型，在32位与64位系统中与指针类型的宽度相同
#define     STACK_LEN   1024  //定义栈的深度
#define     WIDTH       100   //词的名字的最大长度
#define     BUFF_LEN    1024

#define DEBUG 0
#if DEBUG
    #define PRINT(fmt, args...) printf(fmt,##args);
#else
    #define PRINT(fmt, args...)
#endif


//代码域函数指针
typedef void(*fnP)();  


//用结构体定义Forth的词结构，利用链表实现词典
typedef struct Word
{
    struct Word *link;     //Forth词的链接域
    char *name;            //Forth词的名字域
    fnP fn;                //Forth词的代码域指针
    struct Word **wplist;  //Forth词的参数域
} Word;


//定义字典结构
typedef struct Dict
{
    CELL size;
    Word *head;
} Dict;


//Forth系统运行时的核心指针
Dict *forth_dict;            //Forth词典指针
CELL DS[STACK_LEN];          //参数栈
CELL RS[STACK_LEN];          //返回栈
CELL *DP, *RP;               //栈指针
Word *IP_list[BUFF_LEN];     //指令列表，长度为BUFF_LEN/4   
Word **IP;                   //指令列表指针(指针的指针)
Word **IP_head;              //保存指令列表的指针位置
Word *define_p;              //保存在词典中当前定义的扩展词指针，用于支持递归词myself
char next_word[WIDTH] ;      //用来保存一些立即词需要读取的后面的词的名字 


//Forth词的定义函数
Word *code(char*name, fnP  fp);
void dolist();
void change_colon(Word *c, Word **list, int n); //创建扩展词中的wplist
Word *colon(char*name);
Word *constant(char*name, CELL num);
Word *variable(char*name, CELL num);

//Forth词典的接口函数
Dict *dict_init();
int dict_ins_next(Dict *dict, Word *word);
Word *dict_search_name(Dict *dict, char *name);

void explain(); //IP列表执行
int is_num(char *s); //判断字符串是否为数字
int find(char *name, Dict *dict); //根据词名，去执行相应的IP列表操作

//Forth栈操作函数
void empty_stack();
void stack_error(int n);
void ip_push(Word *w);  //IP栈PUSH
void ds_push(CELL n);
void rs_push(CELL n);
CELL ds_pop();
CELL rs_pop();
CELL ds_top();
CELL rs_top();

//Forth核心词
void push();     // push
void popds();    // .
void bye();      // bye

void ret();      // ret

void depth();    // depth
void add();      // +
void sub();      // -
void mul();      // *
void divv();     // /

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
void words();     // words

//Forth核心词中的立即词
void defcolon();   // :
void endcolon();   // ;
void _if();        // if
void _else();      // else
void _then();      // then
void _do();        // do
void _loop();      // loop
void see();        // see
void forget();     // forget
void var();        // variable
void cons();       // constant