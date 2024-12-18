#include<stdio.h>
#include<omp.h>
#define N 10000
#define T 13
int main(){
    int chunksize = N / T;
    int sum[T];
    #pragma omp parallel num_threads(T)
    {
        int tid = omp_get_thread_num();
        int localsum = 0;
        int start = tid * chunksize;
        int end = start + chunksize;
        if(tid == T - 1) end = N;
        for(int i = start; i < end; i++){
            localsum += i + 1;
        }
        sum[tid] = localsum;
    }

    int totalSum = 0;
    for(int i = 0; i < T; i++) totalSum += sum[i];

    printf("Total sum = %d\n", totalSum);

    return 0;
}
