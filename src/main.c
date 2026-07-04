#include <stdio.h>
#include <stdlib.h>
#include "controller/controller.h"
#include "view/view.h"
#include "matriz/matriz.h"



int main() {
    Matriz* matriz = create_problem_controller();
    print_problem_controller(matriz);
    use_metodo_canto_noroeste(matriz);
    return 0;
}