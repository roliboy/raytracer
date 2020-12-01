#include <stdio.h>

int main() {
    #pragma omp parallel for
    for (int i = 1; i <= 10; i++) {
        printf("%d\n", i);
    }
}
