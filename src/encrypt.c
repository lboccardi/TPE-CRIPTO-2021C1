#include "cript.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>

int input_distribute(){

    if (read_image(crypt_info.args.input, -1) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (find_images_in_directory(crypt_info.args.output) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int find_images_in_directory (char * path){
    DIR * dir;
    struct dirent * dirp;
    int read_images = 0;

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error reading directory %s\n", path);
        return EXIT_FAILURE;
    }

    char cwd[MAX_PATH_LENGHT];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "Error accessing current working directory %s\n", path);
        return EXIT_FAILURE;
    }

    chdir(path);

    while((dirp = readdir(dir))!=NULL) {
        if(dirp->d_type == 8) {
            if(crypt_info.args.verbose) {
                printf("Analyzing if file \"%s\" is suitable for being a shadow\n", dirp->d_name);
            }
            if (read_image(dirp->d_name, read_images) == EXIT_SUCCESS) {
                read_images++;
            };
        }
    }

    chdir(cwd);
    closedir(dir);

    if (read_images < crypt_info.args.k) {
        fprintf(stderr, "There are not enough shadow images in the given directory\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int read_image(char * path, int image_index) {

    FILE * fp;
    
    if ((fp = fopen(path, "rb")) == NULL){
        fprintf(stderr,"Error al abrir %s\n", path);
        return EXIT_FAILURE;
    }
    
    uint8_t header_data[HEADER_SIZE];

    if (fread(&header_data, HEADER_SIZE, 1, fp) < 1){
        fclose(fp);
        fprintf(stderr,"error al leer\n");
        return EXIT_FAILURE;
    }

    if (image_index < 0) {
        if(parse(&crypt_info.secret,header_data) == ERROR){
            return EXIT_FAILURE;
        }
        if(crypt_info.args.verbose) {
            printHeaderInfo(&crypt_info.secret);
        }
    } else {
        if(parse(&crypt_info.shadows[image_index],header_data) == ERROR){
            return EXIT_FAILURE;
        }
        if(crypt_info.args.verbose) {
            printHeaderInfo(&crypt_info.shadows[image_index]);
        }
    }

    return EXIT_SUCCESS;
}

int encryption_distribute(){
    printf("estoy en encrypt distribute\n");
    return EXIT_SUCCESS;
}
int output_distribute(){
        // FILE * fp;

        // if ((fp = fopen(output_path, "wb")) == NULL){
        //     fprintf(stderr,"Error al escribir archivo %s\n", output_path);
        //     return EXIT_FAILURE;
        // }

    printf("estoy en output distribute\n");
    return EXIT_SUCCESS;
}
