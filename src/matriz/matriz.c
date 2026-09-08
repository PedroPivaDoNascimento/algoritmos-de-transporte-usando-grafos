#include "matriz.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "../view/view.h"

int** create_matriz(int rows, int collumns) {
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)calloc(collumns, sizeof(int));
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

void free_matriz(Matriz* m) {
    if (!m) return;
    if (m->matriz) {
        for (int i = 0; i < m->rows; i++) {
            free(m->matriz[i]);
        }
        free(m->matriz);
    }
    free(m->oferta);
    free(m->demanda);
    free(m);
}

bool is_balanced(Matriz* matriz) {
    int sum_oferta  = sum_vector(matriz->oferta,  matriz->rows);
    int sum_demanda = sum_vector(matriz->demanda, matriz->collumns);
    return sum_oferta == sum_demanda;
}

Matriz* balance_matriz(Matriz* m, int* ghost_added) {
    int total_oferta  = sum_vector(m->oferta,  m->rows);
    int total_demanda = sum_vector(m->demanda, m->collumns);
    int diff = total_oferta - total_demanda;

    int new_rows = m->rows;
    int new_cols = m->collumns;

    if (diff > 0) {
        new_cols = m->collumns + 1;          /* Consumidor fantasma */
        if (ghost_added) *ghost_added = 1;
    } else if (diff < 0) {
        new_rows = m->rows + 1;              /* Fornecedor fantasma */
        if (ghost_added) *ghost_added = -1;
    } else {
        if (ghost_added) *ghost_added = 0;
    }

    Matriz* bal = create_problem_matriz(new_rows, new_cols);

    /* Copiar custos originais (novas linhas/colunas ficam com 0 via calloc) */
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->collumns; j++) {
            bal->matriz[i][j] = m->matriz[i][j];
        }
    }

    /* Copiar ofertas originais */
    for (int i = 0; i < m->rows; i++) {
        bal->oferta[i] = m->oferta[i];
    }
    /* Copiar demandas originais */
    for (int j = 0; j < m->collumns; j++) {
        bal->demanda[j] = m->demanda[j];
    }

    /* Ajustar nó fictício */
    if (diff > 0) {
        bal->demanda[new_cols - 1] = diff;  /* consumidor: demanda = excesso oferta */
    } else if (diff < 0) {
        bal->oferta[new_rows - 1]  = -diff; /* fornecedor: oferta  = excesso demanda */
    }

    return bal;
}

int calculate_total_cost(int** matriz_solucao, int** custos, int rows, int collumns) {
    int total_cost = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collumns; j++) {
            total_cost += matriz_solucao[i][j] * custos[i][j];
        }
    }
    return total_cost;
}

int** metodo_canto_noroeste(Matriz* matriz) {
    int ghost_added;
    Matriz* bal = balance_matriz(matriz, &ghost_added);

    int rows = bal->rows;
    int cols = bal->collumns;

    int** matriz_solucao = create_matriz(rows, cols);
    int* copy_oferta  = copy_vector(bal->oferta, rows);
    int* copy_demanda = copy_vector(bal->demanda, cols);

    /* Número de variáveis básicas esperado */
    int basicas_necessarias = rows + cols - 1;
    int basicas_feitas = 0;

    int i = 0, j = 0;
    while (i < rows && j < cols) {
        int alocado = get_smaller_value(copy_oferta[i], copy_demanda[j]);
        matriz_solucao[i][j] = alocado;
        basicas_feitas++;

        copy_oferta[i] -= alocado;
        copy_demanda[j] -= alocado;

        if (copy_oferta[i] == 0 && copy_demanda[j] == 0) {
            i++;
            if (i < rows && j < cols && basicas_feitas < basicas_necessarias) {
                matriz_solucao[i][j] = 0;
                basicas_feitas++;
            }
        } else if (copy_oferta[i] == 0) {
            i++;
        } else {
            j++;
        }
    }

    free(copy_oferta);
    free(copy_demanda);

    int custo_total = calculate_total_cost(matriz_solucao, bal->matriz, rows, cols);
    printf("\nCusto total Z (Canto Noroeste): %d", custo_total);

    if (ghost_added == 1) {
        printf("\n[INFO] Consumidor fantasma adicionado (coluna %d) para balancear a matriz.", cols);
    } else if (ghost_added == -1) {
        printf("\n[INFO] Fornecedor fantasma adicionado (linha %d) para balancear a matriz.", rows);
    }

    free_matriz(bal);
    return matriz_solucao;
}

