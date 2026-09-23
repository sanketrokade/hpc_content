# Day 11 — MPI: Point-to-Point (Blocking/Non-blocking)

[← Day 10 — MPI: Introduction & Hello World](../../mpi/day10-mpi-intro/) · [Index](../../README.md) · [Day 12 — MPI: Broadcast, Tags, Status](../../mpi/day12-bcast-tags-status/)

## Agenda

- Recap
- Communicators
- MPI Communications
- MPI Programs
- Point to point communications
- Synchronous and Asynchronous calls
- Non-blocking calls
- Sum using p2p communication

## MPI Communicators

Communicators in MPI define a group of processes that can communicate with each other. The default communicator is `MPI_COMM_WORLD`, which includes all the processes. Custom communicators can be created to define subgroups of processes for specific communication patterns.

## Types of MPI Communications

MPI offers various communication mechanisms to facilitate different types of data exchanges between processes:

### Point-to-Point Communication:

- ****Blocking****: The sending and receiving operations wait until the message is delivered (e.g., `MPI_Send`, `MPI_Recv`).
- ****Non-Blocking****: The operations return immediately, allowing computation and communication to overlap (e.g., `MPI_Isend`, `MPI_Irecv`).

### Collective Communication:

These operations involve a group of processes and include:

- ****Broadcast****: Send data from one process to all other processes (`MPI_Bcast`).
- ****Scatter****: Distribute distinct chunks of data from one process to all processes (`MPI_Scatter`).
- ****Gather****: Collect chunks of data from all processes to one process (`MPI_Gather`).
- ****All-to-All****: Every process sends and receives distinct chunks of data (`MPI_Alltoall`).

Collectives can also include operations like reductions (`MPI_Reduce`, `MPI_Allreduce`) which perform computations on data from all processes and distribute the result.

## Point-to-point communication

```cpp
#include"stdio.h"
#include"mpi.h"

int main(int argc, char **argv)
{
    int myid, size;
    int myval;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if(myid==0){
        myval = 100;
        printf("\nmyid: %d \t myval = %d", myid, myval);
        MPI_Send(&myval, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("\nmyid: %d \t Data sent.\n", myid);
    }
    else if(myid==1){   // Process with ID exactly equal to 1
        myval = 200;
        MPI_Recv(&myval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\nmyid: %d \t Data received.", myid);
        printf("\nmyid: %d \t myval = %d", myid, myval);
        printf("\n\nProgram exit!\n");
    }

    //End MPI environment
    MPI_Finalize();
}
```

```bash
../../scripts/compile.sh p2p_mpi.cpp
```

```bash
../../scripts/run.sh ./p2p_mpi.out 2
```

**Output:**

```text

myid: 0      myval = 100
myid: 0      Data sent.

myid: 1      Data received.
myid: 1      myval = 100

Program exit!

```

### Sending array to process 1

```cpp
#include"stdio.h"
#include"mpi.h"
#define N 100

int main()
{
    int myid, size;
    int myval;

    int arr[N];
    //Initialize MPI environment
    MPI_Init(NULL,NULL);

    //Get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //Get my unique ID among all processes
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Process with ID exactly equal to 0
    if(myid==0){
        //Initialize data to be sent
        for(int i = 0; i < N; i++) arr[i] = i + 1;
        //Send data
        MPI_Send(arr, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("\nmyid: %d \t Data sent.\n", myid);
    }
    else if(myid==1){   // Process with ID exactly equal to 1
        //Initialize receive array to some other data
        MPI_Recv(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("\nmyid: %d \t Data received.\n", myid);
        //Print received data
        for(int i = 0; i < N; i++)
          printf("%d ", arr[i]);
    }

    //End MPI environment
    MPI_Finalize();
}
```

```bash
../../scripts/compile.sh p2p_mpi_array.cpp
```

```bash
../../scripts/run.sh ./p2p_mpi_array.out 2
```

**Output:**

```text

myid: 1      Data received.

myid: 0      Data sent.
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100
```

