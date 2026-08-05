#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THRESHOLD 64 // Cross-over threshold for Hybrid Strassen

// Function to dynamically allocate 2D matrix
double** allocate_matrix(int n) {
    double** mat = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        mat[i] = (double*)calloc(n, sizeof(double));
    }
    return mat;
}

// Function to free allocated matrix memory
void free_matrix(double** mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

// Standard Iterative Matrix Multiplication: O(N^3)
void traditional_multiply(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Matrix Addition and Subtraction Helpers
void add_matrix(double** A, double** B, double** Result, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Result[i][j] = A[i][j] + B[i][j];
}

void subtract_matrix(double** A, double** B, double** Result, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Result[i][j] = A[i][j] - B[i][j];
}

// Strassen's Matrix Multiplication Implementation
void strassen_multiply(double** A, double** B, double** C, int n) {
    if (n <= THRESHOLD) {
        traditional_multiply(A, B, C, n);
        return;
    }

    int new_size = n / 2;

    // Allocate sub-matrices
    double** A11 = allocate_matrix(new_size);
    double** A12 = allocate_matrix(new_size);
    double** A21 = allocate_matrix(new_size);
    double** A22 = allocate_matrix(new_size);

    double** B11 = allocate_matrix(new_size);
    double** B12 = allocate_matrix(new_size);
    double** B21 = allocate_matrix(new_size);
    double** B22 = allocate_matrix(new_size);

    double** A_temp = allocate_matrix(new_size);
    double** B_temp = allocate_matrix(new_size);

    double** M1 = allocate_matrix(new_size);
    double** M2 = allocate_matrix(new_size);
    double** M3 = allocate_matrix(new_size);
    double** M4 = allocate_matrix(new_size);
    double** M5 = allocate_matrix(new_size);
    double** M6 = allocate_matrix(new_size);
    double** M7 = allocate_matrix(new_size);

    // Partition input matrices into 4 sub-blocks
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + new_size];
            A21[i][j] = A[i + new_size][j];
            A22[i][j] = A[i + new_size][j + new_size];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + new_size];
            B21[i][j] = B[i + new_size][j];
            B22[i][j] = B[i + new_size][j + new_size];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    add_matrix(A11, A22, A_temp, new_size);
    add_matrix(B11, B22, B_temp, new_size);
    strassen_multiply(A_temp, B_temp, M1, new_size);

    // M2 = (A21 + A22) * B11
    add_matrix(A21, A22, A_temp, new_size);
    strassen_multiply(A_temp, B11, M2, new_size);

    // M3 = A11 * (B12 - B22)
    subtract_matrix(B12, B22, B_temp, new_size);
    strassen_multiply(A11, B_temp, M3, new_size);

    // M4 = A22 * (B21 - B11)
    subtract_matrix(B21, B11, B_temp, new_size);
    strassen_multiply(A22, B_temp, M4, new_size);

    // M5 = (A11 + A12) * B22
    add_matrix(A11, A12, A_temp, new_size);
    strassen_multiply(A_temp, B22, M5, new_size);

    // M6 = (A21 - A11) * (B11 + B12)
    subtract_matrix(A21, A11, A_temp, new_size);
    add_matrix(B11, B12, B_temp, new_size);
    strassen_multiply(A_temp, B_temp, M6, new_size);

    // M7 = (A12 - A22) * (B21 + B22)
    subtract_matrix(A12, A22, A_temp, new_size);
    add_matrix(B21, B22, B_temp, new_size);
    strassen_multiply(A_temp, B_temp, M7, new_size);

    // Compute C11, C12, C21, C22
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            // C11 = M1 + M4 - M5 + M7
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            // C12 = M3 + M5
            C[i][j + new_size] = M3[i][j] + M5[i][j];
            // C21 = M2 + M4
            C[i + new_size][j] = M2[i][j] + M4[i][j];
            // C22 = M1 - M2 + M3 + M6
            C[i + new_size][j + new_size] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    // Free intermediate sub-matrices
    free_matrix(A11, new_size); free_matrix(A12, new_size);
    free_matrix(A21, new_size); free_matrix(A22, new_size);
    free_matrix(B11, new_size); free_matrix(B12, new_size);
    free_matrix(B21, new_size); free_matrix(B22, new_size);
    free_matrix(A_temp, new_size); free_matrix(B_temp, new_size);
    free_matrix(M1, new_size); free_matrix(M2, new_size);
    free_matrix(M3, new_size); free_matrix(M4, new_size);
    free_matrix(M5, new_size); free_matrix(M6, new_size);
    free_matrix(M7, new_size);
}

int main() {
    int sizes[] = {256, 512, 1024, 2048};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("%-10s | %-20s | %-20s | %-12s\n", "Size (N)", "Traditional (s)", "Strassen (s)", "Speedup");
    printf("----------------------------------------------------------------------\n");

    for (int k = 0; k < num_sizes; k++) {
        int N = sizes[k];
        double** A = allocate_matrix(N);
        double** B = allocate_matrix(N);
        double** C_trad = allocate_matrix(N);
        double** C_stra = allocate_matrix(N);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = (double)(rand() % 10);
                B[i][j] = (double)(rand() % 10);
            }
        }

        clock_t start = clock();
        traditional_multiply(A, B, C_trad, N);
        clock_t end = clock();
        double time_trad = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock();
        strassen_multiply(A, B, C_stra, N);
        end = clock();
        double time_stra = ((double)(end - start)) / CLOCKS_PER_SEC;

        double speedup = time_trad / time_stra;
        printf("%-10d | %-20.4f | %-20.4f | %-12.2fx\n", N, time_trad, time_stra, speedup);

        free_matrix(A, N); free_matrix(B, N);
        free_matrix(C_trad, N); free_matrix(C_stra, N);
    }
    return 0;
}
