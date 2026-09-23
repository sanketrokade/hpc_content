#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2;
    if(rank != 0){
        data1 = rank;
        data2 = 234;
        MPI_Request request;
        MPI_Isend(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    }
    if(rank == 0){
        MPI_Recv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("data1 %d\n", data1);
    }

    MPI_Finalize();
    return 0;
}
