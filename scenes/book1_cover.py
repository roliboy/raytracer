#!/usr/bin/env python

from random import random, uniform
from scenekit import *

size(384, 216)
ray(8, 8)
camera(13, 2, 3)
scene(0, 0, 0)
up(0, 1, 0)
settings(20, 0.1, 10, 0.0, 1.0)
ambient(0.7, 0.8, 1)

makeobject(sphere(0.0, -1000.0, 0.0, 1000.0), makematerial(diffuse(maketexture(solid_color(0.5, 0.5, 0.5)))))
makeobject(sphere(0.0, 1.0, 0.0, 1.0), makematerial(dielectric(1.5)))
makeobject(sphere(-4.0, 1.0, 0.0, 1.0), makematerial(diffuse(maketexture(solid_color(0.4, 0.2, 0.1)))))
makeobject(sphere(4.0, 1.0, 0.0, 1.0), makematerial(metal(0.7, 0.6, 0.5, 0.0)))

for a in range(-20, 20):
    for b in range(-20, 20):
        x = a + 0.75 * random()
        y = 0.2
        z = b + 0.75 * random()

        if ((x - 0)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 2:
            continue
        if ((x + 4)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 2:
            continue
        if ((x - 4)**2 + (y - 1)**2 + (z - 0)**2)**0.5 < 2:
            continue
        material_choice = random()
        material = None
        if material_choice < 0.8:
            r = random() * random()
            g = random() * random()
            b = random() * random()
            texture = maketexture(solid_color(r, g, b))
            material = makematerial(diffuse(texture))
        elif material_choice < 0.95:
            r = uniform(0.5, 1)
            g = uniform(0.5, 1)
            b = uniform(0.5, 1)
            f = uniform(0, 0.5)
            material = makematerial(metal(r, g, b, f))
        else:
            material = makematerial(dielectric(1.5))
        makeobject(sphere(x, y, z, 0.2), material)

printscene()