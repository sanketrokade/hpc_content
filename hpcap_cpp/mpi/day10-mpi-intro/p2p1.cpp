#include<stdio.h>
#include<mpi.h>
#define N 1000
int main(){
    int size, rank;
    int a[N];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        for(int i = 0; i < N; i++){
                a[i] = i + 1;
        }


        for(int i = 1; i < size; i++){
            MPI_Send(a, N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = N - 10; i < N; i++){
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
