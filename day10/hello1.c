#include<stdio.h>
#include<mpi.h>
int main(){
    MPI_Init(NULL, NULL);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello from process %d of %d\n", rank, size);
    MPI_Finalize();
    return 0;
}
