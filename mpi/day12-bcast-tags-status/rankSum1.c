#include<stdio.h>
#include<mpi.h>
int main(){
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //rank sum = 0 + 1 + 2 + 3 + 4
    int rankSum;
    if(rank != 0){
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&rankSum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d received rank sum = %d\n", rank, rankSum);
    }
    else{
        rankSum = rank;
        for(int i = 1; i < size; i++){
            MPI_Recv(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            rankSum+= rank;
        }
        printf("Sum of all the ranks = %d\n", rankSum);
        for(int i = 1; i < size; i++){
            MPI_Send(&rankSum, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
