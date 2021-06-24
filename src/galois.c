#include "galois.h"

uint16_t multiplication(uint8_t a, uint8_t b);
uint8_t redux(uint16_t a, uint16_t b);
void swap_with_last(uint8_t * x, uint8_t * y, int i, int n);
uint8_t sequential_interpolation_product(uint8_t * x, int i, int upper_bound);

uint8_t sum(uint8_t a, uint8_t b){
    return a ^ b;
}


uint8_t prod(uint8_t a, uint8_t b){
    uint16_t result = multiplication(a,b);
    return redux(result, POL_PRIMITIVE);
}

uint16_t multiplication(uint8_t a, uint8_t b){
    int i = 0;
    int array[8];
    for (i = 0; i < PIXEL_SIZE; ++i){
        array[i] = (a >> i) & 1;
    }
    
    uint16_t result = 0;

    for (i = 0; i < PIXEL_SIZE; i++){
        if(array[i] == 1){
            result = result ^ ((uint16_t) b << i);
        }
    }
    
    return result;
}

int recus_degree(uint16_t a, int exp){
    if ( a / pow(2, exp + 1) < 1){
        return exp;
    }
    return recus_degree(a, exp + 1);
}

int degree(uint16_t a){
    if (a == 0){
        return 0;
    }
    return recus_degree(a, 0);
}

uint8_t redux(uint16_t p, uint16_t m){
    int deg_p = degree(p);
    int deg_m = degree(m);
    int deg_resto;

    int deg = deg_p - deg_m;
    uint16_t resto = p;

    while(deg >= 0){
        resto = (m << deg) ^ resto;
        
        deg_resto = degree(resto);
    
        deg = deg_resto - deg_m;
    }
    
     return (uint8_t) resto;
}

uint8_t gf_pow(uint8_t pixel, int times){
    if(times == 0){
        return 1;
    }
    int i; 
    uint8_t result = pixel;

    for(i = 0; i < times - 1; i++){
        result = prod(result, pixel);
    }

    return result;
}

uint8_t f_block(uint8_t * block, uint8_t pixel, int k){
    int i;
    uint8_t result = 0;

    for(i = 0; i < k; i++){
        uint8_t s = block[i];
        uint8_t aux = prod(s, gf_pow(pixel, i));
        result = sum(result, aux);
    }

    return result;
}

uint8_t calc_parity_bit(uint8_t x){
    return ( (x>>7) ^ 
             (x>>6) ^
             (x>>5) ^
             (x>>4) ^
             (x>>3) ^
             (x>>2) ^
             (x>>1) ^
             (x) ) & 1;
}

void distribute_function_in_block(uint8_t * block, int k, uint8_t * ret) {
    uint8_t f_x = f_block(block,ret[0],k);
    uint8_t replacement;

    ret[1]&=0xF8;
    replacement = f_x & 0xE0;
    replacement >>= 5;
    ret[1]|=replacement;

    ret[2]&=0xF8;
    replacement = f_x & 0x1C;
    replacement >>= 2;
    ret[2]|=replacement;

    ret[3]&=0xF8;
    replacement = f_x & 0x03;
    uint8_t parity= calc_parity_bit(f_x) <<2;
    replacement|=parity;
    ret[3]|=replacement;
}

void generate_galois_inverse_table (uint8_t * array, int n) {
    uint8_t aux = 0;
    uint8_t i, j;
    bool flag = false;

    array[0] = NULL_INVERSE; // 0 does not have an inverse, it's a convention

    for (i = 1; i != 0; i++) {
        for(j = 1; j != 0 && !flag; j++) {
            aux = prod(i, j);
            if(aux == 1) {
                array[i] = j;
                flag = true;
            }
        }
        flag = false; 
    }
}

uint8_t sequential_interpolation_product(uint8_t * x, int i, int upper_bound) {
    uint8_t to_return = 1;

    for (int q = 0; q < upper_bound; q++) {
        if (q != i) {
            to_return = prod(to_return, prod(x[q],galois_inverse_table[sum(x[i], x[q])]));
        }
    }

    return to_return;
}

void swap_with_last(uint8_t * x, uint8_t * y, int i, int n) {
    uint8_t aux_x, aux_y;
    
    aux_x = x[n-1];
    aux_y = y[n-1];

    x[n-1] = x[i];
    y[n-1] = y[i];

    x[i] = aux_x;
    y[i] = aux_y; 
}

void galois_lagrange_interpolation(uint8_t * x, uint8_t * y, uint8_t * s,int k) {
    uint8_t curr = 0;
    uint8_t y_p[k];
    int i, r, new_upper_bound;

    for (i = 0; i < k - 1; i++) {
        if (x[i] == 0) {
            swap_with_last(x, y, i, k);
        }

        y_p[i] = y[i];
    }
    y_p[k-1] = y[k-1];
    
    s[0] = curr;

    for(r = 0; r<k; r++) {
        new_upper_bound =  k - r;
        curr = 0;

        for(i = 0; i < new_upper_bound; i++) {
            if (r > 0) {
                y_p[i] = prod(sum(y_p[i],s[r-1]), galois_inverse_table[x[i]]);
            }
            curr = sum(curr, prod(y_p[i], sequential_interpolation_product(x, i, new_upper_bound )));
        }

        s[r] = curr;
    }
}
