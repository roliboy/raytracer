#!/usr/bin/env python

from random import random, uniform
from scenekit import *

resolution(384, 216)
detail(8, 8)
lookfrom(13, 2, 3)
lookat(0, 0, 0)
up(0, 1, 0)
camera(20, 0.1, 10, 0.0, 1.0)
ambient(0.7, 0.8, 1)

sphere(0.0, -1000.0, 0.0, 1000.0, diffuse(solid_color(0.5, 0.5, 0.5)))
sphere(0.0, 1.0, 0.0, 1.0, dielectric(1.5))
sphere(-4.0, 1.0, 0.0, 1.0, diffuse(solid_color(0.4, 0.2, 0.1)))
sphere(4.0, 1.0, 0.0, 1.0, metal(0.7, 0.6, 0.5, 0.0))

for a in range(-20, 20):
    for b in range(-20, 20):
        x = a + 0.75 * random()
        y = 0.2
        z = b + 0.75 * random()

        if ((x - 0)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 1.5:
            continue
        if ((x + 4)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 1.5:
            continue
        if ((x - 4)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 1.5:
            continue
        material_choice = random()
        if material_choice < 0.8:
            r = random() * random()
            g = random() * random()
            b = random() * random()
            sphere(x, y, z, 0.2, diffuse(solid_color(r, g, b)))
        elif material_choice < 0.95:
            r = uniform(0.5, 1)
            g = uniform(0.5, 1)
            b = uniform(0.5, 1)
            f = uniform(0, 0.5)
            sphere(x, y, z, 0.2, metal(r, g, b, f))
        else:
            sphere(x, y, z, 0.2, dielectric(1.5))

printscene()
