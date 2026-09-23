#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 10000

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int chunksize = N / size;
    int* global_arr = NULL;
    int* local_arr = (int*)malloc(chunksize * sizeof(int));
    if (rank == 0) {
        global_arr = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            global_arr[i] = i + 1;
        }
    }
    MPI_Scatter(global_arr, chunksize, MPI_INT, local_arr, chunksize, MPI_INT, 0, MPI_COMM_WORLD);
    int local_sum = 0;
    for (int i = 0; i < chunksize; i++) {
        local_sum += local_arr[i];
    }
    int final_sum;
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&final_sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("rank = %d\ttotal sum = %d\n", rank, final_sum);
    if(rank == 0){
        free(global_arr);
    }
    free(local_arr);
    MPI_Finalize();
    return 0;
}
