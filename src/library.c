#include "cript.h"

void free_all_images() {
    for (int i = -1; i < crypt_info.n; i++) {
        free_image(i);
    }
}

void free_secret_image(){
    secret_image * image = &crypt_info.secret;
    free(image->header_data);
    free(image->image_data);
}

void free_image(int image_index) {
    if (image_index < 0){
        free_secret_image();
    }else{
        bmp_image * image = &crypt_info.shadows[image_index];
        int height = image->header.height_px;

        free(image->header_data);
        for(int i = 0; i < height; i++) {
            free(image->image_data[i]);
        }
        free(image->image_data);
    }
}

int find_images_in_directory (char * path){
    DIR * dir;
    struct dirent * dirp;

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error abriendo directorio: %s\n", path);
        return EXIT_FAILURE;
    }

    char cwd[MAX_PATH_LENGHT];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "Error accediendo al directorio: %s\n", path);
        return EXIT_FAILURE;
    }

    chdir(path);

    crypt_info.n = 0;

    while((dirp = readdir(dir))!=NULL && crypt_info.n < MAX_SHADOWS) {
        if(dirp->d_type == 8) {
            if(crypt_info.args.verbose) {
                printf("Analizando si el archivo \"%s\" es adecuado para ser una shadow\n", dirp->d_name);
            }
            if (read_image(dirp->d_name, crypt_info.n) == EXIT_SUCCESS) {
                crypt_info.n++;
            };
        }
    }

    chdir(cwd);
    closedir(dir);

    if (crypt_info.n < crypt_info.args.k) {
        fprintf(stderr, "No hay suficientes shadows en el directorio dado\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int read_image(char * path, int image_index) {

    FILE * fp;
    int height, width, offset;
    
    if ((fp = fopen(path, "rb")) == NULL){
        fprintf(stderr,"Error al abrir el archivo: %s\n", path);
        return EXIT_FAILURE;
    }
    
    uint8_t header_data[HEADER_SIZE];

    if (fread(&header_data, HEADER_SIZE, 1, fp) < 1){
        fclose(fp);
        fprintf(stderr,"Error al leer el header del archivo: %s\n", path);
        return EXIT_FAILURE;
    }
  
    bmp_image * image = &crypt_info.shadows[image_index]; 

    if(parse(&image->header,header_data) == ERROR){
        return EXIT_FAILURE;
    }

    if(crypt_info.args.verbose) {
        print_header_info(&image->header);
    }

    height = image->header.height_px;
    width = image->header.width_px;
    offset = image->header.offset;

    if(image_index != 0 && (height != crypt_info.shadows[0].header.height_px || width != crypt_info.shadows[0].header.width_px)){
        fprintf(stderr, "La sombra: %s no tiene el tamaño adecuado\n", path);
        return EXIT_FAILURE;
    }
    image->header_data = (uint8_t *) malloc(offset * sizeof(uint8_t));
    
    image->image_data = (uint8_t **) malloc(height * sizeof(uint8_t *));

    for(int i = 0; i < height; i++) {
        image->image_data[i] = malloc(width * sizeof(uint8_t));
    }


    fseek(fp, 0L, SEEK_SET);

    if (fread(image->header_data, sizeof(uint8_t), offset, fp) < 1){
        fclose(fp);
        free_image(image_index);
        fprintf(stderr,"Error al leer el header del archivo: %s\n", path);
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < height; i++) {
        if (fread(image->image_data[i], sizeof(uint8_t), width, fp) < 1) {
            fclose(fp);
            free_image(image_index);
            fprintf(stderr,"Error al leer los datos del archivo: %s\n", path);
            return EXIT_FAILURE;
        }
    }

    fclose(fp);

    strcpy(image->filename, path);

    return EXIT_SUCCESS;
}

int read_image_secret(char * path) {

    FILE * fp;
    int height, width, offset;
    
    if ((fp = fopen(path, "rb")) == NULL){
        fprintf(stderr,"Error al abrir la imagen secreta: %s\n", path);
        return EXIT_FAILURE;
    }
    
    uint8_t header_data[HEADER_SIZE];

    if (fread(&header_data, HEADER_SIZE, 1, fp) < 1){
        fclose(fp);
        fprintf(stderr,"Error al leer la imagen secreta: %s\n", path);
        return EXIT_FAILURE;
    }
    
    secret_image * image = &crypt_info.secret;   

    if(parse(&image->header,header_data) == ERROR){
        return EXIT_FAILURE;
    }

    if(crypt_info.args.verbose) {
        print_header_info(&image->header);
    }

    height = image->header.height_px;
    width = image->header.width_px;
    offset = image->header.offset;

    image->header_data = (uint8_t *) malloc(offset * sizeof(uint8_t));
    
    image->image_data = (uint8_t *) malloc(width*height * sizeof(uint8_t));
    
    fseek(fp, 0L, SEEK_SET);

    if (fread(image->header_data, sizeof(uint8_t), offset, fp) < 1){
        fclose(fp);
        free_secret_image();
        fprintf(stderr,"Error al leer el header de la imagen secreta %s\n", path);
        return EXIT_FAILURE;
    }

    if (fread(image->image_data, sizeof(uint8_t), width*height, fp) < 1) {
            fclose(fp);
            free_secret_image();
            fprintf(stderr,"Error al leer los datos de la imagen secreta %s\n", path);
            return EXIT_FAILURE;
    }

    fclose(fp);

    strcpy(image->filename, path);

    return EXIT_SUCCESS;
}

int write_image(char * file_path, bmp_image * image) {
    FILE * fp;

    int height = image->header.height_px;
    int width = image->header.width_px;
    int offset = image->header.offset;

    if ((fp = fopen(file_path, "wb")) == NULL){
        fprintf(stderr,"Error al escribir archivo: %s\n", file_path);
        return EXIT_FAILURE;
    }

    fwrite(image->header_data, sizeof(uint8_t), offset, fp);

    for (int i = 0; i < height; i++) {
        fwrite((image->image_data)[i], sizeof(uint8_t), width, fp);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int write_secret_image(char * file_path, secret_image * image) {
    FILE * fp;

    int height = image->header.height_px;
    int width = image->header.width_px;
    int offset = image->header.offset;

    if ((fp = fopen(file_path, "wb")) == NULL){
        fprintf(stderr,"Error al escribir archivo de imagen secreta: %s\n", file_path);
        return EXIT_FAILURE;
    }

    fwrite(image->header_data, sizeof(uint8_t), offset, fp);
    fwrite(image->image_data, sizeof(uint8_t), width * height, fp);

    fclose(fp);

    return EXIT_SUCCESS;
}

void get_block_by_index (bmp_image * image, int index, uint8_t * return_array) {
    int blocks_per_row = image->header.width_px/2;
    int row = image->header.height_px - 1 - 2*(index/blocks_per_row);
    int col = 2*(index%blocks_per_row);
    
    return_array[0] = image->image_data[row][col];
    return_array[1] = image->image_data[row][col+1];
    return_array[2] = image->image_data[row-1][col];
    return_array[3] = image->image_data[row-1][col+1];
}

void write_block_by_index (bmp_image * image, int index, uint8_t * data) {
    int blocks_per_row = image->header.width_px/2;
    int row = image->header.height_px - 1 - 2*(index/blocks_per_row);
    int col = 2*(index%blocks_per_row);

    image->image_data[row][col] = data[0];
    image->image_data[row][col+1] = data[1];
    image->image_data[row-1][col] = data[2];
    image->image_data[row-1][col+1] = data[3];
}