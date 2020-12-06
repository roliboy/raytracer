#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "scene.h"
#include "object.h"

int bounding_box_xk_compare(const void* a, const void* b) {
    bounding_box box_a;
    bounding_box box_b;

//    object* ap = *(object**)a;
//    object* bp = *(object**)b;

    printf("%.1f - ", ((object*)a)->data.sphere.center.x);
    printf("%.1f\n", ((object*)b)->data.sphere.center.x);

//    printf("%d %f - ", ((object*)ap)->id, ((object*)ap)->data.sphere.radius);
//    printf("%d %f\n", ((object*)bp)->id, ((object*)bp)->data.sphere.radius);

    if (!object_bounding_box((object*)a, 0, 0, &box_a) || !object_bounding_box((object*)b, 0, 0, &box_b)) {
        puts("reeex");
        exit(-1);
    }

    //printf("%.2f - %.2f\n", box_a.minimum.x, box_b.minimum.x);

    return (box_a.minimum.x < box_b.minimum.x)? -1 : 1;
}

int main() {
    scene scn = scene_load("scenes/test.scn");
    //printf("%d\n", scn.size);
    //object n = node_create(&scn, 0, scn.size, 0, 1);

    for (int i = 0; i < scn.size; i++) {
        printf("%d ", scn.objects[i].id);
        printf("%.1f\n", scn.objects[i].data.sphere.center.x);
    }
    puts("");

    printf("@%p %p@\n", scn.objects + sizeof(scn.objects[0]), &scn.objects[1]);
    object* st = &scn.objects[1];
//    qsort(scn.objects + 1 * sizeof(object), scn.size - 1, sizeof(object), bounding_box_xk_compare);
    qsort(st, scn.size - 2, sizeof(object), bounding_box_xk_compare);

    puts("");

    for (int i = 0; i < scn.size; i++) {
        printf("%d ", scn.objects[i].id);
        printf("%.1f\n", scn.objects[i].data.sphere.center.x);
    }
    puts("");

    scene_destroy(&scn);
}
