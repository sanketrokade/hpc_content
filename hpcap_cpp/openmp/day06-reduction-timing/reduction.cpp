#include<stdio.h>
#include<omp.h>
#define N 1000000000
#define T 13
int main(){
    long long sum = 0;
    #pragma omp parallel for reduction(+ : sum) num_threads(T)
    for(int i = 0; i < N; i++){
        sum += i + 1;
    }

    printf("Calculated sum = %lld\n", sum);
    long long expectedSum = (N * ((N + 1) * 1L) / 2);
    printf("Expected sum = %lld\n", expectedSum);
    if(sum == expectedSum){
        printf("_____Passed_____\n");
    }
    else printf("_____Failed_____\n");

    return 0;
}
