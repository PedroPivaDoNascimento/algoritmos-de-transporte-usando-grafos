#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../matriz/matriz.h"

Matriz* create_problem_controller();
void print_problem_controller(Matriz* matriz);
bool verify_is_balanced(Matriz* matriz);
void use_metodo_canto_noroeste(Matriz* matriz);
void use_metodo_custo_minimo(Matriz* matriz);

#endif // CONTROLLER_H