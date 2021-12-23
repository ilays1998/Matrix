//
// Created by ilays on 05/11/2021.
//

#include "ex3.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ERROR_MATRIX_ALLOCATION "ERROR: The matrix allocate memory fail\n"
#define EMPTY_MATRIX 0
#define ERROR_EMPTY_MATRIX "ERROR: The matrix empty, can't find index\n"
#define ERROR_ILLEGAL_INDEX "ERROR: The input index is out off the matrix limits\n"
#define ERROR_INPUT_MATRIX "ERROR: The input is invalid matrix\n"
#define ERROR_UNEQUAL_COLUMNS "ERROR: The number of columns unequal in "\
                      "the two matrix\n"

static void copy_2_mat_data (matrix** conca_mat, matrix *top, matrix *bottom);

static void copy_mat_data (matrix** conca_mat, matrix *to_copy);

/**
 * create a new matrix_s on the heap.
 * @param n_rows
 * @param n_columns
 * @return A pointer to the new matrix_s struct.
 */
matrix *create_matrix (size_t n_rows, size_t n_columns)
{
    matrix* new_mat = malloc(sizeof(matrix));
    if (new_mat == NULL)
    {
        fprintf(stderr, ERROR_MATRIX_ALLOCATION);
        return NULL;
    }
    new_mat->n_rows = n_rows;
    new_mat->n_columns = n_columns;
    if (n_rows == EMPTY_MATRIX || n_columns == EMPTY_MATRIX)
    {
        new_mat->data = NULL;
        new_mat->n_rows = EMPTY_MATRIX;
        new_mat->n_columns = EMPTY_MATRIX;
    }
    else
    {
        new_mat->data = malloc (n_rows * sizeof(double));
        if (new_mat->data == NULL)
        {
            fprintf (stderr, ERROR_MATRIX_ALLOCATION);
            free (new_mat);
            return NULL;
        }
        for (size_t i = 0; i < n_rows; i++)
        {
            new_mat->data[i] = malloc (n_columns * sizeof (double));
            if (new_mat->data[i] == NULL)
            {
                fprintf (stderr, ERROR_MATRIX_ALLOCATION);
                for (size_t j = 0; j < i; j++)
                {
                    free(new_mat->data[j]);
                }
                free(new_mat->data);
                free (new_mat);
                return NULL;
            }
            for (size_t k = 0; k < n_columns; k++)
            {
                new_mat->data[i][k] = 0.0;
            }
        }
    }
    return new_mat;
}

/**
 * Get value based on index.
 * @param mat
 * @param row_index
 * @param col_index
 * @return matrix_s[row_index, col_index],
 * NULL if the index is out of range.
 */
double *get_by_index (matrix *mat, size_t row_index, size_t col_index)
{
    if (mat == NULL)
    {
        fprintf (stderr, ERROR_INPUT_MATRIX);
        return NULL;
    }
    if (mat->data == NULL)
    {
        fprintf (stderr, ERROR_EMPTY_MATRIX);
        return NULL;
    }
    if (col_index >= mat->n_columns || row_index >= mat->n_rows)
    {
        fprintf (stderr, ERROR_ILLEGAL_INDEX);
        return NULL;
    }
    return &mat->data[row_index][col_index];
}

/**
 * return part of the matrix according to the slice index
 * @param matrix
 * @param row_range
 * @param col_range
 * @return A pointer to new matrix sliced
 */
matrix *slice (matrix *mat, size_t row_start,
               size_t row_end, size_t col_start, size_t col_end)
{
    matrix* slice_mat = malloc(sizeof(matrix));
    if (slice_mat == NULL)
    {
        fprintf(stderr, ERROR_MATRIX_ALLOCATION);
        return NULL;
    }
    if (row_end <= row_start || col_end <= col_start ||
                mat->n_rows == 0 || mat->n_columns == 0 ||
                row_start >= mat->n_rows || col_start >= mat->n_columns)
    {
        slice_mat->data = NULL;
        slice_mat->n_rows = 0;
        slice_mat->n_columns = 0;
        return slice_mat;
    }
    size_t max_row = (row_end > mat->n_rows) ? mat->n_rows : row_end;
    size_t max_col = (col_end > mat->n_columns) ? mat->n_columns : col_end;
    slice_mat->n_rows = max_row - row_start;
    slice_mat->n_columns = max_col - col_start;
    slice_mat->data = malloc (slice_mat->n_rows * sizeof (double));
    for (size_t i = 0; i < slice_mat->n_rows; i++)
    {
        slice_mat->data[i] = malloc (slice_mat->n_columns * sizeof (double));
        if (slice_mat->data[i] == NULL)
        {
            fprintf (stderr, ERROR_MATRIX_ALLOCATION);
            return NULL;
        }
        for (size_t k = 0; k < slice_mat->n_columns; k++)
        {
            slice_mat->data[i][k] = mat->data[i + row_start][k + col_start];
        }
    }
    return slice_mat;
}

/**
 * create new matrix by concatenate 2 matrix - top on bottom (according to rows)
 * @param top
 * @param bottom
 * @return A pointer to the new concatenate matrix struct.
 * NULL if the concatenate illegal
 */
