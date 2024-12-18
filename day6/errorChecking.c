#include<stdio.h>
#ifdef _OPENMP
#include<omp.h>
#endif
#include<stdlib.h>
#define N 100000000
#define T 13

int main(){
    int *a, *b, *c, *cs;
    a = (int *) malloc(sizeof(int) * N);
    b = (int *) malloc(sizeof(int) * N);
    c = (int *) malloc(sizeof(int) * N);
    cs = (int *) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
        b[i] = i + 1;
        c[i] = 0;
        cs[i] = 0;
    }

    omp_set_num_threads(T);

    #pragma omp parallel for
    for(int i = 0; i < N; i++){
        c[i] = a[i] + b[i];
    }

    for(int i = N - 5; i < N; i++){
        printf("%d ", c[i]);
    }

    free(a);
    free(b);
    free(c);
    free(cs);

    return 0;
}
