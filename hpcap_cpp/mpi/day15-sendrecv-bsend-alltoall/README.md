# Day 15 — MPI: Sendrecv, Bsend, Ssend, Alltoall

[← Day 14 — MPI: Reduce & Allreduce](../../mpi/day14-reduce-allreduce/) · [Index](../../README.md) · [Day 16 — MPI: Derived Datatypes](../../mpi/day16-derived-datatypes/)

## MPI Alltoall Example

### mpi_alltoall_example.cpp

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
    int *send_data = (int*)malloc(size * sizeof(int));
    int *recv_data = (int*)malloc(size * sizeof(int));
    // Initialize send_data such that process i sends its rank to all processes
    for (int i = 0; i < size; i++) {
        send_data[i] = rank + i * 10;
    }

    printf("Process %d send data: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", send_data[i]);
    }
    printf("\n");
    // Perform all-to-all communication
    MPI_Alltoall(send_data, 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);
    printf("Process %d received data: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");
    free(send_data);
    free(recv_data);
    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_alltoall_example.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_alltoall_example.out 3
  ```

  **Output:**

```text

  Process 1 send data: 1 11 21
  Process 0 send data: 0 10 20
  Process 2 send data: 2 12 22
  Process 1 received data: 10 11 12
  Process 0 received data: 0 1 2
  Process 2 received data: 20 21 22

  ```

In this example, each process sends its rank and an incremented value to all other processes. The `MPI_Alltoall` function is used to exchange these values among all processes. Each process then prints the received values.

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
    int number;
    if (rank == 0) {
        number = 100;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received number %d from process 1\n", number);
    } else if (rank == 1) {
        number = 200;
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Process 1 sent number %d to process 0\n", number);
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
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
Process 0 received number 200 from process 1
Process 1 sent number 200 to process 0
Process 1 received number 100 from process 0

```

