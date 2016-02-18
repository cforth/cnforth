#define     CELL        long  //定义数据类型，在32位与64位系统中与指针类型的宽度相同
#define     STACK_LEN   1024  //定义栈的深度
#define     BUFF_LEN    1024  //缓冲区长度
#define     TRUE        1
#define     FALSE       0
#define     EXPLAIN     0     //解释模式标记
#define     COMPILE     1     //编译模式标记
#define     IGNORE      2     //注释模式标记
#define     REVEAL_WORD 0     //标记为显示词
#define     IMMD_WORD   1     //标记为立即词
#define     HIDE_WORD   2     //标记为隐藏词

#define DEBUG 0
#if DEBUG
    #define PRINT(fmt, args...) printf(fmt,##args);
#else
    #define PRINT(fmt, args...)
#endif


//代码域函数指针
typedef void(*fn_p)();  


//用结构体定义Forth的词结构，利用链表实现词典
typedef struct Word
{
    struct Word *link;     //Forth词的链接域
    CELL flag;             //Forth标记数，用来识别立即词、隐藏词
    char *name;            //Forth词的名字域
    fn_p code_p;           //Forth词的代码域
    struct Word **wplist;  //Forth词的参数域
} Word;


//定义字典结构
typedef struct Dict
{
    CELL size;               //Forth词典中词的数量
    Word *head;              //Forth词典链表最后一个词的地址
    Word *create_p;          //保存当前正在定义的扩展词地址
    Word *wplist_tmp[BUFF_LEN];//保存编译模式正在定义的扩展词参数域，临时用
} Dict;


//Forth系统运行时的核心指针
CELL state;                  //Forth状态变量
char forth_text[BUFF_LEN];   //Forth代码文本缓冲区
char *current_text;          //当前Forth词的词首指针
char *text_p;                //Forth代码文本指针
Dict *forth_dict;            //Forth词典指针
CELL DS[STACK_LEN];          //参数栈
CELL RS[STACK_LEN];          //返回栈
CELL *DP, *RP;               //栈指针
Word *IP_list[BUFF_LEN];     //解释模式指令列表，长度为BUFF_LEN  
Word **IP;                   //指令列表指针(指针的指针)
Word **IP_head;              //IPlist选择指针，根据状态变量指向不同的指令列表

//文本解析
int check_blank(char c);  //判断是否为空白字符
char *parse_word();  //返回输入流中当前的forth词，并更新text_p指针

//Forth词的构建函数
Word *create(char*name, fn_p  fp); //创建Forth词的名字域
void does(Word *c, Word **list, int n); //创建Forth词中的参数域
void colon_code();  //扩展词的代码域
void cons_code();   //常数词的代码域
void var_code();    //变量词的代码域

//Forth词典的操作函数
Dict *dict_init();
int dict_ins_next(Dict *dict, Word *word);
Word *dict_search_name(Dict *dict, char *name);
void dict_destroy_word(Word *word);
int dict_rem_after(Dict *dict, char *name);

//Forth指令列表操作函数
void explain(); //IP列表执行
int is_num(char *s); //判断字符串是否为数字
int find(Dict *dict, char *name); //根据词名，去执行相应的IP列表操作

//Forth栈操作函数
void empty_stack();
void stack_error(int n);
void ip_push(Word *w, Word** list);  //IP栈PUSH
void ds_push(CELL n);
void rs_push(CELL n);
CELL ds_pop();
CELL rs_pop();
CELL ds_top();
CELL rs_top();

//Forth核心词
void lit();      // (lit)
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

void doo();        // (do)
void loopp();      // (loop)

void tor();        // >r
void rto();        // r>
void rat();        // r@

void emit();       // emit
void words();      // words

//Forth核心词中的立即词
void immediate();  // immediate
void in_interpret(); // [
void out_interpret();// ]
void myself();     // myself
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
void load();       // load


//Forth解释器部分
void interpret();
int load_file(char *file_path);