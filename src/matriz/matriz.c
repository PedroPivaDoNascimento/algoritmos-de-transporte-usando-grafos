#include "matriz.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "../view/view.h"

int** create_matriz(int rows, int collumns) {
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < collumns; i++) {
        matriz[i] = (int*)malloc(collumns * sizeof(int));
    }
    return matriz;
}

Matriz* create_problem_matriz(int rows, int collumns) {
    Matriz* matriz = (Matriz*)malloc(sizeof(Matriz));
    matriz->rows = rows;
    matriz->collumns = collumns;
    matriz->oferta = (int*)malloc(rows * sizeof(int));
    matriz->demanda = (int*)malloc(collumns * sizeof(int));
    matriz->matriz = create_matriz(rows, collumns);
    return matriz;
}

bool is_balanced(Matriz* matriz) {
    int sum_oferta = sum_vector(matriz->oferta, matriz->rows);
    int sum_demanda = sum_vector(matriz->demanda, matriz->collumns);

    return sum_oferta == sum_demanda;
}

int** metodo_canto_noroeste(Matriz* matriz) {
    int** matriz_solucao = create_matriz(matriz->rows, matriz->collumns);
    int* copy_oferta = copy_vector(matriz->oferta, matriz->rows);
    int* copy_demanda = copy_vector(matriz->demanda, matriz->collumns);

    for (int i = 0; i < matriz->rows; i++) {
        for (int j = 0; j < matriz->collumns; j++) {
            int oferta_atual = matriz->oferta[i];
            int demanda_atual = matriz->oferta[j];
            
            int smaller_val = get_smaller_value(oferta_atual, demanda_atual);
            if (smaller_val == 0) {continue;}
            copy_oferta[i] -= smaller_val;
            copy_demanda[j] -= smaller_val; 

        }
    }

}

