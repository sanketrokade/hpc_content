# Day 2 — Pthreads: Race Conditions & Mutex

[← Day 1 — Pthreads: Creating Threads](../../pthreads/day01-thread-basics/) · [Index](../../README.md) · [Day 3 — Pthreads: Reduction, Condition Variables, Barriers](../../pthreads/day03-reduction-condvars-barriers/)

## Agenda

- Passing arguments to the function
- Race condition
- Mutex

## Creating N number of threads

In this code you can see the issue while passing address of iteration for thread ID.

### code

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 20

void* hello(void* threadId){
    int tid = *(int*)threadId;
    printf("Hello World %d of %d\n", tid, N);
}

int main(){
    pthread_t* t;
    t = malloc(sizeof(pthread_t) * N);

    for(int i = 0; i < N; i++){
        pthread_create(&t[i], NULL, hello, (void*)&i);
    }
    for(int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    free(t);
    return 0;
}
```

### compile

```bash
file=pth3
gcc $file.c -o $file.out -lpthread
```

### run

```bash
file=pth3
./$file.out
```

**Output:**

```text
Hello World 3 of 20
Hello World 3 of 20
Hello World 6 of 20
Hello World 3 of 20
Hello World 4 of 20
Hello World 8 of 20
Hello World 8 of 20
Hello World 9 of 20
Hello World 10 of 20
Hello World 11 of 20
Hello World 12 of 20
Hello World 13 of 20
Hello World 14 of 20
Hello World 15 of 20
Hello World 16 of 20
Hello World 16 of 20
Hello World 17 of 20
Hello World 19 of 20
Hello World 19 of 20
Hello World 20 of 20
```

## Program to check address of each iteration

```c
#include<stdio.h>
int main(){
    for(int i = 0; i < 20; i++){
        printf("%d is located at %p\n", i, &i);
    }
    return 0;
}
```

**Output:**

```text
0 is located at 0x7ffe70df1414
1 is located at 0x7ffe70df1414
2 is located at 0x7ffe70df1414
3 is located at 0x7ffe70df1414
4 is located at 0x7ffe70df1414
5 is located at 0x7ffe70df1414
6 is located at 0x7ffe70df1414
7 is located at 0x7ffe70df1414
8 is located at 0x7ffe70df1414
9 is located at 0x7ffe70df1414
10 is located at 0x7ffe70df1414
11 is located at 0x7ffe70df1414
12 is located at 0x7ffe70df1414
13 is located at 0x7ffe70df1414
14 is located at 0x7ffe70df1414
15 is located at 0x7ffe70df1414
16 is located at 0x7ffe70df1414
17 is located at 0x7ffe70df1414
18 is located at 0x7ffe70df1414
19 is located at 0x7ffe70df1414
```

## Create array of size N using N threads

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 20

int* arr;
void* hello(void* threadId){
    int tid = *(int*)threadId;
    arr[tid] = tid;
    free(threadId);
}
int main(){
    arr = malloc(sizeof(int) * N);
    pthread_t* t;
    t = malloc(sizeof(pthread_t) * N);
    for(int i = 0; i < N; i++){
        int* a;
        a = malloc(sizeof(int));
        *a = i;
        pthread_create(&t[i], NULL, hello, (void*)a);
    }
    for(int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    free(t);
    for(int i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
```

**Output:**

```text
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
```

## Sum of elements of an array of size N using N threads

In below code you'll experience race condition.

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 20000

int sum = 0;
void* hello(void* threadId){
    int tid = *(int*)threadId;
    sum+= tid;
    free(threadId);
}

int main(){
    pthread_t* t;
    t = malloc(sizeof(pthread_t) * N);
    for(int i = 0; i < N; i++){
        int* a;
        a = malloc(sizeof(int));
        *a = i;
        pthread_create(&t[i], NULL, hello, (void*)a);
    }
    for(int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    free(t);

    printf("Sum = %d\n", sum);
    if(sum + N == (N * (N + 1) / 2)){
        printf("_____Passed_____\n");
    }
    else{
        printf("_____Failed_____\n");
    }
    return 0;
}
```

```bash
gcc sumWithRaceCondition.c -o sumWithRaceCondition.out -lpthread
```

```bash
./sumWithRaceCondition.out
```

**Output:**

```text
Sum = 199980977
_____Failed_____
```

```bash
./sumWithRaceCondition.out
```

**Output:**

```text
Sum = 199990000
_____Passed_____
```

```bash
./sumWithRaceCondition.out
```

**Output:**

```text
Sum = 199976322
_____Failed_____
```

## Mutex: Sum of elements of an array of size N using N threads

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 10000

pthread_mutex_t mutex;

int sum = 0;
void* hello(void* threadId){
    int tid = *(int*)threadId;

    pthread_mutex_lock(&mutex);
    sum+= tid;
    pthread_mutex_unlock(&mutex);

    free(threadId);
}
int main(){
    pthread_t* t;
    pthread_mutex_init(&mutex, NULL);
    t = malloc(sizeof(pthread_t) * N);
    for(int i = 0; i < N; i++){
        int* a;
        a = malloc(sizeof(int));
        *a = i;
        pthread_create(&t[i], NULL, hello, (void*)a);
    }
    for(int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    free(t);

    pthread_mutex_destroy(&mutex);
    printf("Sum = %d\n", sum);
    if(sum + N == (N * (N + 1) / 2)){
        printf("_____Passed_____\n");
    }
    else{
        printf("_____Failed_____\n");
    }
    return 0;
}
```

**Output:**

```text
Sum = 49995000
_____Passed_____
```

---
[← Day 1 — Pthreads: Creating Threads](../../pthreads/day01-thread-basics/) · [Index](../../README.md) · [Day 3 — Pthreads: Reduction, Condition Variables, Barriers](../../pthreads/day03-reduction-condvars-barriers/)
