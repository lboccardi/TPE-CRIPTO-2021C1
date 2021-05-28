#ifndef GALOIS_H
#define GALOIS_H
#include <math.h>
#include <stdint.h>
#define POL_PRIMITIVE 355
#define PIXEL_SIZE 8

uint8_t sum(uint8_t a, uint8_t b);
uint8_t prod(uint8_t a, uint8_t b);
uint8_t f_block(uint8_t * block, uint8_t pixel, int k);
#endif