#include<stdio.h>
int main(){
    for(int i = 0; i < 20; i++){
        printf("%d is located at %p\n", i, &i);
    }
    return 0;
}
