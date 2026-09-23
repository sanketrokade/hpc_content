#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10000; // Size of the array
    int *arr = NULL;
    int localsum = 0;

    if (rank == 0) {
        // Allocate and initialize the array
        arr = (int*)malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) {
            arr[i] = i + 1;
        }

        // Distribute chunks of the array to other processes
        int chunksize = n / size;
        for (int i = 1; i < size; i++) {
            int start = i * chunksize;
            int end = (i == size - 1) ? n : start + chunksize;
            int send_size = end - start;

            MPI_Send(&arr[start], send_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Calculate the local sum for rank 0's chunk
        for (int i = 0; i < chunksize; i++) {
            localsum += arr[i];
        }

        // Receive local sums from other processes and compute total sum
        int totalsum = localsum;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&localsum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalsum += localsum;
        }

        // Print the total sum
        printf("Total sum = %d\n", totalsum);

        // Free the array
        free(arr);
    } else {
        // Calculate chunksize and allocate a buffer for received data
        int chunksize = n / size;
        int start = rank * chunksize;
        int end = (rank == size - 1) ? n : start + chunksize;
        int recv_size = end - start;

        int *recv_buf = (int*)malloc(sizeof(int) * recv_size);

        // Receive the chunk from rank 0
        MPI_Recv(recv_buf, recv_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate the local sum
        for (int i = 0; i < recv_size; i++) {
            localsum += recv_buf[i];
        }

        // Send the local sum back to rank 0
        MPI_Send(&localsum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Free the buffer
        free(recv_buf);
    }

    MPI_Finalize();
    return 0;
}
