#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define N 10000

int main(){

    int size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunksize = N / size;
    int* arr;
    int *arr2 = (int*)malloc(sizeof(int) * chunksize);

    if(rank == 0){
        arr = (int*)malloc(sizeof(int) * N);
        for(int i = 0; i < N; i++){
            arr[i] = i + 1;
        }
    }

    MPI_Scatter(arr, chunksize, MPI_INT, arr2, chunksize, MPI_INT, 0, MPI_COMM_WORLD);

    int localsum = 0;
    for(int i = 0; i < chunksize; i++){
        localsum += arr2[i];
    }

    if (rank != 0){
        MPI_Send(&localsum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int totalsum = 0;
        totalsum += localsum;
        for(int i = 1; i < size; i++){
            MPI_Recv(&localsum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalsum += localsum;
        }
        printf("totalsum = %d\n", totalsum);
    }

    free(arr2);
    if(rank == 0) free(arr);

    MPI_Finalize();
    return 0;
}
