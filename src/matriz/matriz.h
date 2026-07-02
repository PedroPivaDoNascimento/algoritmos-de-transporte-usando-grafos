#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdbool.h>

typedef struct {
    int** matriz;
    int rows;
    int collumns;
    int* oferta;
    int* demanda;
} Matriz;

Matriz* create_matriz(int rows, int collumns);
bool is_balanced(Matriz* matriz);

#endif // MATRIZ_H