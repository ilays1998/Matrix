//
// Created by ilays on 05/11/2021.
//
#include "ex3.h"


void print_mat (matrix* mat)
{
  for (int i = 0; i < mat->n_rows; ++i)
    {
      for (int k = 0; k < mat->n_columns; ++k)
        {
          printf ("%f ", mat->data[i][k]);
        }
      printf ("\n");
    }
}
//
//int main()
//{
//    matrix* m = create_matrix (10, 5);
//    for (int i = 0; i < m->n_rows; i++)
//    {
//        for (int j = 0; j < m->n_columns; j++)
//        {
//          m->data[i][j] = i + 1;
//        }
//    }
////    matrix* d = create_matrix (10, 5);
////    for (int i = 0; i < d->n_rows; i++)
////    {
////        for (int j = 0; j < d->n_columns; j++)
////        {
////            d->data[i][j] = j;
////        }
////    }
//    print_mat (m);
//    double* arr = ravel (m);
//    for (int i = 0; i < (m->n_columns * m->n_rows); i++){
//        printf ("%f ", arr[i]);
//    }
//    printf ("\n");
//
//  //   print_mat (m);
////    printf ("%f", *get_by_index (m, 7, 4));
//    print_mat (m);
//
//    printf ("\n");
//    free_matrix (m);
//    free (arr);
//
//    return 0;
//}