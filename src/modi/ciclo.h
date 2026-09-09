#ifndef CICLO_H
#define CICLO_H

#include "modi.h"
#include <stdbool.h>

typedef struct {
    int row;   /* Linha da célula   */
    int col;   /* Coluna da célula  */
    int sinal; /* +1 ou -1          */
} VerticeCiclo;

bool encontrar_ciclo_fechado(SolucaoTransporte* sol, int row_in, int col_in, VerticeCiclo* ciclo, int* tam);

void redistribuir_carga(SolucaoTransporte* sol, VerticeCiclo* ciclo, int tam);

#endif /* CICLO_H */
