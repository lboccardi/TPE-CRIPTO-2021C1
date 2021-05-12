#include "cript.h"
#include <stdio.h>

BMPHeader header;
int input_distribute(){
        FILE *fp;          /* Open file pointer */
        if ((fp = fopen(crypt_info.files.input, "rb")) == NULL){
            fprintf(stderr,"error al abrir\n");
            return EXIT_FAILURE;
        }
        uint8_t data[HEADER_SIZE];
        if (fread(&data, HEADER_SIZE, 1, fp) < 1){
            /* Couldn't read the file header - return NULL... */
            fclose(fp);
            fprintf(stderr,"error al leer\n");
            return EXIT_FAILURE;
        }
        
        if(parse(&header,data) == ERROR){
            exit(EXIT_FAILURE);
        }
        if(crypt_info.files.verbose){
            printHeaderInfo(&header);
        } 
    return EXIT_SUCCESS;
}
int encryption_distribute(){
    printf("estoy en encrypt distribute\n");
    return EXIT_SUCCESS;
}
int output_distribute(){
        printf("estoy en output distribute\n");
        return EXIT_SUCCESS;
}
