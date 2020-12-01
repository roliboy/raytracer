#include <stdio.h>
#include <unistd.h>

int main() {
    write(1, "hello world\n", 12);
    puts("hello world");
    printf("hello %s\n", "world");
}
