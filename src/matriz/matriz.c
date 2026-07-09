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

int** multiply_matriz_by_value_matriz(int** matriz_1, int** matriz_2, int rows, int collumns) {
    int** result = create_matriz(rows, collumns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collumns; j++) {
            result[i][j] = matriz_1[i][j] * matriz_2[i][j];
        }
    }
    return result;
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
            int oferta_atual = copy_oferta[i];
            int demanda_atual = copy_demanda[j];
            
            int smaller_val = get_smaller_value(oferta_atual, demanda_atual);
            if (smaller_val == 0) {continue;}
            copy_oferta[i] -= smaller_val;
            copy_demanda[j] -= smaller_val;
            matriz_solucao[i][j] = smaller_val; 

        }
    }

    free(copy_oferta);
    free(copy_demanda);

    return multiply_matriz_by_value_matriz(matriz->matriz, matriz_solucao, matriz->rows, matriz->collumns);
}

int** metodo_custo_minimo(Matriz* matriz) {
    int** matriz_solucao = create_matriz(matriz->rows, matriz->collumns);
    int* copy_oferta = copy_vector(matriz->oferta, matriz->rows);
    int* copy_demanda = copy_vector(matriz->demanda, matriz->collumns);

    while (sum_vector(copy_oferta, matriz->rows) > 0 && sum_vector(copy_demanda, matriz->collumns) > 0) {
        int min_cost = __INT_MAX__;
        int min_row = -1;
        int min_col = -1;

        for (int i = 0; i < matriz->rows; i++) {
            for (int j = 0; j < matriz->collumns; j++) {
                if (copy_oferta[i] > 0 && copy_demanda[j] > 0 && matriz->matriz[i][j] < min_cost) {
                    min_cost = matriz->matriz[i][j];
                    min_row = i;
                    min_col = j;
                }
            }
        }

        if (min_row == -1 || min_col == -1) {
            break;
        }

        int smaller_val = get_smaller_value(copy_oferta[min_row], copy_demanda[min_col]);
        copy_oferta[min_row] -= smaller_val;
        copy_demanda[min_col] -= smaller_val;
        matriz_solucao[min_row][min_col] = smaller_val;
    }

    free(copy_oferta);
    free(copy_demanda);

    return multiply_matriz_by_value_matriz(matriz->matriz, matriz_solucao, matriz->rows, matriz->collumns);
}


