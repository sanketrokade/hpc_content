#include <mpi.h>
#include <stdio.h>
#define N 10

int main() {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int arr[N];
    if (rank == 0) {
        for(int i = 0; i < N; i++) arr[i] = i + 1;
    }

    // Broadcast the data from the root process to all processes
    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i = 0; i < N; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
