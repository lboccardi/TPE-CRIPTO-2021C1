#include "cript.h"
#include <string.h>
#include <stdio.h>

#define VERBOSE "--verbose"

int check_arguments(int argc, char **argv){

    /** checkeo verbose **/
    crypt_info.files.verbose = ((argc > ARGS_COUNT) && (0==strcmp(argv[5],VERBOSE))) ? true : false;

    /** Primer argumento :  r o d **/
    if(0==strcmp(argv[1],"r")){
        crypt_info.files.operation=RECOVER;
    }else if(0==strcmp(argv[1],"d")){
        crypt_info.files.operation=DISTRIBUTE;
    }else{
            fprintf(stderr,"El primer argumento debería ser 'r' o 'd'.\n");
            fprintf(stderr,"d: indica que se va a distribuir una imagen secreta en otras imágenes.\n");
            fprintf(stderr,"r: indica que se va a recuperar una imagen secreta a partir de otras imágenes.\n");
        return EXIT_FAILURE;
    }

    /** Tercer argumento :  k **/
    int k = atoi(argv[3]);
    if(k<2 || k>8){
            fprintf(stderr,"El valor de k debe ser entre 2 y 8.\n");
        return EXIT_FAILURE;
    }
    crypt_info.files.k = k;

    /** paths **/
    int img_len = strlen(argv[2])+1;
    int dir_len = strlen(argv[4])+1;
    if(crypt_info.files.operation == DISTRIBUTE){
        crypt_info.files.input = malloc(img_len);
        strcpy(crypt_info.files.input,argv[2]);
        
        crypt_info.files.output = malloc(dir_len);
        strcpy(crypt_info.files.output,argv[4]);

        crypt_info.functions.handle_input=input_distribute;
        crypt_info.functions.handle_crypt=encryption_distribute;
        crypt_info.functions.handle_output=output_distribute;
    }else{
        crypt_info.files.output = malloc(img_len);
        strcpy(crypt_info.files.output,argv[2]);
        
        crypt_info.files.input = malloc(dir_len);
        strcpy(crypt_info.files.input,argv[4]);

        crypt_info.functions.handle_input=input_recover;
        crypt_info.functions.handle_crypt=decryption_recover;
        crypt_info.functions.handle_output=output_recover;
    }
    if(crypt_info.files.verbose){
        printf("Se guardaron los paths como:\n input %s \n output %s\n",crypt_info.files.input,crypt_info.files.output);
    }
    return EXIT_SUCCESS;
}