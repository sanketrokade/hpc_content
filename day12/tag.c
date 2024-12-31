#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2;
    if(rank == 0){
        data1 = 100;
        data2 = 200;
        MPI_Send(&data1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&data2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if(rank == 1){
        MPI_Recv(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d received data1 %d data2 %d\n", rank, data1, data2);
    }

    MPI_Finalize();
    return 0;
}
