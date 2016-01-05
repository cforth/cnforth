#include "words.h"
#include "parse.h"

int is_num(char *s);

int compile(char *name, Dict *dict);

void explain(Word  **IP_head);

void interpret(char *s, Dict *dict);

int load_file(char *file_path);