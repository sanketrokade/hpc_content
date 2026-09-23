#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    const int n = 400;
    int i, j;
    int m1[n][n], m2[n][n], sumMat[n][n];
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            m1[i][j] = 1;
            m2[i][j] = 1;
        }
    }
    /*
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%d ",m1[i][j]);
        }
        printf("\n");
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%d ",m2[i][j]);
        }
        printf("\n");
    }*/

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            sumMat[i][j] = m1[i][j] + m2[i][j];
        }
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%d ",sumMat[i][j]);
        }
        printf("\n");
    }

    return 0;
}
