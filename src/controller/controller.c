#include "../view/view.h"
#include "../vogel/vogel.h"
#include "controller.h"
#include <stdlib.h>
#include <stdio.h>
#include "../utils/utils.h"

Matriz* create_problem_controller() {
    int rows = get_rows();
    int collumns = get_collumns();
    Matriz* matriz = create_problem_matriz(rows, collumns);
    fill_problem(matriz);
    return matriz;
}

void print_problem_controller(Matriz* matriz) {
    print_problem_view(matriz);
}

/**
 * Exibe o estado de balanceamento do problema.
 * Não bloqueia a execução — o balanceamento automático
 * é feito internamente por cada método de solução.
 */
bool verify_is_balanced(Matriz* matriz) {
    if (!is_balanced(matriz)) {
        printf("\n[AVISO] A matriz não está balanceada.");
        printf("\n        Um nó fictício será adicionado automaticamente em cada método.\n");
        return false;
    }
    return true;
}

/** Libera uma matriz de solução com as dimensões corretas (após balanceamento). */
static void free_solucao(int** sol, Matriz* original) {
    int total_oferta  = sum_vector(original->oferta,  original->rows);
    int total_demanda = sum_vector(original->demanda, original->collumns);
    /* Número de linhas real da solução */
    int rows = (total_demanda > total_oferta)
               ? original->rows + 1
               : original->rows;
    for (int i = 0; i < rows; i++) free(sol[i]);
    free(sol);
}

void use_metodo_canto_noroeste(Matriz* matriz) {
    verify_is_balanced(matriz);
    int** matriz_solucao = metodo_canto_noroeste(matriz);
    print_matriz(matriz_solucao, matriz->rows, matriz->collumns,
                 "\nMatriz de solução usando o método canto noroeste:");
    free_solucao(matriz_solucao, matriz);
}

void use_metodo_custo_minimo(Matriz* matriz) {
    verify_is_balanced(matriz);
    int** matriz_solucao = metodo_custo_minimo(matriz);
    print_matriz(matriz_solucao, matriz->rows, matriz->collumns,
                 "\nMatriz de solução usando o método de custo mínimo:");
    free_solucao(matriz_solucao, matriz);
}

void use_metodo_vogel(Matriz* matriz) {
    verify_is_balanced(matriz);
    int** matriz_solucao = metodo_vogel(matriz);
    print_matriz(matriz_solucao, matriz->rows, matriz->collumns,
                 "\nMatriz de solução usando o Método de Aproximação de Vogel:");
    free_solucao(matriz_solucao, matriz);
}


