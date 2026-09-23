# Day 10 — MPI: Introduction & Hello World

[← Day 9 — OpenMP: Atomic, Locks, Sections, Tasks](../../openmp/day09-atomic-locks-tasks/) · [Index](../../README.md) · [Day 11 — MPI: Point-to-Point (Blocking/Non-blocking)](../../mpi/day11-point-to-point/)

## Introduction to MPI

MPI (Message Passing Interface) is a standardized library for message-passing in parallel programming. It allows multiple processes to communicate and coordinate tasks across distributed-memory systems. MPI is widely used for high-performance computing applications.

- ****Key Highlights****:
  - Enables communication between processes running on different nodes or cores.
  - Portable across various hardware and software platforms.
  - Scalable to thousands or even millions of processes.
  - Provides fine-grained control over communication patterns.

## Basics of MPI

- ****Processes****:
  - Each instance of an MPI program is a separate process.
  - Processes do not share memory and communicate explicitly via messages.
- ****Communicator****:
  - A group of processes that can communicate with each other.
  - The default communicator `MPI_COMM_WORLD` includes all processes.
- ****Rank****:
  - Each process in a communicator is assigned a unique ****rank**** (an integer).
  - Ranks are used to identify and address processes.
- ****Execution Model****:
  - All processes start execution from the same program code.
  - They can follow different execution paths based on their rank.

—

## Processes vs Threads

| ****Feature**** | ****Processes****                 | ****Threads****                    |
|-----------------|-----------------------------------|------------------------------------|
| Memory          | Separate memory for each process. | Shared memory within the process.  |
| Communication   | Message passing (explicit).       | Shared variables (implicit).       |
| Scalability     | Highly scalable.                  | Limited by shared memory capacity. |
| Example         | MPI programs.                     | OpenMP programs.                   |

—

## Distributed Memory Programming Model

- In distributed memory systems, processes execute on separate nodes, each with its own private memory.

- Communication between processes occurs explicitly using message-passing.

- ****Key Characteristics****:

  - No shared memory: Processes cannot directly access each other’s data.
  - Explicit communication: Processes exchange data via messages.
  - Suitable for large-scale distributed systems like clusters and supercomputers.

—

## Distributed vs Shared Memory

| ****Feature****            | ****Shared Memory****              | ****Distributed Memory****         |
|----------------------------|------------------------------------|------------------------------------|
| ****Memory Access****      | All threads share a global memory. | Each process has private memory.   |
| ****Communication****      | Implicit via shared variables.     | Explicit via message passing.      |
| ****Programming Models**** | OpenMP, Pthreads.                  | MPI, Sockets.                      |
| ****Scalability****        | Limited by shared memory size.     | Highly scalable for large systems. |

—

## Why MPI?

1.  ****Scalability****:
    - Handles thousands of processes efficiently on distributed systems.
2.  ****Portability****:
    - Works on diverse hardware architectures and operating systems.
3.  ****Flexibility****:
    - Provides control over data distribution, load balancing, and communication.
4.  ****Efficiency****:
    - Optimized for high-performance computing on clusters and supercomputers.

## Real-World Applications of MPI

- Climate modeling.
- Computational fluid dynamics.
- Genome sequencing.
- Financial simulations.

—

## How MPI Works

1.  ****Initialization****:
    - The MPI environment is set up using `MPI_Init`.
    - All processes start executing from the same program.
2.  ****Communication****:
    - Processes exchange data via point-to-point or collective communication.
    - Use communicators (e.g., `MPI_COMM_WORLD`) to define the scope of communication.
3.  ****Synchronization****:
    - Processes can synchronize using barriers or other mechanisms.
4.  ****Finalization****:
    - The MPI environment is cleaned up using `MPI_Finalize`.

—

## MPI Communications

- ****Point-to-Point Communication****:
  - Direct communication between two specific processes.
  - Example Functions:
    - `MPI_Send`: Sends a message.
    - `MPI_Recv`: Receives a message.
- ****Collective Communication****:
  - Involves all processes in a communicator.
  - Example Functions:
    - `MPI_Bcast`: Broadcasts a message to all processes.
    - `MPI_Reduce`: Combines data from all processes.

—

## MPI Hello World Example

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the size of the communicator (number of processes)
    int world_size      ;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the current process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Print a message from each process
    printf("Hello from process %d of %d\n", world_rank, world_size);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
