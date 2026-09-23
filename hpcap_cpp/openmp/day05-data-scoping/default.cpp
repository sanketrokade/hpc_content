#include<stdio.h>
#include<omp.h>
int main(){
    int a = 5;
    int b = 234;
    #pragma omp parallel default(none) shared(a) private(b) num_threads(4)
    {
        printf("Inside: a = %d by tid %d : %p\n", a, omp_get_thread_num(), &a);
        b = 234;
    }
    printf("After: a = %d : %p\n", a, &a);
    return 0;
}
