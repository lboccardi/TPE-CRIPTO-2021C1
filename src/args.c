#include "args.h"
#include <string.h>
#include <stdio.h>

#define VERBOSE "--verbose"

int check_arguments(int argc, char **argv){

    /** checkeo verbose **/
    files_info.verbose = ((argc > ARGS_COUNT) && (0==strcmp(argv[5],VERBOSE))) ? true : false;

    /** Primer argumento :  r o d **/
    if(0==strcmp(argv[1],"r")){
        files_info.operation=RECOVER;
    }else if(0==strcmp(argv[1],"d")){
        files_info.operation=DISTRIBUTE;
    }else{
        if(files_info.verbose){
            printf("El primer argumento debería ser 'r' o 'd'.\n");
            printf("d: indica que se va a distribuir una imagen secreta en otras imágenes.\n");
            printf("r: indica que se va a recuperar una imagen secreta a partir de otras imágenes.\n");
        }
        return EXIT_FAILURE;
    }

    /** Tercer argumento :  k **/
    int k = atoi(argv[3]);
    if(k<2 || k>8){
        if(files_info.verbose){
            printf("El valor de k debe ser entre 2 y 8.\n");
         }
        return EXIT_FAILURE;
    }
    files_info.k = k;

    /** paths **/
    int img_len = strlen(argv[2])+1;
    int dir_len = strlen(argv[4])+1;
    if(files_info.operation == DISTRIBUTE){
        files_info.input = malloc(img_len);
        strcpy(files_info.input,argv[2]);
        
        files_info.output = malloc(dir_len);
        strcpy(files_info.output,argv[4]);
    }else{
        files_info.output = malloc(img_len);
        strcpy(files_info.output,argv[2]);
        
        files_info.input = malloc(dir_len);
        strcpy(files_info.input,argv[4]);
    }
    if(files_info.verbose){
        printf("Se guardo:\n input %s \noutput %s\n",files_info.input,files_info.output);
    }
    return EXIT_SUCCESS;
}