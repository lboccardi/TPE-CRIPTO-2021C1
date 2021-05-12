#include "cript.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int input_recover(){
    DIR* FD;
    struct dirent* in_file;
    FILE    *entry_file;
    /* Scanning the in directory */
    printf("%s\n",crypt_info.args.input);
    if (NULL == (FD = opendir (crypt_info.args.input))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 1;
    }
    int dirLen = strlen(crypt_info.args.input)+2;
    int file_count = 0;
    while ((in_file = readdir(FD))) 
    {
        if(file_count == crypt_info.args.k){
            fprintf(stderr,"La cantidad de archivos en el directorio es incorrecta.\n");
            fprintf(stderr,"Se esperaban %d.\n",crypt_info.args.k);
            closedir(FD);
            return EXIT_FAILURE;
        }
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        
        dirLen += strlen(in_file->d_name)+strlen("/");
        char path[dirLen];
        path[0]=0;
        strcat(path,crypt_info.args.input);
        strcat(path,"/");
        strcat(path,in_file->d_name);

         if ((entry_file = fopen(path, "rb")) == NULL){
            fprintf(stderr,"error al abrir el archivo %s\n",in_file->d_name);
            closedir(FD);
            return EXIT_FAILURE;
        }
        uint8_t data[HEADER_SIZE];
        if (fread(&data, HEADER_SIZE, 1, entry_file) < 1){
            /* Couldn't read the file header - return NULL... */
            fclose(entry_file);
            closedir(FD);
            fprintf(stderr,"error al leer el archivo %s\n",in_file->d_name);
            return EXIT_FAILURE;
        }
        if(parse(&crypt_info.shadows[file_count++],data) == ERROR){
            exit(EXIT_FAILURE);
        }
        if(crypt_info.args.verbose){
            printf("\nArchivo leído: %s\n",in_file->d_name);
            printHeaderInfo(&crypt_info.shadows[file_count-1]);
        }
        fclose(entry_file);
    }
    closedir(FD);

    if(crypt_info.args.k != file_count){
         fprintf(stderr,"La cantidad de archivos en el directorio es incorrecta.\n");
            fprintf(stderr,"Se esperaban %d.\n",crypt_info.args.k);
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int decryption_recover(){
    printf("estoy en encrypt recover\n");
    return EXIT_SUCCESS;
}
int output_recover(){
    printf("estoy en output recover\n");
    return EXIT_SUCCESS;
}