#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define N 20

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int count = 4;
    const int sl = 5;                 //sl = stridelength
    int blocklengths[4] = {2, 2, 2, 2};
    int displacements[4] = {0 * sl, 1 * sl, 2 * sl, 3 * sl};
    //int displacements[3] = {0, 5, 10};
    int data[N];
    MPI_Datatype indexed_type;

    // Create an indexed datatype
    MPI_Type_indexed(count, blocklengths, displacements, MPI_INT, &indexed_type);
    MPI_Type_commit(&indexed_type);

    if (rank == 0) {
        // Initialize the data array with some values
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;
        }

        MPI_Send(data, 1, indexed_type, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    } else if (rank == 1) {
        // Initialize the data array to zero
        for (int i = 0; i < N; i++) {
            data[i] = 0;
        }

        MPI_Recv(data, 1, indexed_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    MPI_Type_free(&indexed_type);
    MPI_Finalize();
    return 0;
}
