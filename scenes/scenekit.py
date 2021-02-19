#!/usr/bin/env python

from random import random, uniform

size = lambda width, height: print(f'{int(width)} {int(height)}')
ray = lambda samples, bounces: print(f'{samples} {bounces}')
camera = lambda x, y, z: print(f'{x} {y} {z}')
scene = lambda x, y, z: print(f'{x} {y} {z}')
up = lambda x, y, z: print(f'{x} {y} {z}')
settings = lambda fov, aperture, focus, shutter_open, shutter_close: print(f'{fov} {aperture} {focus} {shutter_open} {shutter_close}')
ambient = lambda r, g, b: print(f'{r} {g} {b}')


solid_color = lambda r, g, b: f'solid_color {r} {g} {b}'
image = lambda path: f'image {path}'
noise = lambda scale: f'noise {scale}'

diffuse = lambda ti: f'diffuse {ti}'
diffuse_light = lambda ti: f'diffuse_light {ti}'
dielectric = lambda ir: f'dielectric {ir}'
metal = lambda r, g, b, f: f'metal {r} {g} {b} {f}'

sphere = lambda x, y, z, d: f'sphere {x} {y} {z} {d}'
box = lambda x0, y0, z0, x1, y1, z1: f'box {x0} {y0} {z0} {x1} {y1} {z1}'
zx_rectangle = lambda z0, z1, x0, x1, k: f'zx_rectangle {z0} {z1} {x0} {x1} {k}'
moving_sphere = lambda x0, y0, z0, x1, y1, z1, t0, t1, r: f'moving_sphere {x0} {y0} {z0} {x1} {y1} {z1} {t0} {t1} {r}'


texturemap = {}
def maketexture(texture):
    if not texture in texturemap:
        texturemap[texture] = len(texturemap)
    return texturemap[texture]


materialmap = {}
def makematerial(material):
    if not material in materialmap:
        materialmap[material] = len(materialmap)
    return materialmap[material]


objectmap = []
def makeobject(object, material):
    objectmap.append(f'{object} {material}')


def printscene():
    for texture in texturemap:
        print(texture)
    for material in materialmap:
        print(material)
    for object in objectmap:
        print(object)



# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 2, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(dielectric(1.5)))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 3, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 2, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(0, 3, 3)))))

