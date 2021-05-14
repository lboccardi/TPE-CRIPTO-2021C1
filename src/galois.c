#include "galois.h"

unsigned int multiplication(unsigned a, unsigned int b);
unsigned int redux(unsigned int a, unsigned int b);

unsigned int sum(unsigned int a, unsigned int b){
    return a ^ b;
}


unsigned int prod(unsigned int a, unsigned int b){

    unsigned int result = multiplication(a,b);

    return redux(result, POL_PRIMITIVE);


}

unsigned int multiplication(unsigned a, unsigned int b){
    
    int i = 0;
    int array[8];
    for (i = 0; i < 8; ++i){
        array[i] = (a >> i) & 1;
    }
    
    unsigned int result = 0;

    for (i = 0; i < 8; i++){
        if(array[i] == 1){
            result = result ^ (b << i);
        }
    }
    
    return result;
}

unsigned int recus_degree(unsigned int a, int exp){
    if ( a / pow(2, exp + 1) < 1){
        return exp;
    }
    return recus_degree(a, exp + 1);
}

unsigned int degree(unsigned int a){
    if (a == 0){
        return 0;
    }
    return recus_degree(a, 0);
}

unsigned int redux(unsigned int p, unsigned int m){
    int deg_p = degree(p);
    int deg_m = degree(m);
    int deg_resto;

    int deg = deg_p - deg_m;
    unsigned int resto = p;

    while(deg >= 0){
        resto = (m << deg) ^ resto;
        
        deg_resto = degree(resto);
    
        deg = deg_resto - deg_m;
    }

     return resto;
}