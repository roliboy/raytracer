#!/usr/bin/env python

from random import random, uniform
from scenekit import *

resolution(1920/8, 1080/8)
detail(256, 128)
lookfrom(478, 278, -600)
lookat(278, 278, 0)
up(0, 1, 0)
camera(40, 0.1, 10, 0.0, 1.0)
ambient(0.01, 0.015, 0.02)

for i in range(0, 20):
    for j in range(0, 20):
        w = 100.0
        x0 = -1000.0 + i * w
        z0 = -1000.0 + j * w
        y0 = 0.0
        x1 = x0 + w
        y1 = uniform(1, 101)
        z1 = z0 + w

        box(x0, y0, z0, x1, y1, z1, diffuse(solid_color(0.48, 0.83, 0.53)))


zx_rectangle(123, 423, 147, 412, 554, diffuse_light(solid_color(7, 7, 7)))
moving_sphere(400, 400, 200, 430, 400, 200, 0, 1, 50, diffuse(solid_color(0.7, 0.3, 0.1)))
sphere(260, 150, 45, 50, dielectric(1.5))
sphere(0, 150, 145, 50, metal(0.8, 0.8, 0.9, 1.0))

sphere(360, 150, 145, 70, dielectric(1.5))
constant_medium(360, 150, 145, 70, 0.2, isotropic(solid_color(0.2, 0.4, 0.9)))
constant_medium(0, 0, 0, 5000, 0.0001, isotropic(solid_color(1, 1, 1)))

sphere(400, 200, 400, 100, diffuse(image("images/earth.ppm")))

sphere(220, 280, 300, 80, diffuse(noise(0.1)))

for i in range(1000):
    sphere(uniform(0, 165) - 100, uniform(0, 165) + 270, uniform(0, 165) + 395, 10, diffuse(solid_color(0.73, 0.73, 0.73)))

printscene()
