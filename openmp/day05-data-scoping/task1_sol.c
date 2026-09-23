#include<stdio.h>
#include<omp.h>
#define N 100000
#define T 10
int main(){
    int a[N];
    for(int i = 0; i < N; i++) a[i] = i + 1;

    int start, end;
    int chunksize = N / T;
    #pragma omp parallel shared(chunksize) private(start, end) num_threads(T)
    {
        int tid = omp_get_thread_num();
        start = tid * chunksize;
        end = start + chunksize;
        if(tid == T - 1) end = N;
        for(int i = start; i < end; i++){
            printf("%d ", a[i]);
        }
    }

    return 0;
}