```

—

## Detailed Explanation of Hello World Code

1.  ****MPI_Init****:
    - Initializes the MPI environment.
    - Required before calling any other MPI functions.
    - Syntax: ```c MPI_Init(&argc, &argv); ```
2.  ****MPI_COMM_WORLD****:
    - Default communicator that includes all processes in the MPI program.
    - Every process is part of this communicator.
3.  ****MPI_Comm_size****:
    - Retrieves the total number of processes in the communicator.
    - Syntax: ```c MPI_Comm_size(MPI_COMM_WORLD, &world_size); ```
    - Example:
      - If there are 4 processes, `world_size` will be `4`.
4.  ****MPI_Comm_rank****:
    - Retrieves the rank of the current process in the communicator.
    - Syntax: ```c MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); ```
    - Example:
      - If there are 4 processes, their ranks will be `0`, `1`, `2`, and `3`.
5.  ****MPI_Finalize****:
    - Cleans up the MPI environment.
    - Syntax: ```c MPI_Finalize(); ```

—

## Hello World in C

### code

```cpp
#include<stdio.h>
int main(){
    printf("Hello, World\n");
    return 0;
}
```

### compile

```bash
g++ hello.cpp -o hello.out
```

### run

```bash
./hello.out
```

**Output:**

```text
Hello, World
```

## Hello World in using MPI

### code

```cpp
#include<stdio.h>
#include<mpi.h>
int main(){
    MPI_Init(NULL, NULL);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello from process %d of %d\n", rank, size);
    MPI_Finalize();
    return 0;
}
```

### compile

```bash
#source ~/git/spack/share/spack/setup-env.sh
#spack load openmpi
#mpicxx hello.cpp
../../scripts/compile.sh hello1.cpp
```

### run

```bash
#source ~/git/spack/share/spack/setup-env.sh
#spack load openmpi
#mpirun -np 4 ./a.out
../../scripts/run.sh ./hello1.out 4
```

**Output:**

```text

Hello from process 1 of 4
Hello from process 0 of 4
Hello from process 3 of 4
Hello from process 2 of 4

```

## task1

```cpp
#include<stdio.h>
#include<mpi.h>
#define N 1000
int main(){
    int size, rank;
    int a[N];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunksize = N / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1) end = N;
    for(int i = start; i < end; i++){
        a[i] = i + 1;
    }

    for(int i = start; i < end; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    MPI_Finalize();
}
```

```bash
../../scripts/compile.sh task1.cpp
```

```bash
../../scripts/run.sh ./task1.out 4 > output.txt
```

## Point-to-point communication

```cpp
#include"stdio.h"
#include"mpi.h"

int main()
{
    int myid, size;
    int myval;
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if(myid==0){
        myval = 100;
        printf("\nmyid: %d \t myval = %d", myid, myval);
        for(int i = 1; i < size; i++){
            MPI_Send(&myval, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        printf("\nmyid: %d \t Data sent.\n", myid);
    }
    else{   // Process with ID exactly equal to 1
        if(myid == size - 1){
            printf("I left\n");
        }
        else{
               myval = 200;
               MPI_Recv(&myval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               printf("\nmyid: %d \t Data received.\n", myid);
               printf("\nmyid: %d \t myval = %d\n", myid, myval);
        }
    }

    MPI_Finalize();
}
```

```bash
../../scripts/compile.sh p2p_mpi.cpp
```

```bash
../../scripts/run.sh ./p2p_mpi.out 4
```

**Output:**

```text

myid: 0      myval = 100
myid: 0      Data sent.

myid: 1      Data received.
myid: 1      myval = 100

Program exit!
id = 2, myval = 32765
id = 3, myval = 32764

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

## Point to point communication

This will create 1000 send calls and 1000 recv calls which is not good for your network.

```cpp
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

        for(int i = 0; i < N; i++){
            MPI_Send(&a[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
    }
    else{
        for(int i = 0; i < N; i++){
            MPI_Recv(&a[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for(int i = N - 10; i < N; i++){
            printf("%d ", a[i]);
        }
    }

    MPI_Finalize();
}
```

```bash
../../scripts/compile.sh p2p.cpp
```

```bash
../../scripts/run.sh ./p2p.out 2
```

**Output:**

```text

991 992 993 994 995 996 997 998 999 1000
```

### Better way

```cpp
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
```

```bash
../../scripts/compile.sh p2p1.cpp
```

```bash
../../scripts/run.sh ./p2p1.out 10
```

**Output:**

```text

991 992 993 994 995 996 997 998 999 1000
991 992 993 994 995 996 997 998 999 1000
991 992 993 994 995 996 997 998 999 1000
991 992 993 994 995 996 997 998 999 1000

```

---
[← Day 9 — OpenMP: Atomic, Locks, Sections, Tasks](../../openmp/day09-atomic-locks-tasks/) · [Index](../../README.md) · [Day 11 — MPI: Point-to-Point (Blocking/Non-blocking)](../../mpi/day11-point-to-point/)
