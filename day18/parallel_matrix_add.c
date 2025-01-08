#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char **argv){
    MPI_Init(NULL, NULL);
    int n = 400;
    int **m1, **m2, **sumMat;
    m1 = (int**)malloc(sizeof(int*) * n);
    m2 = (int**)malloc(sizeof(int*) * n);
    sumMat = (int**)malloc(sizeof(int*) * n);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        for(int i = 0; i < n; i++){
            sumMat[i] = (int*)malloc(sizeof(int) * n);
            m1[i] = (int*)malloc(sizeof(int) * n);
            m2[i] = (int*)malloc(sizeof(int) * n);
            for(int j = 0; j < n; j++){
                m1[i][j] = 1;
                m2[i][j] = 1;
                sumMat[i][j] = 0;
            }
        }
    }
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1) end = n;

    int **localArr1 = (int**) malloc(sizeof(int*) * chunksize);
    int **localArr2 = (int**) malloc(sizeof(int*) * chunksize);
    int **sumArr= (int**) malloc(sizeof(int*) * chunksize);
    for(int i = 0; i < chunksize; i++){
        localArr1[i] = (int*) malloc(sizeof(int) * n);
        localArr2[i] = (int*) malloc(sizeof(int) * n);
        sumArr[i] = (int*) malloc(sizeof(int) * n);
    }
    for(int i = 0; i < chunksize; i++){
        MPI_Scatter(m1[i], chunksize, MPI_INT, localArr1[i], chunksize, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(m2[i], chunksize, MPI_INT, localArr2[i], chunksize, MPI_INT, 0, MPI_COMM_WORLD);
    }

    for(int i = 0; i < chunksize; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", localArr1[i][j]);
            printf("%d ", localArr2[i][j]);
        }
    }

    /*
    for(int i = start; i < end; i++){
        for(int j = 0; j < n; j++){
            sumArr[i][j] = localArr1[i][j] + localArr2[i][j];
        }
    }
    MPI_Gather(sumArr, chunksize * n, MPI_INT, sumMat, chunksize * n, MPI_INT, 0, MPI_COMM_WORLD);


        for(int i = start; i < end; i++){
            for(int j = 0; j < n; j++){
                printf("%d ",sumMat[i][j]);
            }
            printf("\n");
    }

    */

    MPI_Finalize();
    return 0;
}
