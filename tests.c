#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_runner.h"
#include "ex3.h"

void print_matrix(matrix *mat) {
    if (mat->data == NULL) {
        printf("data is NULL, n_rows: %lu, n_columns: %lu\n",
               mat->n_rows,
               mat->n_columns);
        return;
    }
    for (size_t i = 0; i < mat->n_rows; i++) {
        for (size_t j = 0; j < mat->n_columns; j++) {
            printf("%.2lf ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int
is_matrix_equal(matrix *mat, double *data, size_t n_rows, size_t n_columns) {
    if (mat->n_rows != n_rows || mat->n_columns != n_columns) return 0;
    if (data == NULL) {
        return mat->data == NULL;
    } else if (mat->data == NULL) {
        return 0;
    }

    for (size_t i = 0; i < n_rows; i++) {
        for (size_t j = 0; j < n_columns; j++) {
            if (mat->data[i][j] != data[i * n_columns + j]) return 0;
        }
    }

    return 1;
}

matrix *create_filled_matrix(size_t n_rows, size_t n_columns) {
    matrix *mat = create_matrix(n_rows, n_columns);
    if (mat->data != NULL) {
        for (size_t i = 0; i < mat->n_rows; i++) {
            for (size_t j = 0; j < mat->n_columns; j++) {
                mat->data[i][j] = (double) i * (double) n_columns + (double) j;
            }
        }
    }


    return mat;
}

int test_create_matrix_zero_rows() {
    matrix *mat = create_matrix(0, 3);

    int result = mat->n_columns == 0 && mat->n_rows == 0 && mat->data == NULL;

    free_matrix(mat);
    return result;
}

int test_create_matrix_zero_columns() {
    matrix *mat = create_matrix(3, 0);

    int result = mat->n_columns == 0 && mat->n_rows == 0 && mat->data == NULL;

    free_matrix(mat);
    return result;
}

int test_create_matrix_zero_rows_and_columns() {
    matrix *mat = create_matrix(0, 0);

    int result = mat->n_columns == 0 && mat->n_rows == 0 && mat->data == NULL;

    free_matrix(mat);
    return result;
}

int test_create_matrix_valid() {
    size_t n_rows = 4;
    size_t n_columns = 7;
    matrix *mat = create_matrix(n_rows, n_columns);

    int result = mat->n_columns == n_columns && mat->n_rows == n_rows;
    if (result) {
        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_columns; ++j) {
                if (mat->data[i][j] != 0.0) {
                    result = 0;
                    break;
                }
            }
        }
    }

    free_matrix(mat);
    return result;
}


int test_get_by_index_valid() {
    matrix *mat = create_filled_matrix(2, 3);

    double new_value = 123.123;
    size_t row = 1;
    size_t column = 2;

    double *ptr = get_by_index(mat, row, column);
    (*ptr) = new_value;

    int result = mat->data[row][column] == new_value;
    free_matrix(mat);

    return result;
}

int test_get_by_index_out_of_range() {
    matrix *mat = create_filled_matrix(2, 3);
    int result = (get_by_index(mat, 2, 0) == NULL) &&
                 (get_by_index(mat, 3, 0) == NULL) &&
                 (get_by_index(mat, 0, 3) == NULL) &&
                 (get_by_index(mat, 0, 4) == NULL) &&
                 (get_by_index(mat, 5, 5) == NULL);

    free_matrix(mat);

    return result;
}

int test_slice_empty_result() {

    size_t n_rows = 3;
    size_t n_columns = 4;

    matrix *mat = create_filled_matrix(3, 4);

    matrix *sliced_matrices[] = {
            slice(mat, n_rows, n_rows, 0, 1),
            slice(mat, n_rows, n_rows + 1, 0, 1),
            slice(mat, n_rows + 1, n_rows + 2, 0, 1),
            slice(mat, 0, 1, n_columns, n_columns),
            slice(mat, 0, 1, n_columns, n_columns + 1),
            slice(mat, 0, 1, n_columns + 1, n_columns + 2),
            slice(mat, n_rows, n_rows, n_columns, n_columns),
            slice(mat, n_rows, n_rows + 1, n_columns, n_columns + 1),
            slice(mat, n_rows + 1, n_rows + 2, n_columns + 1, n_columns + 2),
            slice(mat, 0, 0, 0, 1), // row_start = row_end
            slice(mat, 1, 0, 0, 1), // row_start > row_end
            slice(mat, 0, 1, 0, 0), // col_start = col_end
            slice(mat, 0, 1, 1, 0), // col_start > col_end
            slice(mat, 0, 0, 0, 0),
            slice(mat, 1, 0, 1, 0),
    };

    int result = 1;
    for (int i = 0; i < sizeof(sliced_matrices) / sizeof(matrix *); i++) {
        if (!is_matrix_equal(sliced_matrices[i], NULL, 0, 0)) {
            result = 0;
            break;
        }
        free_matrix(sliced_matrices[i]);
    }

    free_matrix(mat);
    return result;
}

int test_slice_valid() {
    const size_t n_rows = 3;
    const size_t n_columns = 3;
    matrix *mat = create_filled_matrix(n_rows, n_columns);


    matrix *sliced1 = slice(mat, 0, 1, 0, n_columns);
    double expected_data1[] = {
            0.0, 1.0, 2.0
    };

    matrix *sliced2 = slice(mat, 0, 2, 0, n_columns);
    double expected_data2[] = {
            0.0, 1.0, 2.0,
            3.0, 4.0, 5.0
    };

    matrix *sliced3 = slice(mat, 0, 1, 0, 1);
    double expected_data3[] = {
            0.0
    };

    matrix *sliced4 = slice(mat, 0, 2, 1, n_columns);
    double expected_data4[] = {
            1.0, 2.0,
            4.0, 5.0
    };

    matrix *sliced5 = slice(mat, 0, 2, 1, n_columns + 1);
    double expected_data5[] = {
            1.0, 2.0,
            4.0, 5.0
    };

    matrix *sliced6 = slice(mat, 1, n_rows, 0, 2);
    double expected_data6[] = {
            3.0, 4.0,
            6.0, 7.0
    };

    matrix *sliced7 = slice(mat, 1, n_rows + 1, 0, 2);
    double expected_data7[] = {
            3.0, 4.0,
            6.0, 7.0
    };

    matrix *sliced8 = slice(mat, 0, n_rows, 0, n_columns);
    double expected_data8[] = {
            0.0, 1.0, 2.0,
            3.0, 4.0, 5.0,
            6.0, 7.0, 8.0,
    };

    matrix *sliced9 = slice(mat, 1, n_rows + 1, 1, n_columns + 1);
    double expected_data9[] = {
            4.0, 5.0,
            7.0, 8.0
    };

    int result = \
                 is_matrix_equal(sliced1, expected_data1, 1, n_columns) &&
                 is_matrix_equal(sliced2, expected_data2, 2, n_columns) &&
                 is_matrix_equal(sliced3, expected_data3, 1, 1) &&
                 is_matrix_equal(sliced4, expected_data4, 2, 2) &&
                 is_matrix_equal(sliced5, expected_data5, 2, 2) &&
                 is_matrix_equal(sliced6, expected_data6, 2, 2) &&
                 is_matrix_equal(sliced7, expected_data7, 2, 2) &&
                 is_matrix_equal(sliced9, expected_data9, 2, 2) &&
                 is_matrix_equal(sliced8, expected_data8, n_rows, n_columns);

    free_matrix(sliced1);
    free_matrix(sliced2);
    free_matrix(sliced3);
    free_matrix(sliced4);
    free_matrix(sliced5);
    free_matrix(sliced6);
    free_matrix(sliced7);
    free_matrix(sliced8);
    free_matrix(sliced9);
    free_matrix(mat);
    return result;
}

int test_concatenate_vertically_invalid() {
    matrix *top1 = create_filled_matrix(2, 3);
    matrix *top2 = create_filled_matrix(3, 3);
    matrix *bottom1 = create_filled_matrix(2, 4);

    int result = concatenate_vertically(top1, bottom1) == NULL &&
                 concatenate_vertically(top2, bottom1) == NULL;

    free_matrix(top1);
    free_matrix(top2);
    free_matrix(bottom1);

    return result;
}

int test_concatenate_vertically_valid() {
    matrix *top = create_filled_matrix(1, 2);
    matrix *bottom = create_filled_matrix(2, 2);

    matrix *concatenated1 = concatenate_vertically(top, bottom);
    double expected_data1[] = {
            0.0, 1.0, // top
            0.0, 1.0, // bottom
            2.0, 3.0  // bottom
    };
    int result = is_matrix_equal(concatenated1, expected_data1, 3, 2);

    free_matrix(top);
    free_matrix(bottom);
    free_matrix(concatenated1);

    return result;
}

int test_concatenate_vertically_empty_matrices() {
    matrix *top = create_filled_matrix(0, 0);
    matrix *bottom = create_filled_matrix(0, 0);

    matrix *concatenated = concatenate_vertically(top, bottom);

    int result = is_matrix_equal(concatenated, NULL, 0, 0);

    free_matrix(top);
    free_matrix(bottom);
    free_matrix(concatenated);

    return result;
}

int test_concatenate_vertically_top_matrix_is_empty() {
    matrix *top = create_filled_matrix(0, 0);
    matrix *bottom = create_filled_matrix(1, 2);

    matrix *concatenated = concatenate_horizontally(top, bottom);
    double expected_data[] = { // data should equal bottom->data
            0.0, 1.0,
    };

    int result = is_matrix_equal(concatenated, expected_data, 1, 2);

    free_matrix(top);
    free_matrix(bottom);
    free_matrix(concatenated);

    return result;
}

int test_concatenate_vertically_bottom_matrix_is_empty() {
    matrix *top = create_filled_matrix(1, 2);
    matrix *bottom = create_filled_matrix(0, 0);

    matrix *concatenated = concatenate_horizontally(top, bottom);
    double expected_data[] = { // data should equal top->data
            0.0, 1.0,
    };

    int result = is_matrix_equal(concatenated, expected_data, 1, 2);

    free_matrix(top);
    free_matrix(bottom);
    free_matrix(concatenated);

    return result;
}


int test_concatenate_horizontally_invalid() {
    matrix *left1 = create_filled_matrix(3, 2);
    matrix *left2 = create_filled_matrix(3, 3);
    matrix *right = create_filled_matrix(4, 2);

    int result = concatenate_horizontally(left1, right) == NULL &&
                 concatenate_horizontally(left2, right) == NULL;

    free_matrix(left1);
    free_matrix(left2);
    free_matrix(right);

    return result;
}

int test_concatenate_horizontally_valid() {
    matrix *left = create_filled_matrix(2, 1);
    matrix *right = create_filled_matrix(2, 2);

    matrix *concatenated = concatenate_horizontally(left, right);
    double expected_data[] = {
            0.0, 0.0, 1.0,
            1.0, 2.0, 3.0
    };
    int result = is_matrix_equal(concatenated, expected_data, 2, 3);

    free_matrix(left);
    free_matrix(right);
    free_matrix(concatenated);

    return result;
}

int test_concatenate_horizontally_empty_matrices() {
    matrix *left = create_filled_matrix(0, 0);
    matrix *right = create_filled_matrix(0, 0);

    matrix *concatenated = concatenate_horizontally(left, right);

    int result = is_matrix_equal(concatenated, NULL, 0, 0);

    free_matrix(left);
    free_matrix(right);
    free_matrix(concatenated);

    return result;
}

int test_concatenate_horizontally_left_matrix_is_empty() {
    matrix *left = create_filled_matrix(0, 0);
    matrix *right = create_filled_matrix(1, 2);

    matrix *concatenated = concatenate_horizontally(left, right);
    double expected_data[] = { // data should equal right->data
            0.0, 1.0,
    };

    int result = is_matrix_equal(concatenated, expected_data, 1, 2);

    free_matrix(left);
    free_matrix(right);
    free_matrix(concatenated);

    return result;
}

int test_concatenate_horizontally_right_matrix_is_empty() {
    matrix *left = create_filled_matrix(1, 2);
    matrix *right = create_filled_matrix(0, 0);

    matrix *concatenated = concatenate_horizontally(left, right);
    double expected_data[] = { // data should equal left->data
            0.0, 1.0,
    };

    int result = is_matrix_equal(concatenated, expected_data, 1, 2);

    free_matrix(left);
    free_matrix(right);
    free_matrix(concatenated);

    return result;
}

int test_transpose_valid() {
    matrix *mat = create_filled_matrix(2, 4);

    /* before:
     * 0 1 2 3
     * 4 5 6 7
     *
     * after:
     * 0 4
     * 1 5
     * 2 6
     * 3 7
     */
    double expected_transposed[] = {
            0.0, 4.0,
            1.0, 5.0,
            2.0, 6.0,
            3.0, 7.0
    };
    matrix *transposed_matrix = transpose(mat);

    int result = is_matrix_equal(transposed_matrix, expected_transposed, 4, 2);

    free_matrix(mat);
    free_matrix(transposed_matrix);

    return result;
}

int test_transpose_empty_data() {
    matrix *mat = create_filled_matrix(0, 0);
    matrix *transposed_matrix = transpose(mat);

    int result = is_matrix_equal(transposed_matrix, NULL, 0, 0);

    free_matrix(mat);
    free_matrix(transposed_matrix);

    return result;
}

int test_ravel() {
    matrix *mat = create_filled_matrix(2, 3);
    double *raveled_matrix = ravel(mat);

    int result = 1;
    for (int i = 0; i < 6; i++) {
        if (raveled_matrix[i] != i) {
            result = 0;
            break;
        }
    }

    free_matrix(mat);
    free(raveled_matrix);

    return result;
}

int main() {
    TestInfo tests_info[] = {
            {
                    "test_create_matrix_zero_rows",
                    test_create_matrix_zero_rows
            },
            {
                    "test_create_matrix_zero_columns",
                    test_create_matrix_zero_columns
            },
            {
                    "test_create_matrix_zero_rows_and_columns",
                    test_create_matrix_zero_rows_and_columns
            },
            {
                    "test_create_matrix_valid",
                    test_create_matrix_valid
            },
            {
                    "test_get_by_index_valid",
                    test_get_by_index_valid
            },
            {
                    "test_get_by_index_out_of_range",
                    test_get_by_index_out_of_range
            },
            {
                    "test_slice_empty_result",
                    test_slice_empty_result
            },
            {
                    "test_slice_valid",
                    test_slice_valid
            },
            {
                    "test_concatenate_vertically_invalid",
                    test_concatenate_vertically_invalid
            },
            {
                    "test_concatenate_vertically_valid",
                    test_concatenate_vertically_valid
            },
            {
                    "test_concatenate_vertically_empty_matrices",
                    test_concatenate_vertically_empty_matrices
            },
            {
                    "test_concatenate_vertically_top_matrix_is_empty",
                    test_concatenate_vertically_top_matrix_is_empty
            },
            {
                    "test_concatenate_vertically_bottom_matrix_is_empty",
                    test_concatenate_vertically_bottom_matrix_is_empty
            },
            {
                    "test_concatenate_horizontally_invalid",
                    test_concatenate_horizontally_invalid
            },
            {
                    "test_concatenate_horizontally_valid",
                    test_concatenate_horizontally_valid
            },
            {
                    "test_concatenate_horizontally_empty_matrices",
                    test_concatenate_horizontally_empty_matrices
            },
            {
                    "test_concatenate_horizontally_left_matrix_is_empty",
                    test_concatenate_horizontally_left_matrix_is_empty
            },
            {
                    "test_concatenate_horizontally_right_matrix_is_empty",
                    test_concatenate_horizontally_right_matrix_is_empty
            },
            {
                    "test_transpose_valid",
                    test_transpose_valid
            },
            {
                    "test_transpose_empty_data",
                    test_transpose_empty_data
            },
            {
                    "test_ravel",
                    test_ravel
            },
    };

    TestSuite test_suite = {tests_info, sizeof(tests_info) / sizeof(TestInfo),
                            NULL, NULL};

    run_tests(test_suite);
}
