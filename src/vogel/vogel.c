#include "vogel.h"
#include "../matriz/matriz.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Calcula a penalidade de uma linha i:
 * diferença entre o 2º e o 1º menor custo nas colunas ativas.
 * Retorna -1 se a linha estiver bloqueada ou tiver menos de 2 colunas ativas.
 */
static int penalidade_linha(int** custos, int* col_blocked, int rows, int collumns, int i, int* oferta_rest, int* demanda_rest) {
    (void)rows;
    if (oferta_rest[i] == 0) return -1; /* linha esgotada */

    int primeiro = __INT_MAX__;
    int segundo  = __INT_MAX__;

    for (int j = 0; j < collumns; j++) {
        if (col_blocked[j] || demanda_rest[j] == 0) continue;
        int c = custos[i][j];
        if (c < primeiro) {
            segundo  = primeiro;
            primeiro = c;
        } else if (c < segundo) {
            segundo = c;
        }
    }

    if (primeiro == __INT_MAX__) return -1; /* nenhuma coluna ativa */
    if (segundo  == __INT_MAX__) return 0;  /* apenas 1 coluna ativa, penalidade 0 */
    return segundo - primeiro;
}

/**
 * Calcula a penalidade de uma coluna j:
 *   diferença entre o 2º e o 1º menor custo nas linhas ativas.
 */
static int penalidade_coluna(int** custos, int* row_blocked, int rows, int collumns, int j,
                              int* oferta_rest, int* demanda_rest) {
    (void)collumns;
    if (demanda_rest[j] == 0) return -1; /* coluna esgotada */

    int primeiro = __INT_MAX__;
    int segundo  = __INT_MAX__;

    for (int i = 0; i < rows; i++) {
        if (row_blocked[i] || oferta_rest[i] == 0) continue;
        int c = custos[i][j];
        if (c < primeiro) {
            segundo  = primeiro;
            primeiro = c;
        } else if (c < segundo) {
            segundo = c;
        }
    }

    if (primeiro == __INT_MAX__) return -1;
    if (segundo  == __INT_MAX__) return 0;
    return segundo - primeiro;
}
/** Algoritmo de Vogel */
int** metodo_vogel(Matriz* matriz) {
    int rows = matriz->rows;
    int collumns = matriz->collumns;
    int** custos = matriz->matriz;

    /* Matriz de solução (quantidades alocadas) */
    int** matriz_solucao = create_matriz(rows, collumns);

    /* Cópias das ofertas e demandas para não modificar o problema original */
    int* oferta_rest = copy_vector(matriz->oferta, rows);
    int* demanda_rest = copy_vector(matriz->demanda, collumns);

    /* Esse é um vetor onde se o valor estiver como 0, a posição referente 
    a linha ou a coluna está desbloqueada, senão está bloqueada */
    int* row_blocked = (int*)calloc(rows, sizeof(int));
    int* col_blocked = (int*)calloc(collumns, sizeof(int));

    /* Fluxo principal */
    while (sum_vector(oferta_rest, rows) > 0 && sum_vector(demanda_rest, collumns) > 0) {

        // Calcula penalidades de todas as linhas e colunas ativas
        int melhor_pen = -1;
        int eh_linha = -1; /* 1 = é linha, 0 = é coluna */
        int melhor_idx = -1;

        /* Verificar linhas */
        for (int i = 0; i < rows; i++) {
            if (row_blocked[i] || oferta_rest[i] == 0) continue;
            int pen = penalidade_linha(custos, col_blocked, rows, collumns, i, oferta_rest, demanda_rest);
            if (pen > melhor_pen) {
                melhor_pen = pen;
                eh_linha = 1;
                melhor_idx = i;
            }
        }

        /* Verificar colunas */
        for (int j = 0; j < collumns; j++) {
            if (col_blocked[j] || demanda_rest[j] == 0) continue;
            int pen = penalidade_coluna(custos, row_blocked, rows, collumns, j, oferta_rest, demanda_rest);
            if (pen > melhor_pen) {
                melhor_pen = pen;
                eh_linha   = 0;
                melhor_idx = j;
            }
        }

        /*Deu ruim em tudo*/
        if (melhor_idx == -1) break;

        /* Encontrar a célula de menor custo na linha/coluna escolhida*/
        int aloc_row = -1;
        int aloc_col = -1;
        int min_custo = __INT_MAX__;

        if (eh_linha) {
            int i = melhor_idx;
            for (int j = 0; j < collumns; j++) {
                if (col_blocked[j] || demanda_rest[j] == 0) continue;
                if (custos[i][j] < min_custo) {
                    min_custo = custos[i][j];
                    aloc_row  = i;
                    aloc_col  = j;
                }
            }
        } else {
            int j = melhor_idx;
            for (int i = 0; i < rows; i++) {
                if (row_blocked[i] || oferta_rest[i] == 0) continue;
                if (custos[i][j] < min_custo) {
                    min_custo = custos[i][j];
                    aloc_row  = i;
                    aloc_col  = j;
                }
            }
        }

        if (aloc_row == -1 || aloc_col == -1) break;

        /* Alocar o máximo possível em (aloc_row, aloc_col) */
        int alocado = get_smaller_value(oferta_rest[aloc_row], demanda_rest[aloc_col]);
        matriz_solucao[aloc_row][aloc_col] += alocado;
        oferta_rest[aloc_row]  -= alocado;
        demanda_rest[aloc_col] -= alocado;

        /* Bloquear linha se oferta esgotada */
        if (oferta_rest[aloc_row] == 0) {
            row_blocked[aloc_row] = 1;
        }
        /* Bloquear coluna se demanda esgotada */
        if (demanda_rest[aloc_col] == 0) {
            col_blocked[aloc_col] = 1;
        }
    }

    int custo_total = calculate_total_cost(matriz_solucao, custos, rows, collumns);
    printf("\nCusto total Z (Vogel): %d", custo_total);

    free(oferta_rest);
    free(demanda_rest);
    free(row_blocked);
    free(col_blocked);

    return matriz_solucao;
}
