#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

enum ErrorCode
{
    ERROR_SUCCESS,
    ERROR_FAILURE
};

typedef struct Row
{
    int* start;
    int* end;
} Row;

enum ErrorCode input_array(int*** const array, size_t* const size_y, Row** const rows);
enum ErrorCode free_array (int*** const array, size_t* const size_y, Row** const rows);

enum ErrorCode print_array(const Row* const rows, const size_t size_y);

int main()
{
    int** array = NULL;
    size_t size_y = 0;
    Row* rows = NULL;

    if (input_array(&array, &size_y, &rows) == ERROR_FAILURE)
    {
        fprintf(stderr, "INPUT FAILURE\n");
        return -1;
    }

    if (print_array((const Row* const)rows, (const size_t)size_y) == ERROR_FAILURE)
    {
        fprintf(stderr, "PRINT FAILURE\n");
        return -1;
    }

    if (free_array(&array, &size_y, &rows) == ERROR_FAILURE)
    {
        fprintf(stderr, "FREE FAILURE\n");
        return -1;
    }
    
    return 0;
}

enum ErrorCode input_array(int*** const array, size_t* const size_y, Row** const rows)
{
    assert(array);
    assert(size_y);
    assert(rows);


    printf("Input count rows in array: ");
    if (scanf("%zu", size_y) != 1 || size_y == 0)
        return ERROR_FAILURE;
    
    *array = (int**)calloc(*size_y, sizeof(**array));
    *rows = (Row*)calloc(*size_y + 1, sizeof(**rows));

    size_t size_x = 0;
    for (size_t y = 0; y < *size_y; ++y)
    {
        if (printf("Input count elements in %zu row : ", y+1) <= 0)
            return ERROR_FAILURE;

        size_x = 0;
        if (scanf("%zu", &size_x) != 1)
            return ERROR_FAILURE;

        (*array)[y] = (int*)calloc(size_x, sizeof(***array));
        (*rows)[y].start = &(*array)[y][0];
        (*rows)[y].end = &(*array)[y][0] + size_x;

        for (size_t x = 0; x < size_x; ++x)
        {
            if (printf("Input %zu element in %zu row : ", x+1, y+1) <= 0)
                return ERROR_FAILURE;
            if (scanf("%d", &(*array)[y][x]) != 1)
                return ERROR_FAILURE;
        }
    }
    return ERROR_SUCCESS;
}

enum ErrorCode free_array (int*** const array, size_t* const size_y, Row** const rows)
{
    assert(array);
    assert(size_y);
    assert(rows);

    for (size_t y = 0; y < *size_y; ++y)
    {
        free((*array)[y]); (*array)[y] = NULL;
    }
    free(*array); *array = NULL;
    free(*rows); *rows = NULL;

    return ERROR_SUCCESS;
}

enum ErrorCode print_array(const Row* const rows, const size_t size_y)
{
    for(size_t y = 0; y < size_y; ++y)
    {
        for (const int* x_ptr = rows[y].start; x_ptr < rows[y].end; ++x_ptr)
        {
            char output[20] = {};
            if (snprintf(output, 20, "data[%zu][%zu] = %d", y+1, (size_t)(x_ptr - rows[y].start),
                     *x_ptr) <= 0)
                return ERROR_FAILURE;

            if (printf("%-20s", output) <= 0)
                return ERROR_FAILURE;
        }
        printf("\n");
    }
    printf("\n");

    return ERROR_SUCCESS;
}
