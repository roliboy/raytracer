#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "scene.h"
#include "scenes/book1.h"

int main() {
    scene scn = scene_book1();
//    scene scn = scene_load("scenes/book1_cover.scn");
    scene_destroy(&scn);
    //printf("%d\n", c);
}
