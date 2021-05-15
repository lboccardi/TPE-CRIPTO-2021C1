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

    while((dirp = readdir(dir))!=NULL && read_images < crypt_info.args.k) {
        if(dirp->d_type == 8) {
            if(crypt_info.args.verbose) {
                printf("Analyzing if file \"%s\" is suitable for being a shadow\n", dirp->d_name);
            }
            if (read_image(dirp->d_name, read_images) == EXIT_SUCCESS) {
                strcpy(crypt_info.shadows[read_images].filename, dirp->d_name);
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
    int height, width, offset;
    
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

    bmp_image * image = (image_index < 0) ? &crypt_info.secret : &crypt_info.shadows[image_index];

    if(parse(&image->header,header_data) == ERROR){
        return EXIT_FAILURE;
    }

    if(crypt_info.args.verbose) {
        printHeaderInfo(&image->header);
    }

    height = image->header.height_px;
    width = image->header.width_px;
    offset = image->header.offset;

    image->header_data = malloc(HEADER_SIZE + offset);
    image->image_data = malloc(height * width * sizeof(uint8_t));

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

    if (crypt_info.secret.header_data != NULL ) {
        free(crypt_info.secret.header_data);
        free(crypt_info.secret.image_data);
    }

    for (int i = 0; i < crypt_info.args.k; i++) {
        if (crypt_info.shadows[i].header_data != NULL) {
            free(crypt_info.shadows[i].header_data);
            free(crypt_info.shadows[i].image_data);
        }
    }

    printf("estoy en output distribute\n");
    return EXIT_SUCCESS;
}
