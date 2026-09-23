# Day 1 — Pthreads: Creating Threads

[← Day 0 — Serial & Compiler Optimizations](../../optimization/day00-compiler-optimizations/) · [Index](../../README.md) · [Day 2 — Pthreads: Race Conditions & Mutex](../../pthreads/day02-race-conditions-mutex/)

## Agenda

- What is thread
- What are Pthreads
- Pthreads Overview
- Why Pthreads
- Designing Threaded Programs
- Pthreads APIs
- Creating and Terminating Threads

## Terminologies to remember

- threads
- pthread
- process
- context
- context switching
- concurrency
- parallelism
- multithreading
- cores
- hypterthreading

## Pthread Hello World

### code

```cpp
#include<stdio.h>
#include<pthread.h>
void* hello(void*){
    printf("Hello, World\n");
}
int main(){
    pthread_t t;
    pthread_create(&t, NULL, hello, NULL);
    pthread_join(t, NULL);
    return 0;
}
```

### compile

```bash
g++ pth1.cpp -o pth1.out -lpthread
```

### run

```bash
./pth1.out
```

**Output:**

```text
Hello, World
```

## creating 2 threads

### code

```cpp
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void* task1(void*){
    printf("starting task1\n");
    sleep(10);
    printf("ending task1\n");
}

void* task2(void*){
    printf("starting task2\n");
    sleep(10);
    printf("ending task2\n");
}
int main(){
    pthread_t t1, t2;
    pthread_create(&t1, NULL, task1, NULL);
    pthread_create(&t2, NULL, task2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
```

### compile

```bash
g++ twoThreads.cpp -lpthread
```

### run

```bash
./a.out
```

**Output:**

```text
starting task1
starting task2
ending task1
ending task2
```

## creating N number of threads

### code

```cpp
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define N 10

void* task(void*){
    printf("starting task\n");
    sleep(5);
    printf("ending task\n");
}
int main(){
    pthread_t t[N];
    for(int i = 0; i < N; i++){
        pthread_create(&t[i], NULL, task, NULL);
    }

    for(int i = 0; i < N; i++){
        pthread_join(t[i], NULL);
    }
    return 0;
}
```

### compile

```bash
g++ nthreads.cpp -o nthreads.out -lpthread
```

### run

```bash
./nthreads.out
```

**Output:**

```text
starting task
starting task
starting task
starting task
starting task
starting task
starting task
starting task
starting task
starting task
ending task
ending task
ending task
ending task
ending task
ending task
ending task
ending task
ending task
ending task
```

## Devide two tasks between equal number of threads

### code

```cpp
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define N 20

void* task1(void*){
    printf("starting task1\n");
    sleep(2);
    printf("ending task1\n");
}

void* task2(void*){
    printf("starting task2\n");
    sleep(2);
    printf("ending task2\n");

}

int main(){
    pthread_t t1[N];
    for(int i = 0; i < N; i++){
        if(i < N/2)
            pthread_create(&t1[i], NULL, task1, NULL);
        else
            pthread_create(&t1[i], NULL, task2, NULL);

    }

    for(int i = 0; i < N; i++){
        pthread_join(t1[i], NULL);
    }
    return 0;
}
```

### compile

```bash
g++ devideTaskBetweenThreads.cpp -lpthread
```

### run

```bash
./a.out
```

**Output:**

```text
starting task1
starting task1
starting task1
starting task1
starting task1
starting task1
starting task1
starting task1
starting task1
starting task1
starting task2
starting task2
starting task2
starting task2
starting task2
starting task2
starting task2
starting task2
starting task2
starting task2
ending task1
ending task1
ending task1
ending task1
ending task1
ending task1
ending task1
ending task1
ending task2
ending task1
ending task2
ending task1
ending task2
ending task2
ending task2
ending task2
ending task2
ending task2
ending task2
ending task2
```

---
[← Day 0 — Serial & Compiler Optimizations](../../optimization/day00-compiler-optimizations/) · [Index](../../README.md) · [Day 2 — Pthreads: Race Conditions & Mutex](../../pthreads/day02-race-conditions-mutex/)
