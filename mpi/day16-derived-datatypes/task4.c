#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define N 8

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const int count = N;       // Number of blocks
    const int blocklength = 1; // Number of elements in each block
    const int stride = N;      // Number of elements between the start of each block
    MPI_Datatype vector_type;
    MPI_Type_vector(count, blocklength, stride, MPI_INT, &vector_type);
    MPI_Type_commit(&vector_type);
    int data[N][N];              // Array to send/receive
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++){
                data[i][j] = i + 1;
            }
        }
        MPI_Send(&data[0][2], 1, vector_type, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data: \n");
        for (int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++){
                printf("%d ", data[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    } else if (rank == 1) {
        for (int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++){
                data[i][j] = 0;
            }
        }
        MPI_Recv(&data[0][2], 1, vector_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: \n");
        for (int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++){
                printf("%d ", data[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    MPI_Type_free(&vector_type);
    MPI_Finalize();
    return 0;
}
