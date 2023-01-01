#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1
#define ARRAY_MAX 20

typedef struct matrix{
    int **matrix;
    int rows;
    int cols;
}Matrix;

int is_eligible(int row, int col);
int parse_input(char *str, int *row, int *col);
int create_matrix(int ***matrix, Matrix *mat);
void request_input(Matrix mat, char curr);
int free_AB_matrices(Matrix A, Matrix B);
int verify_dim(Matrix A, Matrix B);
int free_result_matrix(Matrix result);
void print_matrix(Matrix mat);
int create_result_matrix(Matrix A, Matrix B, Matrix *mat_C, int ***mat_result);
void matrix_multplication(Matrix A, Matrix B);

int main(){

    Matrix mat_A, mat_B;
    int **matrix_A, **matrix_B;

    if(create_matrix(&matrix_A, &mat_A) == SUCCESS){
        if(create_matrix(&matrix_B, &mat_B) == FAILURE){
            return FAILURE;
        }
    }else{
        return FAILURE;
    }

    if(verify_dim(mat_A, mat_B) == FAILURE){
        printf("Matrix A and B cannot be multiplied!\n");
        return FAILURE;
    }

    request_input(mat_A, 'A');
    request_input(mat_B, 'B');

    printf("\nA * B =\n\n");

    matrix_multplication(mat_A, mat_B);
    
    printf("\n");

    return SUCCESS;
}

void request_input(Matrix mat, char curr){

    int i, j;

    printf("Enter elements of Matrix %c...\n", curr);
    
    for(i = 0; i < mat.rows; i++){
        for(j = 0; j < mat.cols; j++){
            printf("Element at (%d, %d) = ", i + 1, j + 1);
            scanf("%d", &mat.matrix[i][j]);
        }
    }
}

void matrix_multplication(Matrix A, Matrix B){

    Matrix mat_C;
    int **mat_result;
    int i, j, h, result = 0;

    create_result_matrix(A, B, &mat_C, &mat_result);

    for(i = 0; i < A.rows; i++){
        for(j = 0; j < B.cols; j++){
            for(h = 0; h < B.rows; h++){
                result += A.matrix[i][h] * B.matrix[h][j];
            }
            mat_C.matrix[i][j] = result;
            result = 0;
        }
    }

    print_matrix(mat_C);

    free_AB_matrices(A, B);
    free_result_matrix(mat_C);
}

int create_result_matrix(Matrix A, Matrix B, Matrix *mat_C, int ***mat_result){
    
    int i;
    mat_C->rows = A.rows;
    mat_C->cols = B.cols;
    *mat_result = calloc(A.rows, sizeof(int *));

    if(!(*mat_result)){
        return FAILURE;
    }

    for(i = 0; i < A.rows; i++){
        (*mat_result)[i] = calloc(B.cols, sizeof(int));
        if(!((*mat_result)[i])){
            return FAILURE;
        }
    }

    mat_C->matrix = *mat_result;

    return SUCCESS;
}

int create_matrix(int ***matrix, Matrix *mat){
    
    int i = 0;
    int rows, cols;
    char input[ARRAY_MAX] = "\0";

    printf("Enter the dimensions of the m x n matrix in the format \"rows x columns\"\n");

    fgets(input, ARRAY_MAX, stdin);

    parse_input(input, &rows, &cols);

    if(is_eligible(rows, cols) == 0){
        printf("Rows and columns of matrix must be greater than 0.\n");
        return FAILURE;
    }

    *matrix = calloc(rows, sizeof(int *));

    if(!(*matrix)){
        return FAILURE;
    }

    for(i; i < rows; i++){
        (*matrix)[i] = calloc(cols, sizeof(int));
        if(!((*matrix)[i])){
            return FAILURE;
        }
    }

    mat->matrix = *matrix;
    mat->rows = rows;
    mat->cols = cols;

    return SUCCESS;
}

void print_matrix(Matrix mat){

    int i, j;

    for(i = 0; i < mat.rows; i++){
        for(j = 0; j < mat.cols; j++){
           printf("%4d ", mat.matrix[i][j]);
        }
        printf("\n");
    }
}

int verify_dim(Matrix A, Matrix B){
    if(A.cols != B.rows){
        return FAILURE;
    }

    return SUCCESS;
}

int is_eligible(int row, int col){
    return ((row > 0 && col > 0));
}

int parse_input(char *str, int *row, int *col){

    if((sscanf(str, "%d x %d", row, col)) != 2){
        return FAILURE;
    }

    return SUCCESS;
}

int free_result_matrix(Matrix result){

    int i = 0;

    if(result.matrix){

        for(i; i < result.rows; i++){
            free(result.matrix[i]);
        }

        free(result.matrix);
        
        return SUCCESS;
    }

    return FAILURE;
}

int free_AB_matrices(Matrix A, Matrix B){

    int i = 0;

    if(A.matrix && B.matrix){

        for(i; i < A.rows; i++){
            free(A.matrix[i]);
            free(B.matrix[i]);
        }

        if(B.rows > A.rows){
            free(B.matrix[B.rows - 1]);
        }

        free(A.matrix);
        free(B.matrix);
    
        return SUCCESS;
    }

    return FAILURE;
}

/*
    1. request input
    2. parse input
    3. store input into appropriate sized matrix
    4. request input again
    5. parse input
    6. ensure matrix multiplication can be executed based on dimension
    7. store input in second matrix
    8. perform algo to multiply matrices and store new vals into appropraite dimension matrix
*/