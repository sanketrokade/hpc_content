#include<stdio.h>
#include<omp.h>
#define T 5
#define N 20
int main(){
    int a = 5;
    #pragma omp parallel for lastprivate(a) num_threads(T)
    for(int i = 0; i < N; i++){
        if(i > 16){
            continue;
        }
        a = i;
        printf("thread %d is changing a to %d\n", omp_get_thread_num(), a);
    }
    printf("a = %d\n", a);

    return 0;
}
