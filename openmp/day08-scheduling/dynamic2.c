#include<stdio.h>
#include<omp.h>
#define N 10
#define T 5
int main(){
    int a[N] = {1343, 100, 500000, 322, 4444, 544, 300, 70000000, 400, 3244};
    #pragma omp parallel for schedule(dynamic, 1) num_threads(T)
    for(int i = 0; i < N; i++){
        printf("iteration i = %d is assigned to %d\n", i, omp_get_thread_num());
        for(int j = 0; j < a[i]; j++);
    }
}
