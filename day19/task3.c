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
            #pragma omp taskgroup
            {
                for (int i = 0; i < 5; i++) {
                    #pragma omp task
                    work(i);
                }
            }
            printf("All tasks in the group are completed.\n");
        }
    }
    return 0;
}
