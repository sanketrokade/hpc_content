#include<stdio.h>
#include<omp.h>
int main(){
    int a = 5;
    #pragma omp parallel private(a) num_threads(4)
    {
        printf("Inside: a = %d by tid %d\n", a, omp_get_thread_num());
    }
    printf("After: a = %d\n", a);
    return 0;
}
