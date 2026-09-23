#include<stdio.h>
#include<omp.h>
#define N 20
#define T 6
int main(){
    #pragma omp parallel for schedule(runtime) num_threads(T)
    for(int i = 0; i < N; i++){
        printf("thread\t%d\t:\ti\t%d\n", omp_get_thread_num(), i);
    }
}
