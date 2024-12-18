#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define N 100000000
#define T 13

int main(){
    int *a, *b, *c, *cs;
    a = (int *) malloc(sizeof(int) * N);
    b = (int *) malloc(sizeof(int) * N);
    c = (int *) malloc(sizeof(int) * N);
    cs = (int *) malloc(sizeof(int) * N);
    //Data creation
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
        b[i] = i + 1;
        c[i] = 0;
        cs[i] = 0;
    }

    //parallel array addition
    double startTime = omp_get_wtime();
    #pragma omp parallel for num_threads(T)
    for(int i = 0; i < N; i++){
        c[i] = a[i] + b[i];
    }
    double endTime = omp_get_wtime();
    double parallelTime = endTime - startTime;

    //Serial array addition
    startTime = omp_get_wtime();
    for(int i = 0; i < N; i++){
        cs[i] = a[i] + b[i];
    }
    endTime = omp_get_wtime();
    double serialTime = endTime - startTime;
    for(int i = N - 5; i < N; i++){
        printf("%d ", c[i]);
    }
    printf("\nSerial time = %lf\n", serialTime);
    printf("\nParallel time = %lf\n", parallelTime);

    return 0;
}
