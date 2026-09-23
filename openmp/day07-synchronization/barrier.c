#include<stdio.h>
#include<unistd.h>
#include<omp.h>
#define T 5
int main(){
    #pragma omp parallel num_threads(T)
    {
        printf("Before barrier\n");
        #pragma omp barrier
        printf("After barrier\n");
    }

    return 0;
}
