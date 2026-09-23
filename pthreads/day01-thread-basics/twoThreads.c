#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void* task1(){
    printf("starting task1\n");
    sleep(10);
    printf("ending task1\n");
}

void* task2(){
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
