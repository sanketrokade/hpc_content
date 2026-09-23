#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2, data3, data4, data5;
    if(rank == 0){
        data1 = 100;
        data2 = 200;
        data3 = 300;
        data4 = 400;
        data5 = 500;
        MPI_Request request;
        MPI_Isend(&data1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data3, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data4, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data5, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    }
    else if(rank == 1){
        MPI_Recv(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data4, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data5, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d\n", rank);
        printf("data1 %d\n", data1);
        printf("data2 %d\n", data2);
        printf("data3 %d\n", data3);
        printf("data4 %d\n", data4);
        printf("data5 %d\n", data5);
    }

    MPI_Finalize();
    return 0;
}
