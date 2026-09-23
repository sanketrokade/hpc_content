#include <stdio.h>
#include <omp.h>

void work(int id) {
    printf("Task %d is being executed by thread %d\n", id, omp_get_thread_num());
}


int main() {
    #pragma omp parallel num_threads(5)
    {
        #pragma omp single
        {
            printf("%d is creating the task\n", omp_get_thread_num());
            for (int i = 0; i < 5; i++) {
                #pragma omp task
                work(i);
            }
        }
    }
    return 0;
}
