#include "cript.h"
#include <string.h>

void free_all_images() {
    for (int i = -1; i < crypt_info.args.k; i++) {
        free_image(i);
    }
}

void free_image(int image_index) {
    bmp_image * image = (image_index < 0) ? &crypt_info.secret : &crypt_info.shadows[image_index];
    int height = image->header.height_px;

    free(image->header_data);
    for(int i = 0; i < height; i++) {
		free(image->image_data[i]);
    }
    free(image->image_data);
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

    image->header_data = (uint8_t *) malloc(offset * sizeof(uint8_t));
    image->image_data = (uint8_t **) malloc(height * sizeof(uint8_t *));

	for(int i = 0; i < height; i++) {
		image->image_data[i] = malloc(width * sizeof(uint8_t));
    }

    fseek(fp, 0L, SEEK_SET);

    if (fread(image->header_data, sizeof(uint8_t), offset, fp) < 1){
        fclose(fp);
        free_image(image_index);
        fprintf(stderr,"Error al leer\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < height; i++) {
        if (fread(image->image_data[i], sizeof(uint8_t), width, fp) < 1) {
            fclose(fp);
            free_image(image_index);
            fprintf(stderr,"Error al leer\n");
            return EXIT_FAILURE;
        }
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
