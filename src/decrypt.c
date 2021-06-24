#include "cript.h"

int input_recover() {

    if (find_images_in_directory(crypt_info.args.input) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    secret_image *image = &crypt_info.secret;
    image->header_data = (uint8_t *)malloc(crypt_info.shadows[0].header.offset * sizeof(uint8_t));
    image->image_data = (uint8_t *)malloc(crypt_info.shadows[0].header.width_px * crypt_info.shadows[0].header.height_px * sizeof(uint8_t));
    strcpy(image->filename, crypt_info.args.output);

    memcpy(&image->header,&crypt_info.shadows[0].header,sizeof(crypt_info.shadows[0].header));
    memcpy(image->header_data,crypt_info.shadows[0].header_data,crypt_info.shadows[0].header.offset);
    
    return EXIT_SUCCESS;
}

int decryption_recover() {
    int height = crypt_info.shadows[0].header.height_px;
    int width = crypt_info.shadows[0].header.width_px;
    int k = crypt_info.args.k;
    
    int bcount = (height * width) / k;
    for (int i = 0; i < bcount; i++) {
        uint8_t x[k];
        uint8_t f_x[k];
        for (int j = 0; j < k; j++){
            uint8_t aux[4];
            get_block_by_index(&crypt_info.shadows[j], i, aux);

            uint8_t t;

            t = (aux[1] & 0x07) << 5;

            t |= (aux[2] & 0x07) << 2;

            t |= (aux[3] & 0x03);

            uint8_t calc_parity= calc_parity_bit(t);
            uint8_t parity = (aux[3] & 0x04) >> 2;
            if(parity != calc_parity){
                fprintf(stderr, "Error de paridad\n");
                return EXIT_FAILURE;
            }

            x[j] = aux[0];
            f_x[j] = t;
        }
        uint8_t secret[k];
        galois_lagrange_interpolation(x,f_x,secret,k);
        memcpy(crypt_info.secret.image_data+i*k,secret,k*sizeof(uint8_t));
    }

    return EXIT_SUCCESS;
}


int output_recover() {
    int return_value = write_secret_image(crypt_info.secret.filename, &crypt_info.secret);
    free_all_images();
    return return_value;
}