#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 10;
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
        chunksize += n % size;
    }
    int* arr;
    if(rank == 0){
        arr = (int*)malloc(sizeof(int) * n);
        for(int i = 0; i < n; i++){
                arr[i] = i + 1;
        }
        for(int i = 1; i < size; i++){
            MPI_Send(&arr[i * chunksize], chunksize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        arr = (int*)malloc(sizeof(int) * chunksize);
        MPI_Recv(arr, chunksize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for(int i = 0; i < chunksize; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);

    MPI_Finalize();
    return 0;
}
