#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 1000000
#define T 10
int main(){
    long long *a;
    long long sum = 0, sumSquare = 0;
    a = (long long*) malloc(sizeof(long long) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
    }
    double startTime = omp_get_wtime();
    #pragma omp parallel num_threads(T)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for(int i = 0; i < N; i++){
                    sum+= a[i];
                }
            }
            #pragma omp section
            {
                for(int i = 0; i < N; i++){
                    sumSquare += a[i] * a[i];
                }
            }
        }
    }
    double endTime = omp_get_wtime();
    printf("Sum = %lld\n", sum);
    printf("Sum of Squares = %lld\n", sumSquare);
    printf("Execution time = %lf\n", endTime - startTime);
    free(a);
}
