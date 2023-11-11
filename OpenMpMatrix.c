#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define M 1000
#define N 500
#define P 800   

void matrix_multiply_parallel(int **A, int **B, int **C) {
    //omp_set_num_threads(8);
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int **A, **B, **C;

    // Allocate memory for matrices A, B, and C
    double strt;
    A = (int **)malloc(M * sizeof(int *));
    B = (int **)malloc(N * sizeof(int *));
    C = (int **)malloc(M * sizeof(int *));
    for (int i = 0; i < M; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(P * sizeof(int));
    }
    for (int i = 0; i < N; i++) {
        B[i] = (int *)malloc(P * sizeof(int));
    }

    // Initialize matrices A and B (you can modify this part as needed)
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            B[i][j] = i - j;
        }
    }

    // Multiply matrices A and B in parallel using OpenMP

    strt = omp_get_wtime();
    matrix_multiply_parallel(A, B, C);
    double cpu_time_used =omp_get_wtime()- strt;
    printf("Execution Time: %f seconds\n",cpu_time_used);

    // Free allocated memory
    for (int i = 0; i < M; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (int i = 0; i < N; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
