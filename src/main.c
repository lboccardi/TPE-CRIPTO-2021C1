#include "codec.h"
#include <stdlib.h>
#include <errno.h>


void printHeaderInfo(BMPHeader * header){
    printf("Type:\t%x\tSize:\t%d\n",header->type, header->size);
    printf("Res1:\t%x\tRes2:\t%x\n",header->reserved1,header->reserved2);
    printf("Offset:\t%d\tDIB:\t%x\n",header->offset, header->dib_header_size);
    printf("Width:\t%d\tHeight:\t%d\n",header->width_px,header->height_px);
    printf("Planes:\t%d\tBPP:\t%d\n",header->num_planes, header->bits_per_pixel);
    printf("Comp:\t%x\tImg B:\t%d\n",header->compression, header->image_size_bytes);
    printf("Res X:\t%d\tRes Y:\t%d\n",header->x_resolution_ppm,header->y_resolution_ppm);
    printf("Colors:\t%d\tImp C:\t%d\n",header->num_colors,header->important_colors);

}
int main () {
    FILE *fp;          /* Open file pointer */
     if ((fp = fopen("test-images/lena.bmp", "rb")) == NULL){
         printf("error al abrir\n");
         return 0;
     }
    uint8_t data[HEADER_SIZE];
    if (fread(&data, HEADER_SIZE, 1, fp) < 1)
        {
        /* Couldn't read the file header - return NULL... */
	    fclose(fp);
        printf("error al leer\n");
        return 0;
        }
    BMPHeader header;
    if(parse(&header,data) == ERROR){
        exit(EXIT_FAILURE);
    } 
    printHeaderInfo(&header);
    return 0;
}