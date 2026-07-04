#ifndef VIEW_H
#define VIEW_H

#include "../matriz/matriz.h"

int get_rows();
int get_collumns();
int get_val_custo(int row, int collumn);
int get_val_oferta(int row);
int** get_matriz(int lines, int collumns);
void fill_problem(Matriz* matriz);
void print_problem_view(Matriz* matriz);
void print_error_balanced();

#endif // VIEW_H