
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct matrix {
    double **data;
    size_t n_rows;
    size_t n_columns;
} matrix;

/**
 * create a new matrix_s on the heap.
 * @param n_rows number of rows in the new matrix_s.
 * @param n_columns number of columns in the new matrix_s.
 * @return A pointer to the new matrix_s struct.
 */
matrix *create_matrix (size_t n_rows, size_t n_columns);

/**
 * Get value based on index.
 * @param matrix
 * @param row_index
 * @param col_index
 * @return matrix_s[row_index, col_index],
 * NULL if the index is out of range.
 */
double *get_by_index (matrix *mat, size_t row_index, size_t col_index);

/**
 * return part of the matrix according to the slice index
 * @param matrix
 * @param row_range
 * @param col_range
 * @return A pointer to new matrix sliced
 */
matrix *slice (matrix *mat, size_t row_start,
               size_t row_end, size_t col_start, size_t col_end);

/**
 * create new matrix by concatenate 2 matrix - top on bottom
 * (according to rows)
 * @param top
 * @param bottom
 * @return A pointer to the new concatenate matrix struct.
 * NULL if the concatenate illegal
 */
matrix *concatenate_vertically (matrix *top, matrix *bottom);

/**
 * create new matrix by concatenate 2 matrix - right left
 * (according to columns)
 * @param left
 * @param right
 * @return A pointer to the new concatenate matrix struct.
 * NULL if the concatenate illegal
 */
matrix *concatenate_horizontally (matrix *left, matrix *right);

/**
 * get matrix and return matrix transpose by new matrix
 * @param matrix
 * @return A pointer to the new transpose matrix
 * NULL if the input matrix illegal
 */
matrix *transpose (matrix *mat);

/**
 * return array with all the data in the matrix (order)
 * @param matrix
 * @return A pointer to array
 */
double *ravel (matrix *mat);

/**
 *  free the allocated memory in the heap
 * @param matrix
 */
void free_matrix (matrix *mat);

#endif //_MATRIX_H_
