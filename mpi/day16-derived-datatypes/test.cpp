#include<iostream>
#include<vector>
using namespace std;
#define N 10
int main(){
    vector<int> v;
    for(int i = 0; i < N; i++){
        v.push_back(i + 1);
    }
    for(int i = 0; i < N; i++){
        printf("%d ", v[i]);
    }
    printf("\n");

}
