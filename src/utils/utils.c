#include "utils.h"
#include <stdlib.h>

int sum_vector(int* vect, int tam) {
    int sum = 0;
    for (int i = 0; i < tam; i++) {
        sum += vect[i];
    }

    return sum;
}

int* copy_vector(int* vect, int tam){
    int* copy = (int*)malloc(tam * sizeof(int));
    
    for (int i = 0; i < tam; i++) {
        copy[i] = vect[i];
    }

    return copy;
}

int get_smaller_value(int val1, int val2) {
    if (val1 <= val2) {
        return val1;
    }
    return val2;
}
