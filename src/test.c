// #include "util.h"
// #include <stdio.h>
// #include <stdbool.h>
// #include <immintrin.h>
// #include "bounding_box.h"
// #include "ray.h"
// #include "vector.h"

// bool bbounding_box_hit3(bounding_box *box, ray *r, float t_min, float t_max) {
//     float t0, t1;
//     float bxmin = (box->minimum.x - r->origin.x) / r->direction.x;
//     float bxmax = (box->maximum.x - r->origin.x) / r->direction.x;
//     float bymin = (box->minimum.y - r->origin.y) / r->direction.y;
//     float bymax = (box->maximum.y - r->origin.y) / r->direction.y;
//     float bzmin = (box->minimum.z - r->origin.z) / r->direction.z;
//     float bzmax = (box->maximum.z - r->origin.z) / r->direction.z;

//     int bxn = (*(unsigned int*)&r->direction.x) >> 31;
//     int byn = (*(unsigned int*)&r->direction.y) >> 31;
//     int bzn = (*(unsigned int*)&r->direction.z) >> 31;

//     t0 = bxmax * bxn + bxmin * !bxn;
//     t1 = bxmin * bxn + bxmax * !bxn;

//     printf("tmin tmax: %f %f\n", t_min, t_max);

//     printf("t0x t1x: %f %f\n", t0, t1);

//     if (t0 > t_min)
//         t_min = t0;
//     if (t1 < t_max)
//         t_max = t1;

//     t0 = bymax * byn + bymin * !byn;
//     t1 = bymin * byn + bymax * !byn;

//     printf("t0y t1y: %f %f\n", t0, t1);

//     if (t0 > t_min)
//         t_min = t0;
//     if (t1 < t_max)
//         t_max = t1;

//     t0 = bzmax * bzn + bzmin * !bzn;
//     t1 = bzmin * bzn + bzmax * !bzn;

//     printf("t0z t1z: %f %f\n", t0, t1);

//     if (t0 > t_min)
//         t_min = t0;
//     if (t1 < t_max)
//         t_max = t1;

//     printf("final min and max: %f %f\n", t_min, t_max);

//     return t_min < t_max;
// }

// bool bbounding_box_hit_avx(bounding_box *box, ray *r, float t_min, float t_max) {
//     __m256 _box, _swapbox, _origin, _direction;

//     _box = _mm256_set_ps(box->minimum.x, box->minimum.y, box->minimum.z, t_min, box->maximum.x, box->maximum.y, box->maximum.z, t_max);
//     _swapbox = _mm256_permutevar8x32_ps(_box, _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4));

//     _origin = _mm256_set_ps(r->origin.x, r->origin.y, r->origin.z, 0, r->origin.x, r->origin.y, r->origin.z, 0);
//     _direction = _mm256_set_ps(r->direction.x, r->direction.y, r->direction.z, 1, r->direction.x, r->direction.y, r->direction.z, 1);

//     _box = _mm256_blendv_ps(_box, _swapbox, _mm256_cmp_ps(_direction, _mm256_setzero_ps(), 1));

//     __m256 _delta = _mm256_sub_ps(_box, _origin);
//     __m256 _t = _mm256_div_ps(_delta, _direction);

//     printf("%f %f %f %f %f %f %f %f\n", _t[0], _t[1], _t[2], _t[3], _t[4], _t[5], _t[6], _t[7]);

//     __m256 _tswap= _mm256_permute_ps(_t, _MM_SHUFFLE(2, 3, 0, 1));
//     printf("%d\n", _MM_SHUFFLE(2, 3, 0, 1));

//     __m256 _max = _mm256_max_ps(_t, _tswap);
//     __m256 _min = _mm256_min_ps(_t, _tswap);


//     __m256 _maxswap = _mm256_permute_ps(_max, _MM_SHUFFLE(1, 0, 3, 2));
//     __m256 _minswap = _mm256_permute_ps(_min, _MM_SHUFFLE(1, 0, 3, 2));
//     printf("%d\n", _MM_SHUFFLE(1, 0, 3, 2));


//     _max = _mm256_max_ps(_max, _maxswap);
//     _min = _mm256_min_ps(_min, _minswap);

//     printf("%f %f %f %f %f %f %f %f\n", _min[0], _min[1], _min[2], _min[3], _min[4], _min[5], _min[6], _min[7]);
//     printf("%f %f %f %f %f %f %f %f\n", _max[0], _max[1], _max[2], _max[3], _max[4], _max[5], _max[6], _max[7]);


//     //printf("%f %f\n", _max[0], _min[7]);
//     return _max[7] < _min[0];
// }

// int main() {
//     bounding_box box_data = bounding_box_create(vector_create(2, 3, 4), vector_create(5, 6, 7));
//     ray ray_data = ray_create(vector_create(-1, 0, 1), vector_create(-2, 1, 3), 1);

//     bounding_box* box = &box_data;
//     ray* r = &ray_data;

//     printf("box hit 3: %d\n", bbounding_box_hit3(box, r, 0, 1));
//     printf("box hit avx: %d\n", bbounding_box_hit_avx(box, r, 0, 1));

// };
// // 