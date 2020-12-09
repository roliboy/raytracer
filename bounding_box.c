#include "bounding_box.h"

#include "vector.h"
#include "ray.h"
#include <math.h>
#include <immintrin.h>

bounding_box bounding_box_create(vector minimum, vector maximum) {
    return (bounding_box) {
        .minimum = minimum,
        .maximum = maximum
    };
}


bool bounding_box_hit(bounding_box* box, ray* r, float t_min, float t_max) {
    float t0, t1;
    t0 = fmin((box->minimum.x - r->origin.x) / r->direction.x, (box->maximum.x - r->origin.x) / r->direction.x);
    t1 = fmax((box->minimum.x - r->origin.x) / r->direction.x, (box->maximum.x - r->origin.x) / r->direction.x);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    t0 = fmin((box->minimum.y - r->origin.y) / r->direction.y, (box->maximum.y - r->origin.y) / r->direction.y);
    t1 = fmax((box->minimum.y - r->origin.y) / r->direction.y, (box->maximum.y - r->origin.y) / r->direction.y);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    t0 = fmin((box->minimum.z - r->origin.z) / r->direction.z, (box->maximum.z - r->origin.z) / r->direction.z);
    t1 = fmax((box->minimum.z - r->origin.z) / r->direction.z, (box->maximum.z - r->origin.z) / r->direction.z);
    t_min = fmax(t0, t_min);
    t_max = fmin(t1, t_max);
    if (t_max <= t_min)
        return false;

    return true;
}

bool bounding_box_hit2(bounding_box *box, ray *r, float t_min, float t_max) {
    float inv, t0, t1;
    inv = 1.0 / r->direction.x;

    t0 = (box->minimum.x - r->origin.x) * inv;
    t1 = (box->maximum.x - r->origin.x) * inv;
    //TODO: branchless swap
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;


    inv = 1.0 / r->direction.y;
    t0 = (box->minimum.y - r->origin.y) * inv;
    t1 = (box->maximum.y - r->origin.y) * inv;
    //TODO: branchless swapp
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;


    inv = 1.0 / r->direction.z;
    t0 = (box->minimum.z - r->origin.z) * inv;
    t1 = (box->maximum.z - r->origin.z) * inv;
    //TODO: branchless swapp
    if (inv < 0) {
        float a = t0;
        t0 = t1;
        t1 = a;
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max <= t_min)
        return false;

    return true;
}

bool bounding_box_hit3(bounding_box *box, ray *r, float t_min, float t_max) {
    float t0, t1;
    float bxmin = (box->minimum.x - r->origin.x) / r->direction.x;
    float bxmax = (box->maximum.x - r->origin.x) / r->direction.x;
    float bymin = (box->minimum.y - r->origin.y) / r->direction.y;
    float bymax = (box->maximum.y - r->origin.y) / r->direction.y;
    float bzmin = (box->minimum.z - r->origin.z) / r->direction.z;
    float bzmax = (box->maximum.z - r->origin.z) / r->direction.z;

    int bxn = (*(unsigned int*)&r->direction.x) >> 31;
    int byn = (*(unsigned int*)&r->direction.y) >> 31;
    int bzn = (*(unsigned int*)&r->direction.z) >> 31;

    t0 = bxmax * bxn + bxmin * !bxn;
    t1 = bxmin * bxn + bxmax * !bxn;

    if (t0 > t_min)
        t_min = t0;
    if (t1 < t_max)
        t_max = t1;

    t0 = bymax * byn + bymin * !byn;
    t1 = bymin * byn + bymax * !byn;

    if (t0 > t_min)
        t_min = t0;
    if (t1 < t_max)
        t_max = t1;

    t0 = bzmax * bzn + bzmin * !bzn;
    t1 = bzmin * bzn + bzmax * !bzn;

    if (t0 > t_min)
        t_min = t0;
    if (t1 < t_max)
        t_max = t1;

    return t_min < t_max;
}

