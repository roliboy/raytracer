#include "hit.h"

#include "ray.h"

void hit_set_face_normal(hit *h, ray *r, vector outward_normal) {
  h->front_face = vector_dot(r->direction, outward_normal) < 0;
  h->n = h->front_face ? outward_normal : vector_invert(outward_normal);
}
