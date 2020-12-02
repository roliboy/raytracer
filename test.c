#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "object.h"

int main() {
    object* objects = (object*)malloc(10 * sizeof(object));
    printf("%ld\n", sizeof(objects));
    int c = 0;
    for (int i = -11; i < 11; i++) {
    for (int j = -11; j < 11; j++) {
        c++;
    }
    }
    printf("%d\n", c);
}
