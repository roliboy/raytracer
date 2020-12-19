#include "rectangle.h"

#include "../ray.h"
#include "../hit.h"
#include "../object.h"
#include "../vector.h"
#include "../material.h"
#include "../bounding_box.h"
#include <math.h>
#include <stdio.h>

bool rectangle_hit(rectangle* rect, ray* r, float t_min, float t_max, hit* record) {
    float t = (rect->k - r->origin.z) / r->direction.z;

    if (t < t_min || t > t_max)
        return false;

    float x = r->origin.x + t * r->direction.x;
    float y = r->origin.y + t * r->direction.y;

    if (x < rect->x0 || x > rect->x1 || y < rect->y0 || y > rect->y1)
        return false;

    record->u = (x - rect->x0) / (rect->x1 - rect->x0);
    record->v = (y - rect->y0) / (rect->y1 - rect->y0);
    record->t = t;

    vector outward_normal = vector_create(0, 0, 1);

    hit_set_face_normal(record, r, outward_normal);
    record->mat = rect->mat;
    record->p = ray_at(r, t);

    return true;
}

bool rectangle_bounding_box(rectangle* rect, float time0, float time1, bounding_box* box) {
    *box = bounding_box_create(
            vector_create(rect->x0, rect->y0, rect->k - 0.0001),
            vector_create(rect->x1, rect->y1, rect->k + 0.0001));
    return true;
}

object rectangle_create(float x0, float y0, float x1, float y1, float k, material* mat) {
    return (object) {
        .id = object_rectangle,
        .data = (object_data) {
            .rectangle = (rectangle) {
                .x0 = x0,
                .y0 = y0,
                .x1 = x1,
                .y1 = y1,
                .k = k,
                .mat = mat
            }
        }
    };
}
