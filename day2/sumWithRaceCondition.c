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
