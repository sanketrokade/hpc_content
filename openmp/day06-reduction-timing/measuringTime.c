#include<stdio.h>
#include<omp.h>
#define N 1000000000
#define T 13

int main(){
    long long sum = 0;
    double parallelTime, serialTime;
    double startTime = omp_get_wtime();
    #pragma omp parallel for reduction(+ : sum) num_threads(T)
    for(int i = 0; i < N; i++){
        sum += i + 1;
    }
    double endTime = omp_get_wtime();
    parallelTime = endTime - startTime;

    startTime = omp_get_wtime();
    long long serialsum = 0;
    for(int i = 0; i < N; i++){
        serialsum += i + 1;
    }
    endTime = omp_get_wtime();
    serialTime = endTime - startTime;
    printf("Calculated sum = %lld\n", sum);
    printf("Time taken by parallel = %lf\n", parallelTime);
    printf("Time taken by serial = %lf\n", serialTime);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");

    return 0;
}
