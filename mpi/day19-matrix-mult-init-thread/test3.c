#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    long long data_size = 1000000000;
    int num_threads = 10;
    long long *data = NULL;
    long long chunksize;
    long long *local_data = NULL;
    long long local_sum = 0;
    long long global_sum = 0;

    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(provided != MPI_THREAD_FUNNELED){
        printf("MPI_THREAD_FUNNELED not supported\n");
        MPI_Finalize();
        return 0;
    }
    chunksize = data_size / size;

    if (rank == 0) {
        data = (long long *)malloc(data_size * sizeof(long long));
        for (long long i = 0; i < data_size; i++) {
            data[i] = i + 1;
        }
    }

    local_data = (long long *)malloc(chunksize * sizeof(long long ));

    MPI_Scatter(data, chunksize, MPI_LONG_LONG, local_data, chunksize, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    #pragma omp parallel for num_threads(num_threads) reduction(+:local_sum)
    for (long long i = 0; i < chunksize; i++) {
        local_sum += local_data[i];
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final sum: %lld\n", global_sum);
    }

    if (rank == 0) {
        free(data);
    }
    free(local_data);
    MPI_Finalize();

    return 0;
}
