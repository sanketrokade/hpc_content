#include<stdio.h>
#include<omp.h>
#define N 1000000
#define T 13
int main(){
    long long sum = 0;
    long long a[N];
    #pragma omp parallel  num_threads(T)
    {
        #pragma omp for
        for(int i = 0; i < N; i++){
            a[i] = i + 1;
        }
        #pragma omp for reduction( + : sum )
        for(int i = 0; i < N; i++){
            sum += a[i];
        }
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
