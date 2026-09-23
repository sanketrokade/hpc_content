# Day 9 — OpenMP: Atomic, Locks, Sections, Tasks

[← Day 8 — OpenMP: Loop Scheduling](../../openmp/day08-scheduling/) · [Index](../../README.md) · [Day 10 — MPI: Introduction & Hello World](../../mpi/day10-mpi-intro/)

## Agenda

- omp atomic
- omp sections
- nested parallelism

## test

```c
#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Task 1 executed by thread %d\n", omp_get_thread_num());
                #pragma omp parallel for
                for (int i = 0; i < 5; i++) {
                    printf("  Task 1: Loop iteration %d executed by thread %d\n", i, omp_get_thread_num());
                }
            }
            #pragma omp section
            {
                printf("Task 2 executed by thread %d\n", omp_get_thread_num());
                #pragma omp parallel for
                for (int i = 0; i < 5; i++) {
                    printf("  Task 2: Loop iteration %d executed by thread %d\n", i, omp_get_thread_num());
                }
            }
        }
    }
    return 0;
}

```

```bash
gcc test.c -fopenmp
```

```bash
export OMP_NESTED=TRUE
./a.out
```

**Output:**

```text
Task 1 executed by thread 1
Task 2 executed by thread 0
  Task 2: Loop iteration 3 executed by thread 3
  Task 2: Loop iteration 0 executed by thread 0
  Task 2: Loop iteration 2 executed by thread 2
  Task 2: Loop iteration 4 executed by thread 4
  Task 2: Loop iteration 1 executed by thread 1
  Task 1: Loop iteration 3 executed by thread 3
  Task 1: Loop iteration 4 executed by thread 4
  Task 1: Loop iteration 1 executed by thread 1
  Task 1: Loop iteration 2 executed by thread 2
  Task 1: Loop iteration 0 executed by thread 0
```

## Critical sections

```c
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 10000
#define T 10
int main(){
    int sum = 0;
    #pragma omp parallel for num_threads(10)
    for(int i = 0; i < N; i++){
        #pragma omp critical
        {
            sum+= i + 1;
        }
    }
    printf("Sum = %d\n", sum);

    return 0;
}
```

```bash
gcc criticalSection.c -fopenmp -o criticalSection.out
```

```bash
./criticalSection.out
```

**Output:**

```text
Sum = 50005000
```

## Atomic

- read
- write
- update
- capture

```c
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 100000000
#define T 10
int main(){
    long long  csum = 0, asum = 0;
    double startCritical = omp_get_wtime();
    #pragma omp parallel for num_threads(10)
    for(long long i = 0; i < N; i++){
        #pragma omp critical
        {
            csum+= i + 1;
        }
    }
    double endCritical = omp_get_wtime();

    double startAtomic = omp_get_wtime();
    #pragma omp parallel for num_threads(10)
    for(long long i = 0; i < N; i++){
        #pragma omp atomic
        asum+= i + 1;
    }
    double endAtomic = omp_get_wtime();
    printf("Time taken by critical section : %lf\n", endCritical - startCritical);
    printf("Critical sum : %lld\n", csum);
    printf("Time taken by atomic: %lf\n", endAtomic - startAtomic);
    printf("Atomic sum : %lld\n", asum);

    return 0;
}
```

```bash
gcc atomic.c -fopenmp -o atomic.out
```

```bash
./atomic.out
```

**Output:**

```text
Time taken by critical section : 8.757443
Critical sum : 5000000050000000
Time taken by atomic: 2.764866
Atomic sum : 5000000050000000
```

## Nested parallelism

```c
#include<stdio.h>
#include<omp.h>
int main(){
    //omp_set_nested(1); //using this function you can enable desable nested parallelism
    #pragma omp parallel num_threads(2)
    {
        printf("Level 1 : Id %d\n", omp_get_thread_num());
        #pragma omp parallel num_threads(2)
        {
            printf("Level 2 : Id %d\n", omp_get_thread_num());
        }
    }
}
```

```bash
gcc nested.c -o nested.out -fopenmp
```

```bash
export OMP_NESTED=TRUE
./nested.out
```

**Output:**

```text
Level 1 : Id 1
Level 1 : Id 0
Level 2 : Id 1
Level 2 : Id 0
Level 2 : Id 1
Level 2 : Id 0
```

## Sections (task parallelism)

```c
#include<stdio.h>
#include<omp.h>
#define N 10000
#define T 10
int main(){
    #pragma omp parallel num_threads(3)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
        }
    }
}
```

```bash
gcc section.c -fopenmp -o section.out
```

```bash
./section.out
```

**Output:**

