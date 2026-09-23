#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Hello, World : line1\n");
        printf("Hello, World : line2\n");
    }
    return 0;
}
