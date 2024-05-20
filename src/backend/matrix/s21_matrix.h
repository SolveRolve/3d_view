// - Проверяемая точность дробной части - максимум 6 знаков после запятой.

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// #include "s21_auxiliary.h"

#define SUCCESS 1
#define FAILURE 0

#define EXP_EQ 1e-7

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int checkNull(matrix_t *);
