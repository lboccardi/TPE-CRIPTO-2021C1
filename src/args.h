#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

typedef enum operation {
    DISTRIBUTE,
    RECOVER
}operation;
typedef struct files{
    char * input;
    char * output;
    int k;
    operation operation;
    bool verbose;
}files;
#define ARGS_COUNT 5

extern struct files files_info ;

int check_arguments(int argc, char **argv);
#endif