#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

void perform_computation(int rank) {
    printf("Process %d performing computation\n", rank);
    // Simulate computation by sleeping for a while
    sleep(2);
}

void perform_io_operations(int rank) {
    printf("Process %d performing I/O operations\n", rank);
    // Simulate I/O by sleeping for a while
    sleep(3);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    // Split the world group into two groups based on rank
    int color = rank % 2;  // Determine color based on rank
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

    // Get the new rank and size in the new communicator
    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);

    if (color == 0) {
        perform_computation(rank);
    } else {
        perform_io_operations(rank);
    }

    // Free the new communicator and group
    MPI_Comm_free(&new_comm);
    MPI_Group_free(&world_group);

    MPI_Finalize();
    return 0;
}