## MPI_Sendrecv Example

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sendBuf, recvBuf;
    if (rank == 0) {
        sendBuf = 100;
        MPI_Sendrecv(&sendBuf, 1, MPI_INT, 1, 0, &recvBuf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 sent %d and received number %d\n", sendBuf, recvBuf);
    } else if (rank == 1) {
        sendBuf = 200;
        MPI_Sendrecv(&sendBuf, 1, MPI_INT, 0, 0, &recvBuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received %d and sent number %d\n", recvBuf, sendBuf);
    } else {
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_sendrecv_example.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_sendrecv_example.out 2
  ```

  **Output:**

```text

  Process 0 sent 100 and received number 200
  Process 1 received 100 and sent number 200

  ```

In this example, `MPI_Sendrecv` is used to send and receive messages in a single call. Process 0 sends the number 100 to process 1 and receives a number from process 1. Process 1 receives the number from process 0, modifies it to 200, and sends it back to process 0.

## MPI_Sendrecv Example2

```cpp
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int Buffer;
    if (rank == 0) {
        Buffer = 100;
        MPI_Sendrecv(&Buffer, 1, MPI_INT, 1, 0, &Buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received number %d\n", Buffer);
    } else if (rank == 1) {
        Buffer = 200;
        MPI_Sendrecv(&Buffer, 1, MPI_INT, 0, 0, &Buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d\n", Buffer);
    } else {
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_sendrecv_example2.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_sendrecv_example2.out 2
  ```

  **Output:**

```text

  Process 1 received number 100
  Process 0 received number 200

  ```

In this example, `MPI_Sendrecv` is used to send and receive messages in a single call. Process 0 sends the number 100 to process 1 and receives a number from process 1. Process 1 receives the number from process 0, modifies it to 200, and sends it back to process 0.

## MPI_Sendrecv_replace Example

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
        MPI_Sendrecv_replace(&number, 1, MPI_INT, 1, 0, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 sent and received number %d\n", number);
    } else if (rank == 1) {
        number = 200;
        MPI_Sendrecv_replace(&number, 1, MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 sent and received number %d\n", number);
    } else {
        printf("I am process %d and I have nothing to do\n", rank);
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_sendrecv_replace_example.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_sendrecv_replace_example.out 2
  ```

  **Output:**

```text

  Process 0 sent and received number 200
  Process 1 sent and received number 100

  ```

In this example, `MPI_Sendrecv_replace` is used to send and receive messages using the same buffer. Process 0 sends the number 100 to process 1 and receives a number from process 1 into the same buffer. Process 1 receives the number from process 0, modifies it to 200, and sends it back to process 0 using the same buffer.

## MPI_Send and recv Example with larger data

```cpp
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define N 100000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *buffer;
    buffer = (int*) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        buffer[i] = (rank * i) + 1;
    }
    if(rank == 0){
        MPI_Send(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
    else if(rank == 1){
        MPI_Recv(buffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(buffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh task3.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./task3.out 2
  ```

  **Output:**

```text

  Process 0 is having : 1 1 1 1 1 1 1 1 1 1
  Process 1 is having : 1 1 1 1 1 1 1 1 1 1

  ```

In this example, `MPI_Sendrecv` is used to send and receive messages in a single call. Process 0 sends the number 100 to process 1 and receives a number from process 1. Process 1 receives the number from process 0, modifies it to 200, and sends it back to process 0.

## Swap data of two process

```cpp
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define N 100000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *buffer;
    buffer = (int*) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        buffer[i] = (rank * i) + i;
    }
    if(rank == 0){
        MPI_Send(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
    else if(rank == 1){
        int *tempBuffer = (int*) malloc(sizeof(int) * N);
        MPI_Recv(tempBuffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(buffer, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for(int i = 0; i < N; i++) buffer[i] = tempBuffer[i];
        free(tempBuffer);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh task4.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./task4.out 2
  ```

  **Output:**

```text

  Process 0 is having : 199980 199982 199984 199986 199988 199990 199992 199994 199996 199998
  Process 1 is having : 99990 99991 99992 99993 99994 99995 99996 99997 99998 99999

  ```

## Swap data of two process with MPI_Sendrecv_replace

```cpp
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define N 100000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *buffer;
    buffer = (int*) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        buffer[i] = (rank * i) + i;
    }
    if(rank == 0){
        MPI_Sendrecv_replace(buffer, N, MPI_INT, 1, 0, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
    else if(rank == 1){
        MPI_Sendrecv_replace(buffer, N, MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d is having : ", rank);
        for(int i = N - 10; i < N; i++){
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh task5.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./task5.out 2
  ```

  **Output:**

```text

  Process 0 is having : 199980 199982 199984 199986 199988 199990 199992 199994 199996 199998
  Process 1 is having : 99990 99991 99992 99993 99994 99995 99996 99997 99998 99999

  ```

## MPI_Bsend

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
    int number;
    if (rank == 0) {
        number = 100;

        int buffer_size = MPI_BSEND_OVERHEAD + sizeof(int);
        void*buffer = static_cast<void*>(malloc(buffer_size));
        MPI_Buffer_attach(buffer, buffer_size);

        MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);

        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    } else if (rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_bsend_example.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_bsend_example.out 2
  ```

  **Output:**

```text

  Process 0 sent number 100 to process 1
  Process 1 received number 100 from process 0

  ```

In this example, `MPI_Bsend` is used to send a number from process 0 to process 1 using a buffered send.

## MPI_Bsend Array

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

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int array_size = 1000000;
    int* array = (int*)malloc(array_size * sizeof(int));

    if (rank == 0) {
        // Initialize the array with some values
        for (int i = 0; i < array_size; i++) {
            array[i] = i + 1;
        }

        int buffer_size = MPI_BSEND_OVERHEAD + array_size * sizeof(int);
        void*buffer = static_cast<void*>(malloc(buffer_size));
        MPI_Buffer_attach(buffer, buffer_size);

        MPI_Bsend(array, array_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent array to process 1\n");

        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    } else if (rank == 1) {
        MPI_Recv(array, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received array from process 0: ");
        for (int i = array_size - 5; i < array_size; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    free(array);

    MPI_Finalize();
    return 0;
}
```

### Compilation and Execution

- Compile the program:

  ```bash
  ../../scripts/compile.sh mpi_bsend_array_example.cpp
  ```

- Run the program:

  ```bash
  ../../scripts/run.sh ./mpi_bsend_array_example.out 2
  ```

  **Output:**

```text

  Process 0 sent array to process 1
  Process 1 received array from process 0: 999996 999997 999998 999999 1000000

  ```

In this example, `MPI_Bsend` is used to send an array of integers from process 0 to process 1 using a buffered send. The received data is printed by process 1.

---
[← Day 14 — MPI: Reduce & Allreduce](../../mpi/day14-reduce-allreduce/) · [Index](../../README.md) · [Day 16 — MPI: Derived Datatypes](../../mpi/day16-derived-datatypes/)
