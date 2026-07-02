#include "matriz.h"
#include <stdlib.h>
#include <stdio.h>
#include "../view/view.h"

Matriz* create_matriz(int rows, int collumns) {
    Matriz* matriz = (Matriz*)malloc(sizeof(Matriz));
    matriz->rows = rows;
    matriz->collumns = collumns;
    matriz->oferta = (int*)malloc(rows * sizeof(int));
    matriz->demanda = (int*)malloc(collumns * sizeof(int));
    matriz->matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < collumns; i++) {
        matriz->matriz[i] = (int*)malloc(collumns * sizeof(int));
    }
    return matriz;
}



