#include "s21_3dviewer.h"

int view(char *file, count_of_elements *DATA) {
  int error = 0;      // | 0 okey | 1 eror | 2 net file | 3 error vertex |
  FILE *Opened_file;  // 4 error facets | 5 null struct |
  data_from_file data = {0};
  if (DATA != NULL) {
    Opened_file = fopen(file, "r");
    if (Opened_file != NULL) {
      error = count_of_vf(Opened_file, &data);
      rewind(Opened_file);
      error = data_v_f(Opened_file, &data);
      fclose(Opened_file);
      copy_pasta(data, DATA);
      double *ptr = data.v_data;
      int k = data.v;
      if (s21_create_matrix(4, k, &DATA->VERTEXES) == 0) {
        for (int i = 0; i < DATA->VERTEXES.columns; i++) {
          for (int j = 0; j < 4; j++) {
            if (j == 3) {
              DATA->VERTEXES.matrix[j][i] = 1;
            } else {
              DATA->VERTEXES.matrix[j][i] = *ptr;
              ptr++;
            }
          }
        }
      }
    } else
      error = 2;
  } else
    error = 5;
  if (data.f == 0 && !error) error = 4;
  if (data.v == 0 && !error) error = 3;

  if (data.v_data != NULL) {
    free(data.v_data);
    data.v_data = NULL;
  }
  return error;
}

int lenth_cur_str(FILE *opened) {
  char Curr_char = 0;
  int lenth = 0;
  while ((Curr_char = getc(opened)) != '\n' && Curr_char != 0 &&
         Curr_char != -1)
    lenth++;
  long POSITION_ON = ftell(opened) - lenth;
  fseek(opened, POSITION_ON, SEEK_SET);
  return lenth;
}

int count_of_vf(FILE *opened, data_from_file *data) {
  int error = 0, count_char = 0;
  char buffer_str[100] = {0};
  while ((count_char = fscanf(opened, "%s", buffer_str)) != -1) {
    if (strcmp(buffer_str, "v\0") == 0) data->v++;
    if (strcmp(buffer_str, "f\0") == 0) data->f++;
    bzero(buffer_str, 100);
  }
  return error;
}

int data_v_f(FILE *opened, data_from_file *data) {
  int value = 0, count_vertex = 0;
  char zero[100] = {0};
  data->v_data = calloc(3 * data->v, sizeof(double));
  double *ptr = (data->v_data);
  data->f_data = calloc(10 * data->f, sizeof(unsigned int));
  unsigned int *p_f = data->f_data;
  int k = 0, count_v = 0;
  while (fscanf(opened, "%s", zero) != -1) {
    k++;
    if (strcmp(zero, "v\0") == 0) {
      for (int i = 0; i < 3; i++, ptr++) {
        count_v += fscanf(opened, "%lf", ptr);
        if (i == 0) {
          if (data->max_x < *ptr) data->max_x = *ptr;
          if (data->min_x > *ptr) data->min_x = *ptr;
        }
        if (i == 1) {
          if (data->max_y < *ptr) data->max_y = *ptr;
          if (data->min_y > *ptr) data->min_y = *ptr;
        }
      }
      count_vertex++;
      if (count_v != 3) value = 3;
      count_v = 0;
    } else if (strcmp(zero, "f\0") == 0) {
      p_f = parsing_facets(opened, p_f, count_vertex);
    } else
      continue;
    bzero(zero, 100);  // зануление памяти в чаре
  }
  data->f_count = p_f - data->f_data;
  return value;
}

