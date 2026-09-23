#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define N 100000
#define T 10

int* a, *b, *c;

void* task(void* threadId){
    int tid = *(int*)threadId;
    int chunksize = N / T;
    int start = tid * chunksize;
    int end = start + chunksize;
    if(tid == T - 1) end = N;

    for(int i = start; i < end; i++){
        a[i] = i + 1;
        b[i] = i + 1;
        c[i] = 0;
    }
    for(int i = start; i < end; i++){
        c[i] = a[i] + b[i];
    }
}

int main(){
    a = (int*) malloc(sizeof(int) * N);
    b = (int*) malloc(sizeof(int) * N);
    c = (int*) malloc(sizeof(int) * N);

    pthread_t t[T];
    for(int i = 0; i < T; i++){
        int* a = (int*) malloc(sizeof(int));
        *a = i;
        pthread_create(&t[i], NULL, task, (void*)a);
    }

    for(int i = 0; i < T; i++){
        pthread_join(t[i], NULL);
    }

    for(int i = 0; i < N; i++){
        printf("%d ", c[i]);
    }
    return 0;
}
