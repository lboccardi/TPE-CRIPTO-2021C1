#include "cript.h"
#include <string.h>

void free_all_images() {
    for (int i = -1; i < crypt_info.args.k; i++) {
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
        fprintf(stderr,"error al leer header\n");
        return EXIT_FAILURE;
    }
  
    bmp_image * image = &crypt_info.shadows[image_index]; 

    if(parse(&image->header,header_data) == ERROR){
        return EXIT_FAILURE;
    }

    if(crypt_info.args.verbose) {
        printHeaderInfo(&image->header);
    }

    height = image->header.height_px;
    width = image->header.width_px;
    offset = image->header.offset;

    if(image_index != 0 && (height != crypt_info.shadows[0].header.height_px || width != crypt_info.shadows[0].header.width_px)){
        fprintf(stderr, "Las imagenes no tienen el mismo tamaño.\n");
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
        fprintf(stderr,"Error al leer header data\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < height; i++) {
        if (fread(image->image_data[i], sizeof(uint8_t), width, fp) < 1) {
            fclose(fp);
            free_image(image_index);
            fprintf(stderr,"Error al leer image data\n");
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
        fprintf(stderr,"Error al abrir %s\n", path);
        return EXIT_FAILURE;
    }
    
    uint8_t header_data[HEADER_SIZE];

    if (fread(&header_data, HEADER_SIZE, 1, fp) < 1){
        fclose(fp);
        fprintf(stderr,"error al leer\n");
        return EXIT_FAILURE;
    }
    
    secret_image * image = &crypt_info.secret;   

    if(parse(&image->header,header_data) == ERROR){
        return EXIT_FAILURE;
    }

    if(crypt_info.args.verbose) {
        printHeaderInfo(&image->header);
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
        fprintf(stderr,"Error al leer\n");
        return EXIT_FAILURE;
    }

    if (fread(image->image_data, sizeof(uint8_t), width*height, fp) < 1) {
            fclose(fp);
            free_secret_image();
            fprintf(stderr,"Error al leer\n");
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
        fprintf(stderr,"Error al escribir archivo %s\n", file_path);
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
        fprintf(stderr,"Error al escribir archivo %s\n", file_path);
        return EXIT_FAILURE;
    }

    fwrite(image->header_data, sizeof(uint8_t), offset, fp);
    fwrite(image->image_data, sizeof(uint8_t), width * height, fp);

    fclose(fp);

    return EXIT_SUCCESS;
}


uint8_t calcParityBit(uint8_t x){
    return ( (x>>7) ^ 
             (x>>6) ^
             (x>>5) ^
             (x>>4) ^
             (x>>3) ^
             (x>>2) ^
             (x>>1) ^
             (x) ) & 1;
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

void generate_galois_inverse_table (uint8_t * array, int n) {
    uint8_t aux = 0;
    uint8_t i, j;
    bool flag = false;

    array[0] = 0;

    for (i = 1; i != 0; i++) {
        for(j = 1; j != 0 && !flag; j++) {
            aux = prod(i, j);
            if(aux == 1) {
                array[i] = j;
                flag = true;
            }
        }
        flag = false; 
    }

}

uint8_t sequential_interpolation_product(uint8_t * x, int i, int upper_bound, uint8_t * inv) {
    uint8_t to_return = 1;

    for (int q = 0; q < upper_bound; q++) {
        if (q != i) {
            to_return = prod(to_return, prod(x[q],inv[sum(x[i], x[q])]));
        }
    }

    return to_return;
}

uint8_t calculate_y_prime (uint8_t * x, uint8_t * y, uint8_t secret, int i, uint8_t * inv) {
    return prod(sum(y[i],secret), inv[x[i]]);
}

void interpolation(uint8_t * x, uint8_t * y, uint8_t * s,int k, uint8_t * inv) {
    uint8_t actual_secret = 0, productoria, mult, aux_x, aux_y;
    uint8_t y_p[k];
    int i, r;

    for (i = 0; i < k - 1; i++) {
        if (x[i] == 0) {
            aux_x = x[k-1];
            aux_y = y[k-1];

            x[k-1] = x[i];
            y[k-1] = y[i];

            x[i] = aux_x;
            y[i] = aux_y; 
        }
    }
    
    for(i = 0; i < k; i++){
        productoria = sequential_interpolation_product(x, i, k, inv); 
        mult = prod(y[i],productoria);
        actual_secret = sum(actual_secret,mult);
        y_p[i] = y[i];
    }

    s[0] = actual_secret;
    int new_upper_bound;

    for(r = 1; r<k; r++) {
        new_upper_bound =  k - r;
        actual_secret = 0;

        for(i = 0; i < new_upper_bound; i++){
            y_p[i] = calculate_y_prime(x, y_p, s[r-1], i, inv);
            productoria = sequential_interpolation_product(x, i, new_upper_bound, inv);
            mult = prod(y_p[i],productoria);
            actual_secret = sum(actual_secret,mult);
        }
        
        s[r] = actual_secret;
    }
}
