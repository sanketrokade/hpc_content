# Day 13 — MPI: Scatter & Gather

[← Day 12 — MPI: Broadcast, Tags, Status](../../mpi/day12-bcast-tags-status/) · [Index](../../README.md) · [Day 14 — MPI: Reduce & Allreduce](../../mpi/day14-reduce-allreduce/)

## task1

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 10;
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
        chunksize += n % size;
    }
    int* arr;
    if(rank == 0){
        arr = (int*)malloc(sizeof(int) * n);
        for(int i = 0; i < n; i++){
                arr[i] = i + 1;
        }
        for(int i = 1; i < size; i++){
            MPI_Send(&arr[i * chunksize], chunksize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        arr = (int*)malloc(sizeof(int) * chunksize);
        MPI_Recv(arr, chunksize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for(int i = 0; i < chunksize; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task1.cpp
```

```bash
../../scripts/run.sh ./task1.out 4
```

**Output:**

```text

1 2
3 4
5 6
7 8 0 0

```

## task1 with scatter

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 10;
    int chunksize = n / size;
    int* arr;
    int *arr2 = (int*)malloc(sizeof(int) * chunksize);
    if(rank == 0){
        arr = (int*)malloc(sizeof(int) * n);
        for(int i = 0; i < n; i++){
                arr[i] = i + 1;
        }
    }
    MPI_Scatter(arr, chunksize, MPI_INT, arr2, chunksize, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i = 0; i < chunksize; i++){
        printf("%d ", arr2[i]);
    }
    printf("\n");

    free(arr2);
    if(rank == 0) free(arr);

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task1_scatter.cpp
```

```bash
../../scripts/run.sh ./task1_scatter.out 4
```

**Output:**

```text

3 4
5 6
7 8
1 2

```

## MPI_Scatter

```cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10; // Size of the array
    int *array = NULL;
    int chunk_size = n / size;
    int *sub_array = (int*)malloc(chunk_size * sizeof(int));

    if (rank == 0) {
        array = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            array[i] = i + 1; // Initialize the array with values 1 to n
        }
    }

    // Scatter the chunks of the array to all processes
    MPI_Scatter(array, chunk_size, MPI_LONG_LONG_INT, sub_array, chunk_size, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Compute the local sum
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        local_sum += (long long)sub_array[i];
    }

    // Gather all local sums to the root process
    long long final_sum = 0;
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The total sum of array elements is %lld\n", final_sum);
        free(array);
    }

    free(sub_array);

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh scatter.cpp
```

```bash
../../scripts/run.sh ./scatter.out 10
```

## Scatter sum

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define N 10000

int main(){

    int size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunksize = N / size;
    int* arr;
    int *arr2 = (int*)malloc(sizeof(int) * chunksize);

    if(rank == 0){
        arr = (int*)malloc(sizeof(int) * N);
        for(int i = 0; i < N; i++){
            arr[i] = i + 1;
        }
    }

    MPI_Scatter(arr, chunksize, MPI_INT, arr2, chunksize, MPI_INT, 0, MPI_COMM_WORLD);

    int localsum = 0;
    for(int i = 0; i < chunksize; i++){
        localsum += arr2[i];
    }

    if (rank != 0){
        MPI_Send(&localsum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int totalsum = 0;
        totalsum += localsum;
        for(int i = 1; i < size; i++){
            MPI_Recv(&localsum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalsum += localsum;
        }
        printf("totalsum = %d\n", totalsum);
    }

    free(arr2);
    if(rank == 0) free(arr);

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh scatter_sum.cpp
```

```bash
../../scripts/run.sh ./scatter_sum.out 10
```

**Output:**

```text

totalsum = 50005000

```

## task2

```cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    MPI_Init(NULL,NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 2;
    int *array = (int*)malloc(N * sizeof(int));
    int *totarr = NULL;
    for (int i = 0; i < N; i++) {
        array[i] = 1;
    }
    if (rank == 0) {

        totarr = (int*)malloc(N * size * sizeof(int));
    }
    if (rank != 0) {
        MPI_Send(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {

        for (int i = 0; i < N; i++) {
            totarr[i] = array[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(&totarr[i * N], N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = 0; i < N * size; i++) {
            printf("%d\n", totarr[i]);
        }

        free(totarr);
    }
    free(array);
    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task2.cpp
```

```bash
../../scripts/run.sh ./task2.out 5
```

**Output:**

```text

1
2
2
3
3
4
4
5
5
6

```

## MPI_Gather Example

### mpi_gather_example.cpp

```cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int send_data = rank; // Each process sends its rank
    int *recv_data = NULL;
    if (rank == 0) {
        recv_data = (int*)malloc(size * sizeof(int)); // Allocate memory for receiving data
    }
    // Gather the data from all processes to the root process
    MPI_Gather(&send_data, 1, MPI_INT, recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered data at root process: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", recv_data[i]);
        }
        printf("\n");
        free(recv_data);
    }
    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_gather.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_gather.out 10
  ```

  **Output:**

```text

  Gathered data at root process: 0 1 2 3 4 5 6 7 8 9

  ```

In this example, each process sends its rank as `send_data`. The `MPI_Gather` function is called to gather the values of `send_data` from all processes to the `recv_data` array in the root process. After gathering the data, the root process prints the gathered values.

### Summary

- ****`MPI_Gather`****: Gathers data from all processes in the communicator and collects it at the root process.

### mpi_array_sum_scatter.cpp

```cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 100; // Size of the array
    int *array = NULL;
    int chunk_size = n / size;
    int *sub_array = (int*)malloc(chunk_size * sizeof(int));

    if (rank == 0) {
        array = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            array[i] = i + 1; // Initialize the array with values 1 to n
        }
    }

    // Scatter the chunks of the array to all processes
    MPI_Scatter(array, chunk_size, MPI_INT, sub_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute the local sum
    int local_sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        local_sum += sub_array[i];
    }

    // Gather all local sums to the root process
    int final_sum = 0;
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The total sum of array elements is %d\n", final_sum);
        free(array);
    }

    free(sub_array);

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_array_sum_scatter.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_array_sum_scatter.out 10
  ```

  **Output:**

```text

  The total sum of array elements is 5050

  ```

## atharv

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunksize = N / size;
    int* global_arr = NULL;
    int* local_arr = (int*)malloc(chunksize * sizeof(int));

    if (rank == 0) {
        global_arr = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            global_arr[i] = i + 1;
        }
    }

    MPI_Scatter(global_arr, chunksize, MPI_INT, local_arr, chunksize, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < chunksize; i++) {
        local_sum += local_arr[i];
    }

    int* global_sums = NULL;
    if (rank == 0) {
        global_sums = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(&local_sum, 1, MPI_INT, global_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int total_sum = 0;
        printf ("Array of local sums: \n");
        for (int i = 0; i < size; i++) {
            printf("%d ", global_sums[i]);
            total_sum += global_sums[i];
        }
        printf("\nTotal sum = %d\n", total_sum);
        free(global_arr);
        free(global_sums);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task3.cpp
```

```bash
../../scripts/run.sh ./task3.out 10
```

**Output:**

```text

Array of local sums:
55 155 255 355 455 555 655 755 855 955
Total sum = 5050

```

---
[← Day 12 — MPI: Broadcast, Tags, Status](../../mpi/day12-bcast-tags-status/) · [Index](../../README.md) · [Day 14 — MPI: Reduce & Allreduce](../../mpi/day14-reduce-allreduce/)
