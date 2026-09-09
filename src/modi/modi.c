#include "modi.h"
#include "ciclo.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

static void calcular_multiplicadores(SolucaoTransporte* sol, int** custos,double* u, double* v) {
    int rows = sol->rows;
    int cols = sol->cols;

    /* Inicializar todos como NAN usando sentinela INT_MIN / 2 */
    const double NAO_DEF = (double)INT_MIN;

    for (int i = 0; i < rows; i++) u[i] = NAO_DEF;
    for (int j = 0; j < cols; j++) v[j] = NAO_DEF;

    /* Fixar u[0] = 0 */
    u[0] = 0.0;

    /* Propagar enquanto houver algum novo multiplicador a calcular */
    bool progresso = true;
    while (progresso) {
        progresso = false;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (!sol->eh_basica[i][j]) continue;

                double cij = (double)custos[i][j];

                if (u[i] != NAO_DEF && v[j] == NAO_DEF) {
                    v[j] = cij - u[i];
                    progresso = true;
                } else if (v[j] != NAO_DEF && u[i] == NAO_DEF) {
                    u[i] = cij - v[j];
                    progresso = true;
                }
            }
        }
    }
}

static bool verificar_otimalidade(SolucaoTransporte* sol, int** custos, double* u, double* v, int* row_in, int* col_in) {
    int rows = sol->rows;
    int cols = sol->cols;

    *row_in = -1;
    *col_in = -1;

    double max_c_bar = 0.0;  /* Limiar: só interessa se > 0 */

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (sol->eh_basica[i][j]) continue;

            double c_bar = u[i] + v[j] - (double)custos[i][j];
            if (c_bar > max_c_bar) {
                max_c_bar = c_bar;
                *row_in = i;
                *col_in = j;
            }
        }
    }

    return (*row_in == -1); /* true = ótimo */
}

/**
 * Imprime a solução atual (fluxos e marcação de básicas).
 */
static void imprimir_solucao(SolucaoTransporte* sol, int** custos) {
    int rows = sol->rows;
    int cols = sol->cols;
    int custo = 0;

    printf("  Solucao atual (x_ij):\n");
    for (int i = 0; i < rows; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            if (sol->eh_basica[i][j])
                printf("[%3d*]\t", sol->fluxo[i][j]);
            else
                printf("   -  \t");
        }
        printf("\n");
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            custo += sol->fluxo[i][j] * custos[i][j];

    printf(" Custo atual Z = %d\n", custo);
}

SolucaoTransporte* criar_solucao_transporte(int** fluxo, int** custos, int rows, int cols) {
    (void)custos; 

    SolucaoTransporte* sol = (SolucaoTransporte*)malloc(sizeof(SolucaoTransporte));
    sol->rows = rows;
    sol->cols = cols;

    /* Alocar e copiar matriz de fluxo */
    sol->fluxo = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        sol->fluxo[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
            sol->fluxo[i][j] = fluxo[i][j];
    }

    /* Alocar e inicializar mapa de básicas */
    sol->eh_basica = (bool**)malloc(rows * sizeof(bool*));
    for (int i = 0; i < rows; i++) {
        sol->eh_basica[i] = (bool*)calloc(cols, sizeof(bool));
    }

    /*marcar como básica toda célula com fluxo > 0 */
    int basicas = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fluxo[i][j] > 0) {
                sol->eh_basica[i][j] = true;
                basicas++;
            }
        }
    }
    
    int necessarias = rows + cols - 1;
    if (basicas < necessarias) {
        for (int i = 0; i < rows && basicas < necessarias; i++) {
            for (int j = 0; j < cols && basicas < necessarias; j++) {
                if (sol->eh_basica[i][j]) continue;

                /* Verifica se a linha i ou a coluna j tem alguma básica */
                bool linha_tem = false;
                bool coluna_tem = false;
                for (int k = 0; k < cols; k++)
                    if (sol->eh_basica[i][k]) { linha_tem = true; break; }
                for (int k = 0; k < rows; k++)
                    if (sol->eh_basica[k][j]) { coluna_tem = true; break; }

                if (linha_tem != coluna_tem) {
                    sol->eh_basica[i][j] = true;
                    sol->fluxo[i][j] = 0;
                    basicas++;
                }
            }
        }

        /* se ainda faltar, adicionar em qualquer célula não básica */
        for (int i = 0; i < rows && basicas < necessarias; i++) {
            for (int j = 0; j < cols && basicas < necessarias; j++) {
                if (!sol->eh_basica[i][j]) {
                    sol->eh_basica[i][j] = true;
                    sol->fluxo[i][j] = 0;
                    basicas++;
                }
            }
        }
    }

    return sol;
}

void free_solucao_transporte(SolucaoTransporte* sol)
{
    if (!sol) return;
    for (int i = 0; i < sol->rows; i++) {
        free(sol->fluxo[i]);
        free(sol->eh_basica[i]);
    }
    free(sol->fluxo);
    free(sol->eh_basica);
    free(sol);
}

void resolver_problema_transporte(Matriz* bal, int** fluxo_inicial) {
    int rows = bal->rows;
    int cols = bal->collumns;
    int** custos = bal->matriz;

    SolucaoTransporte* sol = criar_solucao_transporte(fluxo_inicial, custos, rows, cols);

    /* Alocar vetores de multiplicadores */
    double* u = (double*)malloc(rows * sizeof(double));
    double* v = (double*)malloc(cols * sizeof(double));

    /* Alocar buffer para o ciclo (tamanho máximo: rows*cols vértices) */
    VerticeCiclo* ciclo = (VerticeCiclo*)malloc(rows * cols * sizeof(VerticeCiclo));

    int iteracao = 0;
    bool otimo = false;

    while (!otimo) {
        iteracao++;

        /*calcular multiplicadores */
        calcular_multiplicadores(sol, custos, u, v);

        /* verificar otimalidade */
        int row_in, col_in;
        otimo = verificar_otimalidade(sol, custos, u, v, &row_in, &col_in);

        if (otimo) {
            printf("\n>>> Solucao OTIMA encontrada na iteracao %d! <<<\n", iteracao);
            break;
        }


        /* encontrar ciclo fechado */
        int tam_ciclo = 0;
        bool ok = encontrar_ciclo_fechado(sol, row_in, col_in, ciclo, &tam_ciclo);
        if (!ok) {
            fprintf(stderr, "[ERRO] Falha ao encontrar ciclo. Encerrando.\n");
            break;
        }

        /* Passo 4: redistribuir carga */
        redistribuir_carga(sol, ciclo, tam_ciclo);
    }

    /* Calcular e exibir custo final */
    int custo_final = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            custo_final += sol->fluxo[i][j] * custos[i][j];

    printf("\n  Solucao otima final:\n");
    imprimir_solucao(sol, custos);
    printf("\n=== Custo Total Minimo Z* = %d ===\n\n", custo_final);

    free(u);
    free(v);
    free(ciclo);
    free_solucao_transporte(sol);
}
