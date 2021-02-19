#include "util.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "allocator.h"

float random_float(float min, float max) {
  return min + (max - min) * ((rand() + 1.0) / RAND_MAX);
}

int random_int(int min, int max) { return min + rand() % (max - min + 1); }

float clamp(float x, float min, float max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

float reflectance(float cosine, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

unsigned char *load_ppm(char *filename, unsigned int *width,
                        unsigned int *height) {
  FILE *fp;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  fscanf(fp, "%*s\n");
  fscanf(fp, "%u %u\n", width, height);
  // printf("w: %d\nh: %d\n", *width, *height);

  fscanf(fp, "%*d");

  unsigned char *result =
      (unsigned char *)allocate(sizeof(unsigned char) * 3 * *width * *height);

  for (int i = 0; i < *width * *height * 3; i += 3) {
    fscanf(fp, "%hhu %hhu %hhu", &result[i + 0], &result[i + 1],
           &result[i + 2]);
  }

  fclose(fp);

  return result;
}
