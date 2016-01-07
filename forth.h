#include "words.h"
#include "parse.h"

int is_num(char *s);

int find(char *name, Dict *dict);

void explain();

void interpret(char *s, Dict *dict);

int load_file(char *file_path);