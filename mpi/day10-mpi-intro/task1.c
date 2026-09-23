#include<stdio.h>
#include<mpi.h>
#define N 1000
int main(){
    int size, rank;
    int a[N];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunksize = N / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1) end = N;
    for(int i = start; i < end; i++){
        a[i] = i + 1;
    }

    for(int i = start; i < end; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    MPI_Finalize();
}
