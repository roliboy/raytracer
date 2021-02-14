#pragma once

#define infinity 0x1.fffffep+127f

float random_float(float min, float max);
int random_int(int min, int max);
float clamp(float x, float min, float max);
float reflectance(float cosine, float ref_idx);
unsigned char* load_ppm(char* filename, unsigned int* width, unsigned int* height);
