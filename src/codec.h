#ifndef CODEC_H
#define CODEF_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define HEADER_SIZE 54
#define TYPE 0x4d42
#define BITS_PER_PX 8

typedef struct BMPHeader {             // Total: 54 bytes
  uint16_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeader;

/** Estado en el que se regresa de la transacción */
typedef enum request_states{
    FINISHED,
    ERROR
}request_states;

/** Estados del parser */
typedef enum states{
 type,
 size,
 reserved,
 offset,
 dib_header,
 width,
 height,
 planes,
 bits_per_px,
 compress,
 image_size_bytes,
 x_resolution_ppm,
 y_resolution_ppm,
 num_colors,
 important_colors,
 finished,
 error   
}states;

/** parsear **/
request_states parse(struct BMPHeader * result,uint8_t *data);
#endif