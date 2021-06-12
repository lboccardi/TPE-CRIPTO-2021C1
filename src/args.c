#include "cript.h"

#define VERBOSE "--verbose"

int check_arguments(int argc, char **argv){

    /** checkeo verbose **/
    crypt_info.args.verbose = ((argc > ARGS_COUNT) && (0==strcmp(argv[5],VERBOSE))) ? true : false;

    /** Primer argumento :  r o d **/
    if(0==strcmp(argv[1],"r")){
        crypt_info.args.operation=RECOVER;
    }else if(0==strcmp(argv[1],"d")){
        crypt_info.args.operation=DISTRIBUTE;
    }else{
            fprintf(stderr,"El primer argumento debería ser 'r' o 'd'.\n");
            fprintf(stderr,"d: indica que se va a distribuir una imagen secreta en otras imágenes.\n");
            fprintf(stderr,"r: indica que se va a recuperar una imagen secreta a partir de otras imágenes.\n");
        return EXIT_FAILURE;
    }

    /** Tercer argumento :  k **/
    int k = atoi(argv[3]);
    if(k<4 || k>6){
            fprintf(stderr,"El valor de k debe ser entre 4 y 6.\n");
        return EXIT_FAILURE;
    }
    crypt_info.args.k = k;

    /** paths **/
   if(crypt_info.args.operation == DISTRIBUTE){
        strcpy(crypt_info.args.input,argv[2]);
        
        strcpy(crypt_info.args.output,argv[4]);

        crypt_info.functions.handle_input=input_distribute;
        crypt_info.functions.handle_crypt=encryption_distribute;
        crypt_info.functions.handle_output=output_distribute;
    }else{
        strcpy(crypt_info.args.output,argv[2]);
        
        strcpy(crypt_info.args.input,argv[4]);

        crypt_info.functions.handle_input=input_recover;
        crypt_info.functions.handle_crypt=decryption_recover;
        crypt_info.functions.handle_output=output_recover;
    }
    if(crypt_info.args.verbose){
        printf("Se guardaron los paths como:\n input %s \n output %s\n",crypt_info.args.input,crypt_info.args.output);
    }
    return EXIT_SUCCESS;
}