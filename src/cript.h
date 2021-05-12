#ifndef CRIPT_H
#define CRIPT_H
#include <stdlib.h>
#include <errno.h>
#include "codec.h"

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
typedef struct functions{
    int (*handle_input)      ();
    int (*handle_crypt)      ();
    int (*handle_output)      ();
}functions;
typedef struct crypt{
    files files;
    functions functions;
}crypt;
#define ARGS_COUNT 5

extern struct crypt crypt_info ;

int check_arguments(int argc, char **argv);

int input_distribute();
int encryption_distribute();
int output_distribute();

int input_recover();
int decryption_recover();
int output_recover();

void printHeaderInfo(BMPHeader * header);
#endif