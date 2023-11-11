#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define M 1000
#define N 500
#define P 800
#define NUM_THREADS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Structure for passing data to threads
struct thread_data {
    int start_row;
    int end_row;
    int** A;
    int** B;
    int** C;
};

// Function to perform matrix multiplication for a given range of rows
void *multiply(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < P; j++) {
            int result = 0;
            for (int k = 0; k < N; k++) {
                result += data->A[i][k] * data->B[k][j];
            }

            // Lock the mutex before updating the result matrix
            pthread_mutex_lock(&mutex);
            data->C[i][j] = result;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

int main() {
    int **A, **B, **C;

    // Allocate memory for matrices A, B, and C
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

    // Initialize matrices A and B
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

    // Create pthread variables
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];


struct timeval start_time, end_time;

    // Measure start time
    //clock_t start_time = clock();

    // Divide the work among threads
gettimeofday(&start_time, NULL);
    int rows_per_thread = M / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data_array[i].start_row = i * rows_per_thread;
        thread_data_array[i].end_row = (i == NUM_THREADS - 1) ? M : (i + 1) * rows_per_thread;
        thread_data_array[i].A = A;
        thread_data_array[i].B = B;
        thread_data_array[i].C = C;

        pthread_create(&threads[i], NULL, multiply, (void *)&thread_data_array[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Measure end time
    //clock_t end_time = clock();
    gettimeofday(&end_time, NULL);
    // Calculate execution time
    //double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1e6;
    // Display the execution time
    printf("Execution Time: %f seconds\n", elapsed_time);

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


