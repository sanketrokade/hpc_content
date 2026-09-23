#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define T 7

void* hello(){
    long* localValue = malloc(sizeof(long));
    *localValue = 123;
    return (void*)*localValue;
}

int main(){
    pthread_t* t;
    t = malloc(sizeof(pthread_t) * T);

    long localValue;
    for(long i = 0; i < T; i++)
        pthread_create(&t[i], NULL, hello, NULL);
    for(long i = 0; i < T; i++){
        pthread_join(t[i], (void**)&localValue);
        printf("Local Value from each thread = %ld\n", localValue);
    }
    free(t);
    return 0;
}
