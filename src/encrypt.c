#include "cript.h"
#include <stdio.h>
#include <string.h>


int input_distribute(){

    if (read_image_secret(crypt_info.args.input) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (find_images_in_directory(crypt_info.args.output) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int encryption_distribute(){
    int height = crypt_info.secret.header.height_px;
    int width = crypt_info.secret.header.width_px;
    int k = crypt_info.args.k;
    int n = crypt_info.n;
    
    int bcount = (height*width)/k;
        for(int i=0; i<bcount;i++){
            uint8_t * secret = crypt_info.secret.image_data+k*i;
            uint8_t xs[n];
         for(int j = 0; j < n ;j++){
             uint8_t aux[4];
             get_block_by_index(&crypt_info.shadows[j],i,aux);
             uint8_t block[k];
             memcpy(block,secret,k*sizeof(uint8_t));

             for(int r=0;r<j;r++){
                 if(aux[0]==xs[r]){
                     aux[0]=(aux[0]+1)%255;
                     r=0;
                 }
             }   
             xs[j]=aux[0];
             
             uint8_t f_x = f_block(block,aux[0],k);
             uint8_t replacement;

             aux[1]&=0xF8;
             replacement = f_x & 0xE0;
             replacement >>= 5;
             aux[1]|=replacement;

             aux[2]&=0xF8;
             replacement = f_x & 0x1C;
             replacement >>= 2;
             aux[2]|=replacement;

             aux[3]&=0xF8;
             replacement = f_x & 0x03;
             uint8_t parity= calc_parity_bit(f_x) <<2;
             replacement|=parity;
             aux[3]|=replacement;
             write_block_by_index(&crypt_info.shadows[j],i,aux);
         }   
        }    
    return EXIT_SUCCESS;
}

int output_distribute(){

    bmp_image * image;
    int length = strlen(crypt_info.args.output) + MAX_PATH_LENGHT + 2;
    char new_file_name[length];


    for (int i = 0; i < crypt_info.n; i++) {
        image = &crypt_info.shadows[i];
        sprintf(new_file_name, "%s/%s", crypt_info.args.output, image->filename);
        write_image(new_file_name, image);
    }

    free_all_images();

    return EXIT_SUCCESS;
}
