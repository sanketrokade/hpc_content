#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 100000000
#define T 10
int main(){
    long long  csum = 0, asum = 0;
    double startCritical = omp_get_wtime();
    #pragma omp parallel for num_threads(10)
    for(long long i = 0; i < N; i++){
        #pragma omp critical
        {
            csum+= i + 1;
        }
    }
    double endCritical = omp_get_wtime();

    double startAtomic = omp_get_wtime();
    #pragma omp parallel for num_threads(10)
    for(long long i = 0; i < N; i++){
        #pragma omp atomic
        asum+= i + 1;
    }
    double endAtomic = omp_get_wtime();
    printf("Time taken by critical section : %lf\n", endCritical - startCritical);
    printf("Critical sum : %lld\n", csum);
    printf("Time taken by atomic: %lf\n", endAtomic - startAtomic);
    printf("Atomic sum : %lld\n", asum);

    return 0;
}
