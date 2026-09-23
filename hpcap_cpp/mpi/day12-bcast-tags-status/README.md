# Day 12 — MPI: Broadcast, Tags, Status

[← Day 11 — MPI: Point-to-Point (Blocking/Non-blocking)](../../mpi/day11-point-to-point/) · [Index](../../README.md) · [Day 13 — MPI: Scatter & Gather](../../mpi/day13-scatter-gather/)

## sum4.cpp

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 1000;
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
        chunksize += n % size;
    }
    int arr[chunksize];

    int index = 0;
    for(int i = start; i < end; i++){
        arr[index] = i + 1;
        index++;
    }

    index = 0;
    int localsum = 0;
    for(int i = start; i < end; i++){
        localsum += arr[index];
        index++;
    }

    if(rank != 0){
        MPI_Send(&localsum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int totalsum = 0;
        totalsum += localsum;
        for(int i = 1; i < size; i++){
            MPI_Recv(&localsum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalsum += localsum;
        }
        printf("Total sum = %d\n", totalsum);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh sum4.cpp
```

```bash
../../scripts/run.sh ./sum4.out 7
```

**Output:**

```text

Total sum = 500500

```

## calculate sum of rank of all the process

```cpp
#include<stdio.h>
#include<mpi.h>
int main(){
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //rank sum = 0 + 1 + 2 + 3 + 4
    if(rank != 0){
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int rankSum = rank;
        for(int i = 1; i < size; i++){
            MPI_Recv(&rank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            rankSum+= rank;
        }
        printf("Sum of all the ranks = %d\n", rankSum);
    }
    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh rankSum.cpp
```

```bash
../../scripts/run.sh ./rankSum.out 10
```

**Output:**

```text

Sum of all the ranks = 45

```

## send this ranksum to all the process

```cpp
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
```

```bash
../../scripts/compile.sh rankSum1.cpp
```

```bash
../../scripts/run.sh ./rankSum1.out 10
```

**Output:**

```text

Sum of all the ranks = 45
process 1 received rank sum = 45
process 8 received rank sum = 45
process 6 received rank sum = 45
process 2 received rank sum = 45
process 3 received rank sum = 45
process 5 received rank sum = 45
process 7 received rank sum = 45
process 4 received rank sum = 45
process 9 received rank sum = 45

```

## MPI Broadcast

### MPI_Bcast Example

#### mpi_bcast_example.cpp

```cpp
#include <mpi.h>
#include <stdio.h>

int main() {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data;
    if (rank == 0) {
        data = 100;  // Root process initializes the data
    }

    // Broadcast the data from the root process to all processes
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received data %d\n", rank, data);

    MPI_Finalize();
    return 0;
}
```

#### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_bcast.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_bcast.out 5
  ```

  **Output:**

```text

  Process 0 received data 100
  Process 2 received data 100
  Process 4 received data 100
  Process 1 received data 100
  Process 3 received data 100

  ```

In this example, the integer `data` is initialized to 100 in the root process (process 0). The `MPI_Bcast` function is called to broadcast the value of `data` to all processes in the communicator. After the broadcast, each process prints the received value.

### MPI_Bcast Example with array

#### mpi_bcast_example.cpp

```cpp
#include <mpi.h>
#include <stdio.h>
#define N 10

int main() {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int arr[N];
    if (rank == 0) {
        for(int i = 0; i < N; i++) arr[i] = i + 1;
    }

    // Broadcast the data from the root process to all processes
    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i = 0; i < N; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
```

#### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_bcast1.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_bcast1.out 5
  ```

  **Output:**

```text

  1 2 3 4 5 6 7 8 9 10
  1 2 3 4 5 6 7 8 9 10
  1 2 3 4 5 6 7 8 9 10
  1 2 3 4 5 6 7 8 9 10
  1 2 3 4 5 6 7 8 9 10

  ```

In this example, the integer `data` is initialized to 100 in the root process (process 0). The `MPI_Bcast` function is called to broadcast the value of `data` to all processes in the communicator. After the broadcast, each process prints the received value.

### send this ranksum to all the process using bcast

```cpp
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
    }
    else{
        rankSum = rank;
        int temp;
        for(int i = 1; i < size; i++){
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            rankSum+= temp;
        }
    }
    MPI_Bcast(&rankSum, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Rank %d : Sum of all the ranks = %d\n", rank, rankSum);

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh rankSum2.cpp
```

```bash
../../scripts/run.sh ./rankSum2.out 10
```

**Output:**

```text

Rank 0 : Sum of all the ranks = 45
Rank 1 : Sum of all the ranks = 45
Rank 3 : Sum of all the ranks = 45
Rank 2 : Sum of all the ranks = 45
Rank 8 : Sum of all the ranks = 45
Rank 4 : Sum of all the ranks = 45
Rank 9 : Sum of all the ranks = 45
Rank 6 : Sum of all the ranks = 45
Rank 7 : Sum of all the ranks = 45
Rank 5 : Sum of all the ranks = 45

```

## Tag

```cpp
#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2, data3, data4, data5;
    if(rank == 0){
        data1 = 100;
        data2 = 200;
        data3 = 300;
        data4 = 400;
        data5 = 500;
        MPI_Request request;
        MPI_Isend(&data1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data3, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data4, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data5, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    }
    else if(rank == 1){
        MPI_Recv(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data4, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&data5, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process %d\n", rank);
        printf("data1 %d\n", data1);
        printf("data2 %d\n", data2);
        printf("data3 %d\n", data3);
        printf("data4 %d\n", data4);
        printf("data5 %d\n", data5);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh tag.cpp
```

```bash
../../scripts/run.sh ./tag.out 10
```

**Output:**

```text

process 1
data1 100
data2 200
data3 300
data4 400
data5 500

```

## Anytag, Anysource

```cpp
#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2;
    if(rank != 0){
        data1 = rank;
        data2 = 234;
        MPI_Request request;
        MPI_Isend(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    }
    if(rank == 0){
        MPI_Recv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("data1 %d\n", data1);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh anySource.cpp
```

```bash
../../scripts/run.sh ./anySource.out 10
```

**Output:**

```text

data1 7

```

## MPI_Status

```cpp
#include <mpi.h>
#include <stdio.h>

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data1, data2;
    if(rank != 0){
        data1 = rank;
        data2 = 234;
        MPI_Request request;
        MPI_Isend(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&data2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    }
    if(rank == 0){
        MPI_Status status;
        MPI_Recv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("data1 %d\n", data1);
        printf("sender %d\n", status.MPI_SOURCE);
        printf("tag %d\n", status.MPI_TAG);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh mpi_status.cpp
```

```bash
../../scripts/run.sh ./mpi_status.out 10
```

**Output:**

```text

data1 3
sender 3
tag 0

```

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
../../scripts/run.sh ./task1.out 5
```

**Output:**

```text

1
2
3
5
4

```

---
[← Day 11 — MPI: Point-to-Point (Blocking/Non-blocking)](../../mpi/day11-point-to-point/) · [Index](../../README.md) · [Day 13 — MPI: Scatter & Gather](../../mpi/day13-scatter-gather/)
