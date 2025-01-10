#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Process %d: Preparing data...\n", rank);
        sleep(2); // Simulate data preparation
    }

    printf("Process %d: Waiting at the barrier.\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);

    printf("Process %d: Proceeding to next phase.\n", rank);

    MPI_Finalize();
    return 0;
}
