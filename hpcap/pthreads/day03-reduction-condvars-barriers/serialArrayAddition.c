#include<stdio.h>
#include<stdlib.h>
#define N 100000
int main(){
    int* a, *b, *c;
    a = (int*) malloc(sizeof(int) * N);
    b = (int*) malloc(sizeof(int) * N);
    c = (int*) malloc(sizeof(int) * N);
    for(int i = 0; i < N; i++){
        a[i] = i + 1;
        b[i] = i + 1;
        c[i] = 0;
    }

    for(int i = 0; i < N; i++){
        c[i] = a[i] + b[i];
    }
    for(int i = 0; i < N; i++){
        printf("%d ", c[i]);
    }
    return 0;
}
