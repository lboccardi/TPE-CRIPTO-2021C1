#ifndef CODEC_H
#define CODEF_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define HEADER_SIZE 54
#define TYPE 0x4d42
#define BITS_PER_PX 8

/* Struct for bmp_header and access info  */
typedef struct bmp_header {             // Total: 54 bytes
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
} bmp_header;

/** Transaction states */
typedef enum request_states{
    FINISHED,
    ERROR
}request_states;

/** Parser states */
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

request_states parse(struct bmp_header * result,uint8_t *data);
#endif