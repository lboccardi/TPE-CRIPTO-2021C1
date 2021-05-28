#include "galois.h"

uint16_t multiplication(uint8_t a, uint8_t b);
uint8_t redux(uint16_t a, uint16_t b);

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

