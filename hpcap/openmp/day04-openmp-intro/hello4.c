#include<stdio.h>
#include<omp.h>         //for openmp support

void task1(int tid){
    printf("executing task1 by thread %d\n", tid);
}

void task2(int tid){
    printf("executing task2 by thread %d\n", tid);
}

int main(){
    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();
        if(tid == 0){
            task1(tid);
        }
        else{
            task2(tid);
        }

    }

    return 0;
}
