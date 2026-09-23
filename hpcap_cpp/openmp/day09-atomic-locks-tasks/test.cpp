#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Task 1 executed by thread %d\n", omp_get_thread_num());
                #pragma omp parallel for
                for (int i = 0; i < 5; i++) {
                    printf("  Task 1: Loop iteration %d executed by thread %d\n", i, omp_get_thread_num());
                }
            }
            #pragma omp section
            {
                printf("Task 2 executed by thread %d\n", omp_get_thread_num());
                #pragma omp parallel for
                for (int i = 0; i < 5; i++) {
                    printf("  Task 2: Loop iteration %d executed by thread %d\n", i, omp_get_thread_num());
                }
            }
        }
    }
    return 0;
}
