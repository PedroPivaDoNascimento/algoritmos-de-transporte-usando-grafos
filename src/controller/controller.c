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

void verify_is_balanced(Matriz* matriz) {
    if (!is_balanced(matriz)) {
        print_error_balanced();
    }
}
