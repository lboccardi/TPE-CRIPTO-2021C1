#include "codec.h"


request_states parse(struct bmp_header * result,uint8_t *data){
    states actual_state = type;
    uint8_t * d_index = data;
    while(actual_state != finished && actual_state != error){
        switch (actual_state)
        {
        case type:
            memcpy(&result->type,d_index,2);
            d_index+=2;
            if(result->type != TYPE){
                actual_state = error;
                fprintf(stderr, "Tipo de archivo incorrecto, no es un BMP.\n");
                break;
            }
            actual_state =size;
            break;
        case size:
            memcpy(&result->size,d_index,4);
            d_index+=4;
            if(result->size <= 0){
                actual_state = error;
                fprintf(stderr, "El tamaño de la imagen debe ser mayor a 0.\n");
                break;
            }
            actual_state =reserved;
            break;
        case reserved:
            memcpy(&result->reserved1,d_index,2);
            d_index+=2;
            memcpy(&result->reserved2,d_index,2);
            d_index+=2;
            actual_state =offset;
            break;
        case offset:
            memcpy(&result->offset,d_index,4);
            d_index+=4;
            actual_state =dib_header;
            break;
        case dib_header:
            memcpy(&result->dib_header_size,d_index,4);
            d_index+=4;
            actual_state =width;
            break;
        case width:
            memcpy(&result->width_px,d_index,4);
            d_index+=4;
            if(result->width_px <= 0){
                fprintf(stderr, "El ancho de la imagen no debe ser 0.\n");
                actual_state = error;
                break;
            }
            actual_state =height;
            break;
        case height:
            memcpy(&result->height_px,d_index,4);
            d_index+=4;
            if(result->height_px <= 0){
                fprintf(stderr, "La altura de la imagen no debe ser 0.\n");
                actual_state = error;
                break;
            }
            actual_state =planes;
            break;
        case planes:
            memcpy(&result->num_planes,d_index,2);
            d_index+=2;
            actual_state = bits_per_px;
            break;
        case bits_per_px:
            memcpy(&result->bits_per_pixel,d_index,2);
            d_index+=2;
            if(result->bits_per_pixel != BITS_PER_PX){
                fprintf(stderr, "La cantidad de bits por pixel debe ser 8.\n");
                actual_state = error;
                break;
            }
            actual_state = compress;
            break;
        case compress:
            memcpy(&result->compression,d_index,4);
            d_index+=4;
            actual_state = image_size_bytes;
            break;
        case image_size_bytes:
            memcpy(&result->image_size_bytes,d_index,4);
            d_index+=4;
            actual_state = x_resolution_ppm;
            break;
        case x_resolution_ppm:
            memcpy(&result->x_resolution_ppm,d_index,4);
            d_index+=4;
            actual_state = y_resolution_ppm;
            break;
        case y_resolution_ppm:
            memcpy(&result->y_resolution_ppm,d_index,4);
            d_index+=4;
            actual_state = num_colors;
            break;
        case num_colors:
            memcpy(&result->num_colors,d_index,4);
            d_index+=4;
            actual_state = important_colors;
            break;
        case important_colors:
            memcpy(&result->important_colors,d_index,4);
            d_index+=4;
            actual_state = finished;
            break;
        default:
            break;
        }
    }
    return actual_state == finished ? FINISHED : ERROR;
}