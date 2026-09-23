#include<stdio.h>
#include<omp.h>         //for openmp support
void task1(int a, int b, int tid){
    int c = a + b + tid;
    printf("Value of C for thread %d is %d\n", tid, c);
}
int main(){
    int a = 6;
    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();
        int b = 6;
        if(tid == 2) b = 7;
        task1(a, b, tid);
    }
    return 0;
}
