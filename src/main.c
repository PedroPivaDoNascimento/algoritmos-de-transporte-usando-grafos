#include <stdio.h>
#include <stdlib.h>
#include "controller/controller.h"
#include "view/view.h"



int main() {
    Matriz* matriz = create_problem();
    verify_is_balanced(matriz);
    print_problem_controller(matriz);
    return 0;
}