matrix *concatenate_vertically (matrix *top, matrix *bottom)
{
    if (top == NULL || bottom == NULL)
    {
      fprintf (stderr, ERROR_INPUT_MATRIX);
      return NULL;
    }
    if (top->data == NULL && bottom->data == NULL)
    {
      matrix* conca_mat = create_matrix (EMPTY_MATRIX, EMPTY_MATRIX);
      return conca_mat;
    }
    if (top->n_columns == EMPTY_MATRIX)
    {
      matrix* conca_mat = create_matrix (bottom->n_rows, bottom->n_columns);
      copy_mat_data (&conca_mat, bottom);
      return conca_mat;
    }
    if (bottom->n_rows == EMPTY_MATRIX)
    {
      matrix* conca_mat = create_matrix (top->n_rows, top->n_columns);
      copy_mat_data (&conca_mat, top);
      return conca_mat;
    }
    if (top->n_columns != bottom->n_columns)
    {
        fprintf (stderr, ERROR_UNEQUAL_COLUMNS);
        return NULL;
    }
    matrix* conca_mat = create_matrix ((top->n_rows + bottom->n_rows), top->n_columns);
    //check if the matrix empty
    if (conca_mat != NULL && conca_mat->data != NULL)
    {
        copy_2_mat_data(&conca_mat, top, bottom);
    }
    return conca_mat;
}
/**
 * copy the data from 2 matrix to conca_mat (vertically)
 * @param conca_mat
 * @param top
 * @param bottom
 */
static void copy_2_mat_data (matrix** conca_mat, matrix *top, matrix *bottom)
{
    //copy the top matrix data to the conca matrix
    for (size_t i = 0; i < top->n_rows; i++)
    {
        for (size_t j = 0; j < top->n_columns; j++)
        {
            (*conca_mat)->data[i][j] = top->data[i][j];
        }
    }

    //copy the bottom matrix data to the conca matrix
    for (size_t i = top->n_rows; i < (*conca_mat)->n_rows; i++)
    {
        for (size_t j = 0; j < top->n_columns; j++)
        {
            (*conca_mat)->data[i][j] = bottom->data[i - top->n_rows][j];
        }
    }
}

static void copy_mat_data (matrix** conca_mat, matrix *to_copy)
{
    for (size_t i = 0; i < to_copy->n_rows; i++)
    {
        for (size_t j = 0; j < to_copy->n_columns; j++)
        {
            (*conca_mat)->data[i][j] = to_copy->data[i][j];
        }
    }
}

/**
 * create new matrix by concatenate 2 matrix - right left (according to columns)
 * @param left
 * @param right
 * @return A pointer to the new concatenate matrix struct.
 * NULL if the concatenate illegal
 */
matrix *concatenate_horizontally (matrix *left, matrix *right)
{
    if (left == NULL || right == NULL)
    {
      fprintf (stderr, ERROR_INPUT_MATRIX);
      return NULL;
    }
    if (left->data == NULL && right->data == NULL)
    {
        matrix* conca_mat = create_matrix (EMPTY_MATRIX, EMPTY_MATRIX);
        return conca_mat;
    }
    if (left->n_columns == EMPTY_MATRIX)
    {
        matrix* conca_mat = create_matrix (right->n_rows, right->n_columns);
        copy_mat_data (&conca_mat, right);
        return conca_mat;
    }
    if (right->n_rows == EMPTY_MATRIX)
    {
        matrix* conca_mat = create_matrix (left->n_rows, left->n_columns);
        copy_mat_data (&conca_mat, left);
        return conca_mat;
    }
    if (left->n_rows != right->n_rows)
    {
      fprintf (stderr, ERROR_UNEQUAL_COLUMNS);
      return NULL;
    }
    matrix* trans_left = transpose (left);
    matrix* trans_right = transpose (right);

    //the transpose concatenate matrix
    matrix* tmp = concatenate_vertically (trans_left, trans_right);

    matrix* conca_hor = transpose (tmp);

    //free the transpose matrix
    if (trans_left != NULL)
    {
        free_matrix (trans_left);
    }
    if (trans_right != NULL)
    {
        free_matrix (trans_right);
    }
    if (tmp != NULL)
    {
          free_matrix (tmp);
    }
    return conca_hor;
}

/**
 * get matrix and return matrix transpose by new matrix
 * @param matrix
 * @return A pointer to the new transpose matrix
 * NULL if the input matrix illegal
 */
matrix *transpose (matrix *mat)
{
    if (mat == NULL)
    {
        fprintf(stderr, ERROR_INPUT_MATRIX);
        return NULL;
    }
    matrix* trans = create_matrix (mat->n_columns, mat->n_rows);
    if (trans == NULL || trans->data == NULL)
    {
        return trans;
    }
    for (size_t i = 0; i < trans->n_rows; i++)
    {
        for (size_t j = 0; j < trans->n_columns; j++)
        {
            trans->data[i][j] = mat->data[j][i];
        }
    }
    return trans;
}

/**
 * return array with all the data in the matrix (order)
 * @param matrix
 * @return A pointer to array
 */
double *ravel (matrix *mat)
{
    if (mat == NULL)
    {
        fprintf(stderr, ERROR_INPUT_MATRIX);
        return NULL;
    }
    if (mat->n_rows == EMPTY_MATRIX || mat->n_columns == EMPTY_MATRIX||
          mat->data == NULL)
    {
        return NULL;
    }
    double* arr = malloc ((mat->n_rows * mat->n_columns) * sizeof(double));
    int count = 0;
    for (size_t i = 0; i < mat->n_rows; i++)
    {
        for (size_t j = 0; j < mat->n_columns; j++)
        {
            arr[count] = mat->data[i][j];
            count++;
        }
    }
    return arr;
}


/**
 *  free the allocated memory in the heap
 * @param matrix
 */
void free_matrix (matrix *mat)
{
    if (mat == NULL)
    {
        fprintf (stderr, ERROR_INPUT_MATRIX);
        return;
    }
    for (size_t i = 0; i < mat->n_rows; ++i)
    {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
    mat = NULL;
}

