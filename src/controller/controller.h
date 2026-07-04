#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../matriz/matriz.h"

Matriz* create_problem_controller();
void print_problem_controller(Matriz* matriz);
void verify_is_balanced(Matriz* matriz);

#endif // CONTROLLER_H