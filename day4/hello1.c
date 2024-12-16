#include<stdio.h>
#include<omp.h>         //for openmp support
int main(){
    omp_set_num_threads(14); //it will create 4 threads
    #pragma omp parallel num_threads(6)
    {
        printf("Hello from first parallel region\n");
    }

    #pragma omp parallel
    {
        printf("Hello from second parallel region\n");
    }
    return 0;
}
