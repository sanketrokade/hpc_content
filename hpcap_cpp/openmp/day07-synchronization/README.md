# Day 7 — OpenMP: Synchronization (critical, single, master, barrier)

[← Day 6 — OpenMP: Reduction & Timing](../../openmp/day06-reduction-timing/) · [Index](../../README.md) · [Day 8 — OpenMP: Loop Scheduling](../../openmp/day08-scheduling/)

## Agenda

- lastprivate
- critical section
- omp single
- omp master
- omp barrier
- parallel data creation

## Lastprivate

```cpp
#include<stdio.h>
#include<omp.h>
#define T 10
#define N 20
int main(){
    int a = 5;
    #pragma omp parallel for lastprivate(a) num_threads(T)
    for(int i = 0; i < N; i++){
        if(i == N - 1){
            continue;
        }
        a = i;
        printf("thread %d is changing a to %d\n", omp_get_thread_num(), a);
    }
    printf("a = %d\n", a);

    return 0;
}
```

```bash
g++ lastprivate.cpp -o lastprivate.out -fopenmp
```

```bash
./lastprivate.out
```

**Output:**

```text
thread 4 is changing a to 8
thread 1 is changing a to 2
thread 7 is changing a to 14
thread 4 is changing a to 9
thread 1 is changing a to 3
thread 5 is changing a to 10
thread 7 is changing a to 15
thread 5 is changing a to 11
thread 3 is changing a to 6
thread 3 is changing a to 7
thread 6 is changing a to 12
thread 6 is changing a to 13
thread 0 is changing a to 0
thread 0 is changing a to 1
thread 9 is changing a to 18
thread 2 is changing a to 4
thread 8 is changing a to 16
thread 8 is changing a to 17
thread 2 is changing a to 5
a = 18
```

## Lastprivate2

```cpp
#include<stdio.h>
#include<omp.h>
#define T 5
#define N 20
int main(){
    int a = 5;
    #pragma omp parallel for lastprivate(a) num_threads(T)
    for(int i = 0; i < N; i++){
        if(i > 16){
            continue;
        }
        a = i;
        printf("thread %d is changing a to %d\n", omp_get_thread_num(), a);
    }
    printf("a = %d\n", a);

    return 0;
}
```

```bash
g++ lastprivate2.cpp -o lastprivate2.out -fopenmp
```

```bash
./lastprivate2.out
```

**Output:**

```text
thread 4 is changing a to 16
thread 0 is changing a to 0
thread 0 is changing a to 1
thread 0 is changing a to 2
thread 0 is changing a to 3
thread 3 is changing a to 12
thread 3 is changing a to 13
thread 3 is changing a to 14
thread 3 is changing a to 15
thread 1 is changing a to 4
thread 1 is changing a to 5
thread 1 is changing a to 6
thread 1 is changing a to 7
thread 2 is changing a to 8
thread 2 is changing a to 9
thread 2 is changing a to 10
thread 2 is changing a to 11
a = 16
```

## Critical sections

```cpp
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
g++ criticalSection.cpp -fopenmp -o criticalSection.out
```

```bash
./criticalSection.out
```

**Output:**

```text
Sum = 50005000
```

## You can use critical section for manual reduction

```cpp
#include<stdio.h>
#include<omp.h>
#define N 1000000000
#define T 13
int main(){
    int chunksize = N / T;
    long long sum = 0;
    #pragma omp parallel num_threads(T)
    {
        int tid = omp_get_thread_num();
        long long localsum = 0;
        int start = tid * chunksize;
        int end = start + chunksize;
        if(tid == T - 1) end = N;
        for(int i = start; i < end; i++){
            localsum += i + 1;
        }
        #pragma omp critical
        {
            sum += localsum;
        }
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");

    return 0;
}
```

```bash
g++ criticalSection2.cpp -fopenmp -o criticalSection2.out
```

```bash
./criticalSection2.out
```

**Output:**

```text
Calculated sum = 500000000500000000
Expected sum = 500000000500000000
_____Passed_____
```

## Critical Section usage

```cpp
#include<stdio.h>
#include<omp.h>
#define N 1000000000
#define T 13
int main(){
    long long sum = 0;
    #pragma omp parallel num_threads(T)
    {
        int tid = omp_get_thread_num();
        long long localsum = 0;
        #pragma omp for
        for(int i = 0; i < N; i++){
            localsum += i + 1;
        }
        #pragma omp critical
        {
            sum+= localsum;
        }
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");

    return 0;
}
```

```bash
g++ criticalSection3.cpp -fopenmp -o criticalSection3.out
```

```bash
./criticalSection3.out
```

**Output:**

```text
Calculated sum = 500000000500000000
Expected sum = 500000000500000000
_____Passed_____
```

## Master

```cpp
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 10000
#define T 10
int main(){
    #pragma omp parallel num_threads(T)
    {
        #pragma omp master
        {
            printf("%d have an extra statement to execute\n", omp_get_thread_num());
        }
        printf("Hi, I am thread %d\n", omp_get_thread_num());
    }
    return 0;
}
```

