#ifndef GALOIS_H
#define GALOIS_H
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#define POL_PRIMITIVE 355
#define PIXEL_SIZE 8
#define NULL_INVERSE 0

/** Se calculó a traves de la función generate_galois_inverse_table **/
static const uint8_t galois_inverse_table[256] = 
{0,1,177,222,233,74,111,140,197,165,37,193,134,84,70,231,
211,93,227,133,163,52,209,237,67,166,42,99,35,158,194,119,
216,45,159,28,192,10,243,171,224,181,26,98,217,33,199,189,
144,86,83,234,21,162,128,121,160,91,79,229,97,73,138,205,
108,154,167,24,254,124,14,230,96,61,5,232,200,130,228,58,
112,106,235,50,13,135,49,145,221,246,161,57,210,17,239,191,
72,60,43,27,152,213,117,151,187,184,81,113,64,155,141,6,
80,107,156,142,150,102,195,31,129,55,149,178,69,255,215,136,
54,120,77,201,226,19,12,85,127,214,62,204,7,110,115,157,
48,87,175,248,179,122,116,103,100,212,65,109,114,143,29,34,
56,90,53,20,196,9,25,66,183,172,242,39,169,182,249,146,
223,2,123,148,225,41,173,168,105,186,185,104,198,47,238,95,
36,11,30,118,164,8,188,46,76,131,219,253,139,63,250,245,
236,22,92,16,153,101,137,126,32,44,252,202,247,88,3,176,
40,180,132,18,78,59,71,15,75,4,51,82,208,23,190,94,
241,240,170,38,251,207,89,220,147,174,206,244,218,203,68,125};

uint8_t sum(uint8_t a, uint8_t b);
uint8_t prod(uint8_t a, uint8_t b);
uint8_t f_block(uint8_t * block, uint8_t pixel, int k);
uint8_t calc_parity_bit(uint8_t x);

void generate_galois_inverse_table (uint8_t * array, int n);
void galois_lagrange_interpolation(uint8_t * x, uint8_t * y, uint8_t * s,int k);
void distribute_function_in_block(uint8_t * block, int k, uint8_t * ret);

#endif