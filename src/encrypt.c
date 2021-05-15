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

int encryption_distribute(){
    printf("estoy en encrypt distribute\n");
    return EXIT_SUCCESS;
}

int output_distribute(){

    bmp_image * image;
    int length = strlen(crypt_info.args.output) + MAX_PATH_LENGHT + 2;
    char new_file_name[length];


    for (int i = 0; i < crypt_info.args.k; i++) {
        image = &crypt_info.shadows[i];
        sprintf(new_file_name, "%s/%s", crypt_info.args.output, image->filename);
        write_image(new_file_name, image);
    }

    free_all_images();

    printf("estoy en output distribute\n");
    return EXIT_SUCCESS;
}
