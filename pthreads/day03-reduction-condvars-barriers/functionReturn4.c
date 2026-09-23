#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define T 7

void* hello(){
    int* a;
    a = (int*) malloc(sizeof(int));
    *a = 5;
    return (void*)a;
}

int main(){
    pthread_t* t;
    t = malloc(sizeof(pthread_t) * T);

    int* b;
    for(long i = 0; i < T; i++)
        pthread_create(&t[i], NULL, hello, NULL);
    for(long i = 0; i < T; i++){
        pthread_join(t[i], (void**)&b);
        printf("Local Value from each thread = %d\n", *b);
    }
    free(t);
    return 0;
}