```text
This section is executed by thread 2
This section is executed by thread 2
This section is executed by thread 0
This section is executed by thread 1
```

## task parallelism

```c
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 1000000
#define T 10
int main(){
    long long *a;
    long long sum = 0, sumSquare = 0;
    a = (long long*) malloc(sizeof(long long) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
    }
    double startTime = omp_get_wtime();
    #pragma omp parallel num_threads(T)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for(int i = 0; i < N; i++){
                    sum+= a[i];
                }
            }
            #pragma omp section
            {
                for(int i = 0; i < N; i++){
                    sumSquare += a[i] * a[i];
                }
            }
        }
    }
    double endTime = omp_get_wtime();
    printf("Sum = %lld\n", sum);
    printf("Sum of Squares = %lld\n", sumSquare);
    printf("Execution time = %lf\n", endTime - startTime);
    free(a);
}
```

```bash
gcc taskParallelism.c -fopenmp -o taskParallelism.out
```

```bash
./taskParallelism.out
```

**Output:**

```text
Sum = 500000500000
Sum of Squares = 333333833333500000
Execution time = 0.005666
```

## task parallelism1

```c
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 1000000
#define T 10
int main(){
    long long *a;
    long long sum = 0, sumSquare = 0;
    a = (long long*) malloc(sizeof(long long) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
    }
    omp_set_nested(1);
    double startTime = omp_get_wtime();
    #pragma omp parallel num_threads(T)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                #pragma omp parallel for reduction(+ : sum) num_threads(T)
                for(int i = 0; i < N; i++){
                    sum+= a[i];
                }
            }
            #pragma omp section
            {
                #pragma omp parallel for reduction(+ : sumSquare) num_threads(T)
                for(int i = 0; i < N; i++){
                    sumSquare += a[i] * a[i];
                }
            }
        }
    }
    double endTime = omp_get_wtime();
    printf("Sum = %lld\n", sum);
    printf("Sum of Squares = %lld\n", sumSquare);
    printf("Execution time = %lf\n", endTime - startTime);
    free(a);
}
```

```bash
gcc taskParallelism1.c -fopenmp -o taskParallelism1.out
```

```bash
./taskParallelism1.out
```

**Output:**

```text
Sum = 500000500000
Sum of Squares = 333333833333500000
Execution time = 0.001996
```

## task parallelism1

```c
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 1000000
#define T 10
int main(){
    long long *a;
    long long sum = 0, sumSquare = 0;
    a = (long long*) malloc(sizeof(long long) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
    }
    for(int i = 0; i < N; i++){
            sum+= a[i];
    }
    for(int i = 0; i < N; i++){
            sumSquare += a[i] * a[i];
    }
    printf("Sum = %lld\n", sum);
    printf("Sum of Squares = %lld\n", sumSquare);
    free(a);
}
```

```bash
gcc taskParallelism1.c -fopenmp -o taskParallelism1.out
```

```bash
./taskParallelism1.out
```

**Output:**

```text
Sum = 500000500000
Sum of Squares = 333333833333500000
Execution time = 0.001996
```

## Locks

```c
#include <omp.h>
#include <stdio.h>
omp_lock_t lock;

void solve(int thread_id) {
    omp_set_lock(&lock);
    printf("Thread %d is taking the lock\n", thread_id);

    printf("Thread %d is releasing the lock\n", thread_id);
    omp_unset_lock(&lock);
}

int main() {
    omp_init_lock(&lock);
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        solve(thread_id);
    }
    omp_destroy_lock(&lock);
    return 0;
}
```

```bash
gcc locks.c -o locks.out -fopenmp
```

```bash
./locks.out
```

**Output:**

```text
Thread 6 is taking the lock
Thread 6 is releasing the lock
Thread 11 is taking the lock
Thread 11 is releasing the lock
Thread 9 is taking the lock
Thread 9 is releasing the lock
Thread 0 is taking the lock
Thread 0 is releasing the lock
Thread 10 is taking the lock
Thread 10 is releasing the lock
Thread 2 is taking the lock
Thread 2 is releasing the lock
Thread 5 is taking the lock
Thread 5 is releasing the lock
Thread 8 is taking the lock
Thread 8 is releasing the lock
Thread 4 is taking the lock
Thread 4 is releasing the lock
Thread 1 is taking the lock
Thread 1 is releasing the lock
Thread 7 is taking the lock
Thread 7 is releasing the lock
Thread 3 is taking the lock
Thread 3 is releasing the lock
```

---
[← Day 8 — OpenMP: Loop Scheduling](../../openmp/day08-scheduling/) · [Index](../../README.md) · [Day 10 — MPI: Introduction & Hello World](../../mpi/day10-mpi-intro/)