bool bounding_box_hit_avx(bounding_box *box, ray *r, float t_min, float t_max) {
    __m256 _box, _swapbox, _origin, _direction;

    _box = _mm256_set_ps(box->minimum.x, box->minimum.y, box->minimum.z, t_min, box->maximum.x, box->maximum.y, box->maximum.z, t_max);
    _swapbox = _mm256_permutevar8x32_ps(_box, _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4));

    _origin = _mm256_set_ps(r->origin.x, r->origin.y, r->origin.z, 0, r->origin.x, r->origin.y, r->origin.z, 0);
    _direction = _mm256_set_ps(r->direction.x, r->direction.y, r->direction.z, 1, r->direction.x, r->direction.y, r->direction.z, 1);

    _box = _mm256_blendv_ps(_box, _swapbox, _mm256_cmp_ps(_direction, _mm256_setzero_ps(), 1));

    __m256 _delta = _mm256_sub_ps(_box, _origin);
    __m256 _t = _mm256_div_ps(_delta, _direction);

//    __m256 _tswap= _mm256_permute_ps(_t, _MM_SHUFFLE(2, 3, 0, 1));
    __m256 _tswap= _mm256_permute_ps(_t, 177);

    __m256 _max = _mm256_max_ps(_t, _tswap);
    __m256 _min = _mm256_min_ps(_t, _tswap);


    //__m256 _maxswap = _mm256_permute_ps(_max, _MM_SHUFFLE(1, 0, 3, 2));
    //__m256 _minswap = _mm256_permute_ps(_min, _MM_SHUFFLE(1, 0, 3, 2));
    __m256 _maxswap = _mm256_permute_ps(_max, 78);
    __m256 _minswap = _mm256_permute_ps(_min, 78);


    _max = _mm256_max_ps(_max, _maxswap);
    _min = _mm256_min_ps(_min, _minswap);


    //printf("%f %f\n", _max[0], _min[7]);
    return _max[7] < _min[0];
}

bool bounding_box_hit_avx2(bounding_box *box, ray *r, float t_min, float t_max) {
    __m256 _direction = _mm256_set_ps(r->direction.x, r->direction.y, r->direction.z, 1, r->direction.x, r->direction.y, r->direction.z, 1);

    __m256 _box = _mm256_blendv_ps(
            _mm256_set_ps(box->minimum.x, box->minimum.y, box->minimum.z, t_min, box->maximum.x, box->maximum.y, box->maximum.z, t_max),
            _mm256_set_ps(box->maximum.x, box->maximum.y, box->maximum.z, t_max, box->minimum.x, box->minimum.y, box->minimum.z, t_min),
            _mm256_cmp_ps(_direction, _mm256_setzero_ps(), 1));

    __m256 _t = _mm256_div_ps(
            _mm256_sub_ps(
                _box,
                _mm256_set_ps(r->origin.x, r->origin.y, r->origin.z, 0, r->origin.x, r->origin.y, r->origin.z, 0)),
            _direction);

    __m256 _max = _mm256_max_ps(_t, _mm256_permute_ps(_t, 177));
    __m256 _min = _mm256_min_ps(_t, _mm256_permute_ps(_t, 177));

    _max = _mm256_max_ps(_max, _mm256_permute_ps(_max, 78));
    _min = _mm256_min_ps(_min, _mm256_permute_ps(_min, 78));

    return _max[7] < _min[0];
}

bounding_box bounding_box_surround(bounding_box box0, bounding_box box1) {
    vector small = vector_create(
            fmin(box0.minimum.x, box1.minimum.x),
            fmin(box0.minimum.y, box1.minimum.y),
            fmin(box0.minimum.z, box1.minimum.z));

    vector large = vector_create(
            fmax(box0.maximum.x, box1.maximum.x),
            fmax(box0.maximum.y, box1.maximum.y),
            fmax(box0.maximum.z, box1.maximum.z));

    return (bounding_box) {
        .minimum = small,
        .maximum = large
    };
}
