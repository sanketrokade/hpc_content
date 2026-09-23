#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define N 100000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *buffer;
    buffer = (int*) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        buffer[i] = (rank * i) + i;
    }
    if(rank == 0){
        MPI_Send(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
    else if(rank == 1){
        int *tempBuffer = (int*) malloc(sizeof(int) * N);
        MPI_Recv(tempBuffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(buffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for(int i = 0; i < N; i++) buffer[i] = tempBuffer[i];
        free(tempBuffer);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