```bash
g++ master.cpp -fopenmp -o master.out
```

```bash
./master.out
```

**Output:**

```text
Hi, I am thread 2
Hi, I am thread 6
0 have an extra statement to execute
Hi, I am thread 0
Hi, I am thread 8
Hi, I am thread 3
Hi, I am thread 1
Hi, I am thread 4
Hi, I am thread 5
Hi, I am thread 7
Hi, I am thread 9
```

## Single

```cpp
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 10000
#define T 10
int main(){
    #pragma omp parallel num_threads(T)
    {
        #pragma omp single
        {
            printf("%d have an extra statement to execute\n", omp_get_thread_num());
        }
        printf("Hi, I am thread %d\n", omp_get_thread_num());
    }
    return 0;
}
```

```bash
g++ single.cpp -fopenmp -o single.out
```

```bash
./single.out
```

**Output:**

```text
4 have an extra statement to execute
Hi, I am thread 9
Hi, I am thread 4
Hi, I am thread 0
Hi, I am thread 3
Hi, I am thread 5
Hi, I am thread 2
Hi, I am thread 6
Hi, I am thread 7
Hi, I am thread 8
Hi, I am thread 1
```

## Data creation inside parallel region using single

```cpp
#include<stdio.h>
#include<omp.h>
#define N 1000000
#define T 13
int main(){
    long long sum = 0;
    long long a[N];
    #pragma omp parallel  num_threads(T)
    {
        #pragma omp single
        {
            for(int i = 0; i < N; i++) a[i] = i + 1;
        }
        #pragma omp for reduction( + : sum )
        for(int i = 0; i < N; i++){
            sum += a[i];
        }
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");
    return 0;
}
```

```bash
g++ dc1.cpp -fopenmp -o dc1.out
```

```bash
./dc1.out
```

**Output:**

```text
Calculated sum = -8991545208627516298
Expected sum = 500000500000
_____Failed_____
```

## Barrier

```cpp
#include<stdio.h>
#include<unistd.h>
#include<omp.h>
#define T 5
int main(){
    #pragma omp parallel num_threads(T)
    {
        printf("Before barrier\n");
        #pragma omp barrier
        printf("After barrier\n");
    }

    return 0;
}
```

```bash
g++ barrier.cpp -o barrier.out -fopenmp
```

```bash
./barrier.out
```

**Output:**

```text
Before barrier
Before barrier
Before barrier
Before barrier
Before barrier
After barrier
After barrier
After barrier
After barrier
After barrier
```

## Data creation inside parallel region using master

```cpp
#include<stdio.h>
#include<omp.h>
#define N 1000000
#define T 13
int main(){
    long long sum = 0;
    long long a[N];
    #pragma omp parallel  num_threads(T)
    {
        #pragma omp master
        {
            for(int i = 0; i < N; i++) a[i] = i + 1;
        }
        #pragma omp barrier
        #pragma omp for reduction( + : sum )
        for(int i = 0; i < N; i++){
            sum += a[i];
        }
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");
    return 0;
}
```

```bash
g++ dc2.cpp -fopenmp -o dc2.out
```

```bash
./dc2.out
```

**Output:**

```text
Calculated sum = 500000500000
Expected sum = 500000500000
_____Passed_____
```

## Barrier data creation

```cpp
#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 1000000
#define T 13

int main(){
    long long *a, sum=0;
    a= static_cast<long long*>(malloc(sizeof(long long) * N));

    #pragma omp parallel num_threads(T)
    {
        #pragma omp master
        {
            for(int i=0; i<N; i++){
                a[i] = i+1;
            }
        }
        #pragma omp for reduction(+:sum)
        for(int i=0; i<N; i++){
            sum += a[i];
        }
    }
    printf("sum = %lld\n", sum);

    return 0;
}
```

```bash
g++ dc3.cpp -fopenmp -o dc3.out
```

```bash
./dc3.out
```

**Output:**

```text
sum = 2958689350
```

## Data creation parallelly

```cpp
#include<stdio.h>
#include<omp.h>
#define N 1000000
#define T 13
int main(){
    long long sum = 0;
    long long a[N];
    #pragma omp parallel  num_threads(T)
    {
        #pragma omp for
        for(int i = 0; i < N; i++){
            a[i] = i + 1;
        }
        #pragma omp for reduction( + : sum )
        for(int i = 0; i < N; i++){
            sum += a[i];
        }
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");
    return 0;
}
```

```bash
g++ dc4.cpp -fopenmp -o dc4.out
```

```bash
./dc4.out
```

**Output:**

```text
Calculated sum = 500000500000
Expected sum = 500000500000
_____Passed_____
```

---
[← Day 6 — OpenMP: Reduction & Timing](../../openmp/day06-reduction-timing/) · [Index](../../README.md) · [Day 8 — OpenMP: Loop Scheduling](../../openmp/day08-scheduling/)
