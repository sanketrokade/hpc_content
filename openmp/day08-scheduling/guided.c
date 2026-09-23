#include<stdio.h>
#include<omp.h>
#define N 20
#define T 3
int main(){
    #pragma omp parallel for schedule(guided, 3) num_threads(T)
    for(int i = 0; i < N; i++){
        printf("thread\t%d\t:\ti\t%d\n", omp_get_thread_num(), i);
    }
}
