#include<stdio.h>
#include<omp.h>         //for openmp support
int main(){
    #pragma omp parallel num_threads(6)
    {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }

    return 0;
}