unsigned int *logic_numer(unsigned int *p_f, int *fl_minus,
                          unsigned int *first_f, int *Curr_num,
                          int *count_facets, int *flag) {
  if (*fl_minus) *Curr_num = *count_facets - *Curr_num + 1;
  *p_f = *Curr_num - 1;
  p_f++;
  (*flag)++;
  if (*flag > 1) {
    *p_f = *Curr_num - 1;
    p_f++;
  } else
    *first_f = *Curr_num;
  return p_f;
}
unsigned int *parsing_facets(FILE *opened, unsigned int *p_f,
                             int count_facets) {
  int Curr_char = 0, Curr_num = 0, flag = 0, fl_minus = 0, lenth = 0;
  unsigned int first_f = 0;
  lenth = lenth_cur_str(opened);
  while ((Curr_char = getc(opened)) != '\n' && Curr_char != 0 &&
         Curr_char != -1 && lenth > 0) {
    lenth--;
    if (Curr_char == ' ') {
      if (Curr_num)
        p_f = logic_numer(p_f, &fl_minus, &first_f, &Curr_num, &count_facets,
                          &flag);
      Curr_num = 0;
      continue;
    } else if (Curr_char == '/') {
      if (Curr_num)
        p_f = logic_numer(p_f, &fl_minus, &first_f, &Curr_num, &count_facets,
                          &flag);
      int s = 100;
      while (s != -1 && s != '\0' && s != '\n' && s != ' ') s = getc(opened);
      if (s == '\n') {
        long POSITION_ON = ftell(opened) - 1;
        fseek(opened, POSITION_ON, SEEK_SET);
        Curr_num = 0;
        break;
        flag = 0;
      }
      Curr_num = 0;
      continue;
    } else if (Curr_char == '-')
      fl_minus = 1;
    else if (Curr_char > 47 && Curr_char < 58) {
      Curr_num *= 10;
      Curr_num += Curr_char - 48;
    }
  }
  if (Curr_num)
    for (int i = 0; i < 2; i++, p_f++) *p_f = Curr_num - 1;
  Curr_num = 0;
  if (first_f) *p_f = first_f - 1, p_f++;
  return p_f;
}

void copy_pasta(data_from_file data, count_of_elements *DATA) {
  DATA->count_of_facets = data.f;
  DATA->count_of_vertexes = data.v;
  DATA->min_y = data.min_y;
  DATA->max_y = data.max_y;
  DATA->min_x = data.min_x;
  DATA->max_x = data.max_x;
  DATA->cout_arg = data.f_count;
  DATA->facets = data.f_data;
}

void free_data(count_of_elements *DATA) {
  if (DATA != NULL) {
    s21_remove_matrix(&(DATA->VERTEXES));

    if (DATA->facets != NULL) {
      free(DATA->facets);
      DATA->facets = NULL;
    }
    DATA->count_of_facets = 0;
    DATA->count_of_vertexes = 0;
    DATA->cout_arg = 0;
    DATA->max_x = 0;
    DATA->min_x = 0;
    DATA->max_y = 0;
    DATA->min_y = 0;
  }
}

void move_x(matrix_t *VERTEXS, double a) {
  for (int i = 0; i < VERTEXS->columns; i++) VERTEXS->matrix[0][i] += a;
}
void move_y(matrix_t *VERTEXS, double a) {
  for (int i = 0; i < VERTEXS->columns; i++) VERTEXS->matrix[1][i] += a;
}
void move_z(matrix_t *VERTEXS, double a) {
  for (int i = 0; i < VERTEXS->columns; i++) VERTEXS->matrix[2][i] += a;
}

void rotation_by_ox(matrix_t *VERTEXS, double a) {
  double rad = (a * M_PI) / 180;

  for (int i = 0; i < VERTEXS->columns; i++) {
    double Y = VERTEXS->matrix[1][i];
    VERTEXS->matrix[1][i] = Y * cos(rad) + VERTEXS->matrix[2][i] * sin(rad);
    VERTEXS->matrix[2][i] =
        (-1) * Y * sin(rad) + VERTEXS->matrix[2][i] * cos(rad);
  }
}
void rotation_by_oy(matrix_t *VERTEXS, double a) {
  double rad = (a * M_PI) / 180;
  for (int i = 0; i < VERTEXS->columns; i++) {
    double X = VERTEXS->matrix[0][i];
    VERTEXS->matrix[0][i] = X * cos(rad) + VERTEXS->matrix[2][i] * sin(rad);
    VERTEXS->matrix[2][i] =
        (-1) * X * sin(rad) + VERTEXS->matrix[2][i] * cos(rad);
  }
}

void rotation_by_oz(matrix_t *VERTEXS, double a) {
  double rad = (a * M_PI) / 180;
  for (int i = 0; i < VERTEXS->columns; i++) {
    double X = VERTEXS->matrix[0][i];
    VERTEXS->matrix[0][i] = X * cos(rad) + VERTEXS->matrix[1][i] * sin(rad);
    VERTEXS->matrix[1][i] =
        (-1) * X * sin(rad) + VERTEXS->matrix[1][i] * cos(rad);
  }
}

void scaling(matrix_t *VERTEXS, double a) {
  if (a == 0) a = 1;
  for (int i = 0; i < VERTEXS->columns; i++) {
    VERTEXS->matrix[0][i] *= a;
    VERTEXS->matrix[1][i] *= a;
    VERTEXS->matrix[2][i] *= a;
  }
}
