#include "../view/view.h"
#include "controller.h"
#include <stdlib.h>
#include <stdio.h>

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

bool verify_is_balanced(Matriz* matriz) {
    if (!is_balanced(matriz)) {
        print_error_balanced();
        return false;
    }
    return true;
}

void use_metodo_canto_noroeste(Matriz* matriz) {
    if (!verify_is_balanced(matriz)) {
        return;
    }
    int** matriz_solucao = metodo_canto_noroeste(matriz);
    print_matriz(matriz_solucao, matriz->rows, matriz->collumns, "Matriz de solução usando o método canto noroeste:");
}
