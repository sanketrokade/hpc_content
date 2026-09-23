#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

int main() {
    int n = 2000;
    int i, j, k;

    // Allocate memory for matrices
    int **A = (int **)malloc(n * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    int **C = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
        B[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(n * sizeof(int));
    }

    // Initialize matrices
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    double starttime = omp_get_wtime();
    // Matrix multiplication
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double endtime = omp_get_wtime();

    // Print result
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    printf("execution time: %lf\n", endtime - starttime);

    // Free allocated memory
    for (i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
