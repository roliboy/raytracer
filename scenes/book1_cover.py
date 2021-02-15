#!/usr/bin/env python

from random import random, uniform

print('384 216')
print('16 8')
print('13 2 3')
print('0 0 0')
print('0 1 0')
print('20 0.1 10 0.0 1.0')
print('0.7 0.8 1')

sphere = lambda x, y, z, d: f"sphere {x} {y} {z} {d}"
diffuse = lambda r, g, b: f"diffuse {r} {g} {b}"
metal = lambda r, g, b, f: f"metal {r} {g} {b} {f}"
dielectric = lambda i: f"dielectric {i}"

makeobject = lambda obj, mat: print(obj + " " + mat)

makeobject(sphere(0.0, -1000.0, 0.0, 1000.0), diffuse(0.5, 0.5, 0.5))
makeobject(sphere(0.0, 1.0, 0.0, 1.0), dielectric(1.5))
makeobject(sphere(-4.0, 1.0, 0.0, 1.0), diffuse(0.4, 0.2, 0.1))
makeobject(sphere(4.0, 1.0, 0.0, 1.0), metal(0.7, 0.6, 0.5, 0.0))

for a in range(-20, 20):
    for b in range(-20, 20):
        x = a + 0.75 * random()
        y = 0.2
        z = b + 0.75 * random()

        if ((x - 4)**2 + (y - 0.2)**2 + (z - 0)**2)**0.5 < 1:
            continue
        material = random()
        if material < 0.8:
            r = random() * random()
            g = random() * random()
            b = random() * random()
            makeobject(sphere(x, y, z, 0.2), diffuse(r, g, b))
        elif material < 0.95:
            r = uniform(0.5, 1)
            g = uniform(0.5, 1)
            b = uniform(0.5, 1)
            f = uniform(0, 0.5)
            makeobject(sphere(x, y, z, 0.2), metal(r, g, b, f))
        else:
            makeobject(sphere(x, y, z, 0.2), dielectric(1.5))