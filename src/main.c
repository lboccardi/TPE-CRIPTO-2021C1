#include "cript.h"

struct crypt crypt_info ;

void printHeaderInfo(bmp_header * header){
    printf("Type:\t%x\tSize:\t%d\n",header->type, header->size);
    printf("Res1:\t%x\tRes2:\t%x\n",header->reserved1,header->reserved2);
    printf("Offset:\t%d\tDIB:\t%x\n",header->offset, header->dib_header_size);
    printf("Width:\t%d\tHeight:\t%d\n",header->width_px,header->height_px);
    printf("Planes:\t%d\tBPP:\t%d\n",header->num_planes, header->bits_per_pixel);
    printf("Comp:\t%x\tImg B:\t%d\n",header->compression, header->image_size_bytes);
    printf("Res X:\t%d\tRes Y:\t%d\n",header->x_resolution_ppm,header->y_resolution_ppm);
    printf("Colors:\t%d\tImp C:\t%d\n",header->num_colors,header->important_colors);
    printf("\n\n");
}
int main (int argc, char **argv) {
    
    if(argc < ARGS_COUNT){
         fprintf(stderr,"Cantidad incorrecta de parámetros\n");
         return EXIT_FAILURE;
    }
    if(EXIT_FAILURE == check_arguments(argc,argv)){
        return EXIT_FAILURE;
    }
    if(EXIT_FAILURE == crypt_info.functions.handle_input()){
        return EXIT_FAILURE;
    }
    if(EXIT_FAILURE == crypt_info.functions.handle_crypt()){
        return EXIT_FAILURE;
    }
    if(EXIT_FAILURE == crypt_info.functions.handle_output()){
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}