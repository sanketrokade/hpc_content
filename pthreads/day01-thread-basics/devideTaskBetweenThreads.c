#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define N 20

void* task1(){
    printf("starting task1\n");
    sleep(2);
    printf("ending task1\n");
}

void* task2(){
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
