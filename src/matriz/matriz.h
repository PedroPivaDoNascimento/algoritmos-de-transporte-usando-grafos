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

int** create_matriz(int rows, int collumns);
Matriz* create_problem_matriz(int rows, int collumns);
bool is_balanced(Matriz* matriz);
int** metodo_canto_noroeste(Matriz* matriz);
#endif // MATRIZ_H