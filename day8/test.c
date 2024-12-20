#include<stdio.h>
#include<omp.h>
#define N 10
#define T 3
int main(){
    #pragma omp parallel for schedule(guided, 1) num_threads(T)
    for(int i = 0; i < N; i++){
        printf("thread\t%d\ti\t%d\n", omp_get_thread_num(), i);
    }
}
