#include "ciclo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int row;        /* Linha atual        */
    int col;        /* Coluna atual       */
    int profund;    /* Profundidade (índice no ciclo) */
    int direcao;    /* 0 = mover horizontalmente, 1 = mover verticalmente */
} EstadoDFS;

static bool dfs(SolucaoTransporte* sol, VerticeCiclo* ciclo, int* tam, int row_in, int col_in, int cur_row, int cur_col, int direcao)
{
    int rows = sol->rows;
    int cols = sol->cols;

    if (direcao == 0) {
        /* Mover horizontalmente: percorre colunas na mesma linha cur_row */
        for (int j = 0; j < cols; j++) {
            if (j == cur_col) continue;

            /* Condição de fechamento: voltou à origem pelo eixo horizontal */
            if (cur_row == row_in && j == col_in && *tam >= 3) {
                /* Ciclo fechado! O vértice de partida já está em ciclo[0]. */
                return true;
            }

            /* Só pode fazer curva em célula básica */
            if (!sol->eh_basica[cur_row][j]) continue;

            /* Verificar se já visitamos esta célula neste caminho */
            bool visitada = false;
            for (int k = 1; k < *tam; k++) {
                if (ciclo[k].row == cur_row && ciclo[k].col == j) {
                    visitada = true;
                    break;
                }
            }
            if (visitada) continue;

            /* Adicionar vértice e continuar a DFS verticalmente */
            ciclo[*tam].row  = cur_row;
            ciclo[*tam].col  = j;
            ciclo[*tam].sinal = 0; /* sinais atribuídos depois */
            (*tam)++;

            if (dfs(sol, ciclo, tam, row_in, col_in, cur_row, j, 1))
                return true;

            (*tam)--;
        }
    } else {
        /* Mover verticalmente: percorre linhas na mesma coluna cur_col */
        for (int i = 0; i < rows; i++) {
            if (i == cur_row) continue;

            /* Condição de fechamento: voltou à origem pelo eixo vertical */
            if (i == row_in && cur_col == col_in && *tam >= 3) {
                return true;
            }

            /* Só pode fazer curva em célula básica */
            if (!sol->eh_basica[i][cur_col]) continue;

            /* Verificar se já visitamos esta célula neste caminho */
            bool visitada = false;
            for (int k = 1; k < *tam; k++) {
                if (ciclo[k].row == i && ciclo[k].col == cur_col) {
                    visitada = true;
                    break;
                }
            }
            if (visitada) continue;

            /* Adicionar vértice e continuar a DFS horizontalmente */
            ciclo[*tam].row  = i;
            ciclo[*tam].col  = cur_col;
            ciclo[*tam].sinal = 0;
            (*tam)++;

            if (dfs(sol, ciclo, tam, row_in, col_in, i, cur_col, 0))
                return true;

            (*tam)--;
        }
    }

    return false;
}

bool encontrar_ciclo_fechado(SolucaoTransporte* sol, int row_in, int col_in, VerticeCiclo* ciclo, int* tam) {
    *tam = 0;

    /* O ponto de partida é a célula de entrada (não é básica ainda) */
    ciclo[0].row   = row_in;
    ciclo[0].col   = col_in;
    ciclo[0].sinal = 0;
    *tam = 1;

    bool encontrado = dfs(sol, ciclo, tam, row_in, col_in, row_in, col_in, 0);

    if (!encontrado) {
        fprintf(stderr,
                "[ERRO] Ciclo fechado não encontrado para célula (%d,%d).\n",
                row_in, col_in);
        *tam = 0;
        return false;
    }

    /* Atribuir sinais alternados: + para índices pares, - para ímpares */
    for (int k = 0; k < *tam; k++) {
        ciclo[k].sinal = (k % 2 == 0) ? +1 : -1;
    }

    return true;
}

void redistribuir_carga(SolucaoTransporte* sol, VerticeCiclo* ciclo, int tam)
{
    /* 1. Encontrar theta = min(fluxo) entre células com sinal - */
    int theta  = INT_MAX;
    int saida_idx = -1; /* índice no ciclo do vértice que sairá da base */

    for (int k = 0; k < tam; k++) {
        if (ciclo[k].sinal == -1) {
            int fluxo = sol->fluxo[ciclo[k].row][ciclo[k].col];
            if (fluxo < theta) {
                theta     = fluxo;
                saida_idx = k;
            }
        }
    }

    /* 2. Redistribuir carga */
    for (int k = 0; k < tam; k++) {
        int r = ciclo[k].row;
        int c = ciclo[k].col;
        sol->fluxo[r][c] += ciclo[k].sinal * theta;
    }

    /* 3. Atualizar base: célula de entrada passa a ser básica */
    sol->eh_basica[ciclo[0].row][ciclo[0].col] = true;

    /* 4. Célula que forneceu theta (sinal -) sai da base */
    if (saida_idx != -1) {
        int r = ciclo[saida_idx].row;
        int c = ciclo[saida_idx].col;
        sol->eh_basica[r][c] = false;
        sol->fluxo[r][c] = 0;  /* garante que não sobra resíduo */
    }
}
