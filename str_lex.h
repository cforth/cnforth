/*
** str_lex.h
** cforth输入流词法分析模块接口
*/


/*
** STR_NODE
** 链表队列结构体，保存词法分析后的forth词。
*/
typedef struct STR_NODE {
	int type;
	int length;
	char str[20];
	struct STR_NODE *next;
} StrNode;


/*
** create_str
** 创建一个新的队列头部节点。
*/
StrNode *create_str( void );


/*
** add_str
** 在队列中新增一个节点。
*/
void add_str( int t, char *s );


/*
** del_str
** 在队列中删除包含指定字符串的节点。
*/
void del_str( StrNode *head, char *s );


/*
** printf_str
** 按先进先出顺序打印出队列中的所有节点数据。
*/
void printf_str( StrNode *head );


/*
** destroy_str
** 除了头部节点外，释放掉队列中的所有其他节点。
** 使str_prev指向头部节点指针，str_now指向NULL指针。
*/
void destroy_str( StrNode *head );
