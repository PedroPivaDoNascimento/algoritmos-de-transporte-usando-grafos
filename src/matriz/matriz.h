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

int**    create_matriz(int rows, int collumns);
int**    multiply_matriz_by_value_matriz(int** matriz_1, int** matriz_2, int rows, int collumns);
Matriz*  create_problem_matriz(int rows, int collumns);
void     free_matriz(Matriz* m);          /* libera toda a estrutura Matriz */
bool     is_balanced(Matriz* matriz);

/**
 * balance_matriz — Retorna uma nova Matriz* balanceada.
 *
 * Se oferta == demanda  → devolve cópia da própria matriz.
 * Se oferta >  demanda  → acrescenta coluna fictícia (consumidor fantasma).
 * Se demanda >  oferta  → acrescenta linha  fictícia (fornecedor fantasma).
 *
 * O ponteiro 'ghost_added' (pode ser NULL) é preenchido com:
 *   0  — já era balanceada.
 *   1  — coluna fictícia adicionada (consumidor fantasma).
 *  -1  — linha  fictícia adicionada (fornecedor fantasma).
 *
 * O chamador é responsável por liberar a estrutura retornada via free_matriz().
 */
Matriz*  balance_matriz(Matriz* matriz, int* ghost_added);

int**    metodo_canto_noroeste(Matriz* matriz);
int**    metodo_custo_minimo(Matriz* matriz);
int      calculate_total_cost(int** matriz_solucao, int** custos, int rows, int collumns);

#endif // MATRIZ_H