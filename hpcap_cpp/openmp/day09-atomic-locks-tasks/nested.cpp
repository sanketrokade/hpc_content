#include<stdio.h>
#include<omp.h>
int main(){
    //omp_set_nested(1); //using this function you can enable desable nested parallelism
    #pragma omp parallel num_threads(2)
    {
        printf("Level 1 : Id %d\n", omp_get_thread_num());
        #pragma omp parallel num_threads(2)
        {
            printf("Level 2 : Id %d\n", omp_get_thread_num());
        }
    }
}
