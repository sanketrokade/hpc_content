#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 1000000000;
    double startTime, endTime;
    long long chunksize = N / size;
    long long* global_arr = NULL;
    long long* local_arr = (long long*)malloc(chunksize * sizeof(long long));

    if (rank == 0) {
        global_arr = (long long*)malloc(N * sizeof(long long));
        for (int i = 0; i < N; i++) {
            global_arr[i] = i + 1;
        }
    }

    if(rank == 0) startTime = MPI_Wtime();
    MPI_Scatter(global_arr, chunksize, MPI_LONG_LONG_INT, local_arr, chunksize, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    long long local_sum = 0;
    for (int i = 0; i < chunksize; i++) {
        local_sum += local_arr[i];
    }

    long long* global_sums = NULL;
    if (rank == 0) {
        global_sums = (long long*)malloc(size * sizeof(long long));
    }

    MPI_Gather(&local_sum, 1, MPI_LONG_LONG_INT, global_sums, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        long long total_sum = 0;

        printf ("Array of local sums: \n");
        for (int i = 0; i < size; i++) {
            //printf("%d ", global_sums[i]);
            total_sum += global_sums[i];
        }
        endTime = MPI_Wtime();
        printf("rank %d : timing %lf\n", rank, endTime - startTime);
        printf("\nTotal sum = %lld\n", total_sum);
        free(global_arr);
        free(global_sums);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}
