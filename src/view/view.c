#include "view.h"
#include <stdlib.h>
#include <stdio.h>

int get_rows() {
    int rows = 0;
    printf("Digite o número de linhas da matriz: ");
    scanf("%d", &rows);
    return rows;
}

int get_collumns() {
    int collumns = 0;
    printf("Digite o número de colunas da matriz: ");
    scanf("%d", &collumns);
    return collumns;
}


int get_val_custo(int row, int collumn) {
    int val = 0;
    printf("Digite o valor do custo da posição [%d][%d]: ", row, collumn);
    scanf("%d", &val);
    return val;
}

int get_val_oferta(int row) {
    int val = 0;
    printf("Figite o valor da oferta na linha [%d]: ", row);
    scanf("%d", &val);
    return val;
}

int get_val_demanda(int collumn) {
    int val = 0;
    printf("Digite o valor da demanda na coluna [%d]: ", collumn);
    scanf("%d", &val);
    return val;
}


void fill_problem(Matriz* matriz) {
    for (int i = 0; i < matriz->rows; i++) {
        for (int j = 0; j < matriz->collumns; j++) {
            matriz->matriz[i][j] = get_val_custo(i+1, j+1);
        }
    }

    for (int i = 0; i < matriz->rows; i++) {
        matriz->oferta[i] = get_val_oferta(i+1);
    }

    for (int j = 0; j < matriz->collumns; j++) {
        matriz->demanda[j] = get_val_demanda(j+1);
    }
}

void print_matriz(int** matriz, int rows, int collumns, char* label) {
    printf("\n%s\n", label);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collumns; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
}

void print_problem_view(Matriz* matriz) {
    print_matriz(matriz->matriz, matriz->rows, matriz->collumns, "Matriz de custos:");

    printf("\nOferta:\n");
    for (int i = 0; i < matriz->rows; i++) {
        printf("%d ", matriz->oferta[i]);
    }
    printf("\n");

    printf("\nDemanda:\n");
    for (int j = 0; j < matriz->collumns; j++) {
        printf("%d ", matriz->demanda[j]);
    }
    printf("\n");
}

void print_error_balanced() {
    printf("\nERRO - Os valoresde oferta de demanda não estão balanceados, portanto não será possível resolver esse problema.\n");
}
