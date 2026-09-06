#ifndef VOGEL_H
#define VOGEL_H

#include "../matriz/matriz.h"

/**
 * Aplica o Método de Aproximação de Vogel (VAM) ao problema de transporte.
 *
 * Pré-condição: a matriz deve estar estritamente balanceada
 * (sum(oferta) == sum(demanda)).
 *
 * @param matriz  Ponteiro para a estrutura do problema de transporte.
 * @return        Matriz de solução com os custos ponderados (X_ij * C_ij),
 *                com dimensões [rows][collumns].
 */
int** metodo_vogel(Matriz* matriz);

#endif // VOGEL_H
