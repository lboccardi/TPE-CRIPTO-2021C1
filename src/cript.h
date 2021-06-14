#ifndef CRIPT_H
#define CRIPT_H
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include "codec.h"
#include "galois.h"
#include <dirent.h>

#define MAX_PATH_LENGHT 1024
#define MAX_SHADOWS 100
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

typedef struct secret_image {
    bmp_header header;
    uint8_t * header_data;
    uint8_t * image_data;
    char filename[MAX_PATH_LENGHT];
} secret_image;

typedef struct crypt{
    args_info args;
    functions functions;
    secret_image secret;
    bmp_image shadows[MAX_SHADOWS];
    uint8_t n;         
} crypt;

#define ARGS_COUNT 5

extern struct crypt crypt_info ;

int check_arguments(int argc, char **argv);

int find_images_in_directory (char * path);
int read_image(char * path, int image_index);
int read_image_secret(char * path);
int write_image(char * file_path, bmp_image * image);
int write_secret_image(char * file_path, secret_image * image);

void free_secret_image();
void free_image(int image_index);
void free_all_images();

void get_block_by_index (bmp_image * image, int index, uint8_t * return_array);
void write_block_by_index (bmp_image * image, int index, uint8_t * data);

int input_distribute();
int encryption_distribute();
int output_distribute();

int input_recover();
int decryption_recover();
int output_recover();

void print_header_info(bmp_header * header);

#endif