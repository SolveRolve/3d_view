#include "s21_matrix.h"
// #include "s21_auxiliary.h"

/*

Все операции (кроме сравнения матриц) должны возвращать результирующий код:
- 0 - OK
- 1 - Ошибка, некорректная матрица
- 2 - Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой
нельзя провести вычисления и т.д.)

*/

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int code = 1;
  int memory_error = 0;
  if (rows <= 0 || columns <= 0) return code;
  result->matrix = NULL;
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix != NULL) {
    for (int i = 0; i < rows && memory_error == 0; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        for (int j = 0; j < i; j++) free(result->matrix[j]);

        memory_error = 1;
      }
    }
  } else
    memory_error = 1;

  if (memory_error == 0) {
    code = 0;
    result->columns = columns;
    result->rows = rows;
  } else {
    code = 1;
  }

  return code;
}

void s21_remove_matrix(matrix_t *A) {
  if (!checkNull(A)) {
    for (int i = 0; i < A->rows; i++)
      if (A->matrix[i] != NULL) {
        free(A->matrix[i]);
        A->matrix[i] = NULL;
      }

    A->rows = 0;
    A->columns = 0;
    free(A->matrix);
    A->matrix = NULL;
  }
}

int checkNull(matrix_t *A) {
  int code = 0;  // 1 - incorrect
  if (A != NULL) {
    if (A->rows < 1 || A->columns < 1 || A->matrix == NULL) {
      code = 1;
    }
  } else {
    code = 1;
  }

  return code;
}
