#ifndef CRIPT_H
#define CRIPT_H
#include <stdlib.h>
#include <errno.h>
#include "codec.h"

#define MAX_PATH_LENGHT 1024
typedef enum operation {
    DISTRIBUTE,
    RECOVER
}operation;
typedef struct args_info{
    char input[MAX_PATH_LENGHT];
    char output[MAX_PATH_LENGHT];
    int k;
    operation operation;
    bool verbose;
}args_info;
typedef struct functions{
    int (*handle_input)      ();
    int (*handle_crypt)      ();
    int (*handle_output)      ();
}functions;

typedef struct bmp_image {
    bmp_header header;
    uint8_t * header_data;
    uint8_t ** image_data;
    char filename[MAX_PATH_LENGHT];
} bmp_image;

typedef struct crypt{
    args_info args;
    functions functions;
    bmp_image secret;
    bmp_image shadows[6];
} crypt;

#define ARGS_COUNT 5

extern struct crypt crypt_info ;

int check_arguments(int argc, char **argv);

int find_images_in_directory (char * path);
int read_image(char * path, int image_index);

int input_distribute();
int encryption_distribute();
int output_distribute();

int input_recover();
int decryption_recover();
int output_recover();

void printHeaderInfo(bmp_header * header);
#endif