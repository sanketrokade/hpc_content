#include<stdio.h>
#include<omp.h>
int main(){
    #pragma omp parallel num_threads(2)
    {
        printf("Level 1 : Id %d\n", omp_get_thread_num());
        #pragma omp parallel num_threads(2)
        {
            printf("Level 2 : Id %d\n", omp_get_thread_num());
        }
    }
}
