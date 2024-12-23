#include<stdio.h>
#include<omp.h>
#define N 10000
#define T 10
int main(){
    #pragma omp parallel num_threads(3)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("This section is executed by thread %d\n", omp_get_thread_num());
            }
        }
    }
}
