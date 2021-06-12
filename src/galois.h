#ifndef GALOIS_H
#define GALOIS_H
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#define POL_PRIMITIVE 355
#define PIXEL_SIZE 8
#define NULL_INVERSE 0

uint8_t sum(uint8_t a, uint8_t b);
uint8_t prod(uint8_t a, uint8_t b);
uint8_t f_block(uint8_t * block, uint8_t pixel, int k);
uint8_t calc_parity_bit(uint8_t x);

void generate_galois_inverse_table (uint8_t * array, int n);
void galois_lagrange_interpolation(uint8_t * x, uint8_t * y, uint8_t * s,int k, uint8_t * inv);

#endif