#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Split the world group into two groups
    int color = rank % 2;  // Determine color based on rank
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);
    // Get the new rank and size in the new communicator
    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);
    printf("World Rank: %d, New Rank: %d, New Size: %d\n", rank, new_rank, new_size);
    // Perform some communication within the new communicator
    int send_data = new_rank;
    int recv_data;
    MPI_Allreduce(&send_data, &recv_data, 1, MPI_INT, MPI_SUM, new_comm);
    printf("World Rank: %d, New Comm Sum: %d\n", rank, recv_data);
    // Free the new communicator and group
    MPI_Comm_free(&new_comm);
    MPI_Finalize();
    return 0;
}
