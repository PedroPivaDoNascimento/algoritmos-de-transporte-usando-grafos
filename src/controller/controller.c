#include "../view/view.h"
#include "controller.h"
#include <stdlib.h>
#include <stdio.h>

Matriz* create_problem() {
    int rows = get_rows();
    int collumns = get_collumns();
    Matriz* matriz = create_matriz(rows, collumns);
    fill_problem(matriz);
    return matriz;
}

void print_problem_controller(Matriz* matriz) {
    print_problem_view(matriz);
}