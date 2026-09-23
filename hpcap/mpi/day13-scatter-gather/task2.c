#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    MPI_Init(NULL,NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 2;
    int *array = (int*)malloc(N * sizeof(int));
    int *totarr = NULL;
    for (int i = 0; i < N; i++) {
        array[i] = 1;
    }
    if (rank == 0) {

        totarr = (int*)malloc(N * size * sizeof(int));
    }
    if (rank != 0) {
        MPI_Send(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {

        for (int i = 0; i < N; i++) {
            totarr[i] = array[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(&totarr[i * N], N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = 0; i < N * size; i++) {
            printf("%d\n", totarr[i]);
        }

        free(totarr);
    }
    free(array);
    MPI_Finalize();
    return 0;
}
