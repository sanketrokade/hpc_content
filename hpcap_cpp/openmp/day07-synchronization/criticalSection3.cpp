#include<stdio.h>
#include<omp.h>
#define N 1000000000
#define T 13
int main(){
    long long sum = 0;
    #pragma omp parallel num_threads(T)
    {
        int tid = omp_get_thread_num();
        long long localsum = 0;
        #pragma omp for
        for(int i = 0; i < N; i++){
            localsum += i + 1;
        }
        #pragma omp critical
        {
            sum+= localsum;
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
