#ifndef _SRC_BACKEND_
#define _SRC_BACKEND_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "./matrix/s21_matrix.h"

typedef struct {
  int v;
  int f;
  int f_count;
  double *v_data;
  unsigned int *f_data;
  double max_x, max_y, min_x, min_y;
} data_from_file;

typedef struct Data {
  unsigned long long count_of_vertexes;
  unsigned long long count_of_facets;  // строчки
  matrix_t VERTEXES;                   // 3 на н
  unsigned int *facets;
  double max_x, max_y, min_x, min_y;
  int cout_arg;  // строчки на штуки
} count_of_elements;

unsigned int *logic_numer(unsigned int *p_f, int *fl_minus,
                          unsigned int *first_f, int *Curr_num,
                          int *count_facets, int *flag);
int view(char *file, count_of_elements *DATA);
unsigned int *parsing_facets(FILE *opened, unsigned int *p_f, int count_facets);
int count_of_vf(FILE *opened, data_from_file *data);
int data_v_f(FILE *opened, data_from_file *data);
void copy_pasta(data_from_file data, count_of_elements *DATA);

void free_data(count_of_elements *DATA);
// АФФИННЫЕ ПРЕОБРАЗОВАНИЯ //

void move_x(matrix_t *FACETS, double a);
void move_y(matrix_t *FACETS, double a);
void move_z(matrix_t *FACETS, double a);
void rotation_by_ox(matrix_t *FACETS, double a);
void rotation_by_oy(matrix_t *FACETS, double a);
void rotation_by_oz(matrix_t *FACETS, double a);
void scaling(matrix_t *FACETS, double a);

#endif  // _SRC_BACKEND_