## MPI Communication: Synchronous and Asynchronous

### Synchronous Communication using MPI_Send and MPI_Recv

In synchronous communication, the send operation does not complete until the matching receive operation has been started.

#### mpi_sync.cpp

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (rank == 0) {
        number = -1;
        MPI_Ssend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
    } else if (rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }

    MPI_Finalize();
    return 0;
}
```

#### Compilation and Execution (Synchronous)

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_sync.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_sync.out 2
  ```

  **Output:**

```text

  Process 1 received number -1 from process 0
  Process 0 sent number -1 to process 1

  ```

### Asynchronous Communication using MPI_Isend and MPI_Irecv

In asynchronous communication, the send operation can complete before the matching receive operation starts. Non-blocking operations allow computation and communication to overlap.

#### mpi_async.cpp

```cpp
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (rank == 0) {
        number = -1;
        MPI_Request request;
        MPI_Isend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
    } else if (rank == 1) {
        MPI_Request request;
        MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        printf("Process 1 received number %d from process 0\n", number);
    }

    MPI_Finalize();
    return 0;
}
```

#### Compilation and Execution (Asynchronous)

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_async.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_async.out 2
  ```

  **Output:**

```text

  Process 0 sent number -1 to process 1
  Process 1 received number 31121 from process 0

  ```

## MPI Array Sum Calculation Example

### flow of your program

- initialize mpi environment
- let process 0 create and initialize the whole data
- now process 0 will send the complete data to all other process
- now every process is having the complete data
- to define start and end for each process to allow them perform computation on their part of data only
- every process will start their computation of performing localsum on their part of data from start to end
- then each process will send their computed localsum to process 0
- 0 should receive the localsum of each process and at the same time it should add it to a variable totalsum
- once your total localsum is received by all the process 0 should print the result on your screen.
- finalize mpi environment

### mpi_array_sum.cpp

```cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n = 10000; // Size of the array
    int *array = NULL;
    int chunk_size = n / world_size;
    int *sub_array = (int*)malloc(chunk_size * sizeof(int));

    if (world_rank == 0) {
        array = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            array[i] = i + 1; // Initialize the array with values 1 to n
        }

        // Distribute chunks of the array to other processes
        for (int i = 1; i < world_size; i++) {
            MPI_Send(array + i * chunk_size, chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Copy the first chunk to sub_array
        for (int i = 0; i < chunk_size; i++) {
            sub_array[i] = array[i];
        }
    } else {
        // Receive chunk of the array
        MPI_Recv(sub_array, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Compute the local sum
    int local_sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        local_sum += sub_array[i];
    }

    if (world_rank != 0) {
        // Send local sum to process 0
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // Process 0 receives the local sums and computes the final sum
        int final_sum = local_sum;
        int temp_sum;
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&temp_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            final_sum += temp_sum;
        }
        printf("The total sum of array elements is %d\n", final_sum);
    }

    free(sub_array);
    if (world_rank == 0) {
        free(array);
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_array_sum.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_array_sum.out 7
  ```

  **Output:**

```text

  The total sum of array elements is 49965006

  ```

## Task1

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    if (rank == 0) {
        number = 100;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received number %d from process 1\n", number);
    } else if (rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
        number = 200;
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Process 1 sent number %d to process 0\n", number);
    } else{
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task1.cpp
```

```bash
../../scripts/run.sh ./task1.out 2
```

**Output:**

```text

Process 0 sent number 100 to process 1
Process 0 received number 200 from process 1
Process 1 received number 100 from process 0
Process 1 sent number 200 to process 0

```

## Task2

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number1, number2;
    if (rank == 0) {
        number1 = 100;
        number2 = 200;
        MPI_Send(&number1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&number2, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number1);
        printf("Process 0 sent number %d to process 1\n", number2);
    } else if (rank == 1) {
        MPI_Recv(&number1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&number2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 n1 received number %d from process 0\n", number1);
        printf("Process 1 n2 received number %d from process 0\n", number2);
    } else{
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task2.cpp
```

```bash
../../scripts/run.sh ./task2.out 2
```

**Output:**

```text

Process 0 sent number 100 to process 1
Process 0 sent number 200 to process 1
Process 1 n1 received number 100 from process 0
Process 1 n2 received number 200 from process 0

```

## Task3

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number1, number2;
    if (rank == 0) {
        number1 = 100;
        number2 = 200;
        MPI_Request request;
        MPI_Isend(&number1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Isend(&number2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        printf("Process 0 sent number %d to process 1\n", number1);
        printf("Process 0 sent number %d to process 1\n", number2);
    } else if (rank == 1) {
        MPI_Request request;
        MPI_Irecv(&number1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        MPI_Irecv(&number2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number1);
        printf("Process 1 received number %d from process 0\n", number2);
    } else{
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task3.cpp
```

```bash
../../scripts/run.sh ./task3.out 2
```

**Output:**

```text

Process 0 sent number 100 to process 1
Process 0 sent number 200 to process 1
Process 1 received number 100 from process 0
Process 1 received number 200 from process 0

```

## Task4

```cpp
#include <mpi.h>
#include <stdio.h>
#define N 10000

int main(int argc, char** argv) {
    int arr[N];
    for(int i = 0; i < N; i++){
        arr[i] = i + 1;
    }
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int chunksize = N / size;
    int start = chunksize * rank;
    int end = (rank + 1) * chunksize;
    if(rank == size - 1) end = N;
    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int localSum = 0;
    for(int i = start; i < end; i++){
        localSum+= arr[i];
    }
    if(rank != 0){
        MPI_Send(&localSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        int totalSum = 0;
        totalSum += localSum;
        for(int i = 1; i < size; i++){
            MPI_Recv(&localSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalSum += localSum;
        }
        printf("Total sum = %d\n", totalSum);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh task4.cpp
```

```bash
../../scripts/run.sh ./task4.out 10
```

**Output:**

```text

Total sum = 50005000

```

## test

### test.cpp

```cpp
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

```

```bash
../../scripts/compile.sh test.cpp
```

```bash
../../scripts/run.sh ./test.out 8
```

**Output:**

```text

Total sum = 705082704

```

## sum1.cpp

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
    int arr[n];
    for(int i = 0; i < n; i++) arr[i] = i + 1;
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
    }

    int localsum = 0;
    for(int i = start; i < end; i++){
        localsum += arr[i];
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
        printf("totalsum = %d\n", totalsum);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh sum1.cpp
```

```bash
../../scripts/run.sh ./sum1.out 7
```

**Output:**

```text

totalsum = 500500

```

## sum2.cpp

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main(){
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 10000;
    int arr[n];
    if(rank == 0){
        for(int i = 0; i < n; i++) arr[i] = i + 1;
        for(int i = 1; i < size; i++){
            MPI_Send(arr, n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(arr, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
    }

    int localsum = 0;
    for(int i = start; i < end; i++){
        localsum += arr[i];
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
        printf("totalsum = %d\n", totalsum);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh sum2.cpp
```

```bash
../../scripts/run.sh ./sum2.out 7
```

**Output:**

```text

totalsum = 50005000

```

## sum3.cpp

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
    int arr[n];
    int chunksize = n / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1){
        end = n;
    }
    for(int i = start; i < end; i++){
        arr[i] = i + 1;
    }

    int localsum = 0;
    for(int i = start; i < end; i++){
        localsum += arr[i];
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
        printf("totalsum = %d\n", totalsum);
    }

    MPI_Finalize();
    return 0;
}
```

```bash
../../scripts/compile.sh sum3.cpp
```

```bash
../../scripts/run.sh ./sum3.out 6
```

**Output:**

```text

totalsum = 500500

```

---
[← Day 10 — MPI: Introduction & Hello World](../../mpi/day10-mpi-intro/) · [Index](../../README.md) · [Day 12 — MPI: Broadcast, Tags, Status](../../mpi/day12-bcast-tags-status/)
