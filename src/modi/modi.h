#ifndef MODI_H
#define MODI_H

#include "../matriz/matriz.h"
#include <stdbool.h>


typedef struct {
    int** fluxo;       /* Matriz de fluxo x_ij (quantidades transportadas) */
    bool** eh_basica;  /* true se (i,j) é variável básica (mesmo que fluxo == 0) */
    int rows;          /* Número de fornecedores (m) */
    int cols;          /* Número de consumidores (n)  */
} SolucaoTransporte;


SolucaoTransporte* criar_solucao_transporte(int** fluxo, int** custos, int rows, int cols);

void free_solucao_transporte(SolucaoTransporte* sol);

void resolver_problema_transporte(Matriz* bal, int** fluxo_inicial);

#endif /* MODI_H */
