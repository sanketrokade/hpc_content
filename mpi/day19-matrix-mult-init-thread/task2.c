#include <stdio.h>
#include <omp.h>

void work(int id) {
    printf("Task %d is being executed by thread %d\n", id, omp_get_thread_num());
}

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < 5; i++) {
                #pragma omp task
                work(i);
            }
            #pragma omp taskwait
            printf("All tasks are completed.\n");
        }
    }
    return 0;
}