int** metodo_custo_minimo(Matriz* matriz) {
    int ghost_added;
    Matriz* bal = balance_matriz(matriz, &ghost_added);

    int rows = bal->rows;
    int cols = bal->collumns;

    int** matriz_solucao = create_matriz(rows, cols);
    int* copy_oferta  = copy_vector(bal->oferta,  rows);
    int* copy_demanda = copy_vector(bal->demanda, cols);

    while (sum_vector(copy_oferta, rows) > 0 && sum_vector(copy_demanda, cols) > 0) {
        int min_cost = __INT_MAX__;
        int min_row  = -1;
        int min_col  = -1;
        int max_aloc = -1; 

        int ghost_row = (ghost_added == -1) ? (rows - 1) : -1;
        int ghost_col = (ghost_added == 1)  ? (cols - 1) : -1;

        int tem_rota_real = 0;
        for (int i = 0; i < rows && !tem_rota_real; i++) {
            if (i == ghost_row || copy_oferta[i] <= 0) continue;
            for (int j = 0; j < cols; j++) {
                if (j == ghost_col || copy_demanda[j] <= 0) continue;
                tem_rota_real = 1;
                break;
            }
        }

        for (int i = 0; i < rows; i++) {
            if (copy_oferta[i] <= 0) continue;
            int eh_linha_fantasma = (i == ghost_row);

            for (int j = 0; j < cols; j++) {
                if (copy_demanda[j] <= 0) continue;
                int eh_coluna_fantasma = (j == ghost_col);
                int eh_ficticia = eh_linha_fantasma || eh_coluna_fantasma;

                if (tem_rota_real && eh_ficticia) continue;

                int custo = bal->matriz[i][j];
                int aloc  = get_smaller_value(copy_oferta[i], copy_demanda[j]);

                if (custo < min_cost) {
                    min_cost = custo;
                    min_row  = i;
                    min_col  = j;
                    max_aloc = aloc;
                } else if (custo == min_cost) {
                    /* Desempate 1: maior alocação possível */
                    if (aloc > max_aloc) {
                        min_row  = i;
                        min_col  = j;
                        max_aloc = aloc;
                    }
                }
            }
        }

        if (min_row == -1 || min_col == -1) break;

        int alocado = get_smaller_value(copy_oferta[min_row], copy_demanda[min_col]);
        matriz_solucao[min_row][min_col] = alocado;

        copy_oferta[min_row] -= alocado;
        copy_demanda[min_col] -= alocado;

        if (copy_oferta[min_row] == 0 && copy_demanda[min_col] == 0) {
            int found = 0;
            for (int j2 = 0; j2 < cols && !found; j2++) {
                if (j2 != min_col && copy_demanda[j2] > 0 && matriz_solucao[min_row][j2] == 0) {
                    matriz_solucao[min_row][j2] = 0;
                    found = 1;
                }
            }
            if (!found) {
                for (int i2 = 0; i2 < rows && !found; i2++) {
                    if (i2 != min_row && copy_oferta[i2] > 0 && matriz_solucao[i2][min_col] == 0) {
                        matriz_solucao[i2][min_col] = 0;
                        found = 1;
                    }
                }
            }
        }
    }

    free(copy_oferta);
    free(copy_demanda);

    int custo_total = calculate_total_cost(matriz_solucao, bal->matriz, rows, cols);
    printf("\nCusto total Z (Custo Mínimo): %d", custo_total);

    if (ghost_added == 1) {
        printf("\n[INFO] Consumidor fantasma adicionado (coluna %d) para balancear a matriz.", cols);
    } else if (ghost_added == -1) {
        printf("\n[INFO] Fornecedor fantasma adicionado (linha %d) para balancear a matriz.", rows);
    }

    free_matriz(bal);
    return matriz_solucao;
}



