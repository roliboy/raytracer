#include "noise.h"
#include "vector.h"
#include <math.h>

noise noise_create() {
    noise n;
    for (int i = 0; i < 256; i++) {
        n.rnf[i] = vector_random_in_range(-1, 1);
        n.px[i] = i;
        n.py[i] = i;
        n.pz[i] = i;
    }

    for (int i = 256 - 1; i > 0; i--) {
        int targetx = random_int(0, i);
        int targety = random_int(0, i);
        int targetz = random_int(0, i);
        int tmp = n.px[i];
        n.px[i] = n.px[targetx];
        n.px[targetx] = tmp;
        tmp = n.py[i];
        n.py[i] = n.py[targety];
        n.py[targety] = tmp;
        tmp = n.pz[i];
        n.pz[i] = n.pz[targetz];
        n.pz[targetz] = tmp;
    }

    return n;
}

float noise_get_value(noise* n, vector p) {
    float u = p.x - floor(p.x);
    float v = p.y - floor(p.y);
    float w = p.z - floor(p.z);

    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);

    int i = floor(p.x);
    int j = floor(p.y);
    int k = floor(p.z);

    vector c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = n->rnf[
                    n->px[(i + di) & 255] ^
                    n->py[(j + dj) & 255] ^
                    n->pz[(k + dk) & 255]
                ];

    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                vector weight_v = vector_create(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                       * (j * vv + (1 - j) * (1 - vv))
                       * (k * ww + (1 - k) * (1 - ww))
                       * vector_dot(c[i][j][k], weight_v);
            }

    return accum;
}

float noise_get_turbulent(noise* n, vector p, int depth) {
    float accum = 0.0;
    vector temp_p = p;
    float weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise_get_value(n, temp_p);
        weight *= 0.5;
        temp_p = vector_multiply_scalar(temp_p, 2);
    }

    return fabs(accum);
}

