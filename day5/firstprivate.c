#include<stdio.h>
#include<omp.h>
int main(){
    int a = 5;
    #pragma omp parallel firstprivate(a) num_threads(4)
    {
        printf("Inside: a = %d by tid %d : %p\n", a, omp_get_thread_num(), &a);
    }
    printf("After: a = %d : %p\n", a, &a);
    return 0;
}
