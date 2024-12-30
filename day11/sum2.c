#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 10000;
    int arr[n];
    if(rank == 0){
        for(int i = 0; i < n; i++) arr[i] = i + 1;
        for(int i = 1; i < size; i++){
            MPI_Send(arr, n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(arr, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
    }

    int localsum = 0;
    for(int i = start; i < end; i++){
        localsum += arr[i];
    }

    if(rank != 0){
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

    MPI_Finalize();
    return 0;
}
