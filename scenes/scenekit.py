#!/usr/bin/env python

class scene:
    def __init__(self):
        self.textures = {}
        self.materials = {}
        self.objects = []
    def resolution(self, width, height):
        self.width = width
        self.height = height
    def detail(self, samples_per_pixel, max_ray_bounces):
        self.samples_per_pixel = samples_per_pixel
        self.max_ray_bounces = max_ray_bounces
    def lookfrom(self, x, y, z):
        self.lookfrom_x = x
        self.lookfrom_y = y
        self.lookfrom_z = z
    def lookat(self, x, y, z):
        self.lookat_x = x
        self.lookat_y = y
        self.lookat_z = z
    def up(self, x, y, z):
        self.up_x = x
        self.up_y = y
        self.up_z = z
    def camera(self, fov, aperture, focal_length, shutter_open, shutter_close):
        self.fov = fov
        self.aperture = aperture
        self.focal_length = focal_length
        self.shutter_open = shutter_open
        self.shutter_close = shutter_close
    def ambient(self, r, g, b):
        self.ambient_r = r
        self.ambient_g = g
        self.ambient_b = b
    def addtexture(self, texture):
        if not texture in self.textures:
            self.textures[texture] = len(self.textures)
        return self.textures[texture]
    def addmaterial(self, material):
        if not material in self.materials:
            self.materials[material] = len(self.materials)
        return self.materials[material]
    def addobject(self, object, material):
        self.objects.append(f'{object} {material}')
    def print(self):
        print(f'{int(self.width)} {int(self.height)}')
        print(f'{int(self.samples_per_pixel)} {int(self.max_ray_bounces)}')
        print(f'{self.lookfrom_x} {self.lookfrom_y} {self.lookfrom_z}')
        print(f'{self.lookat_x} {self.lookat_y} {self.lookat_z}')
        print(f'{self.up_x} {self.up_y} {self.up_z}')
        print(f'{self.fov} {self.aperture} {self.focal_length} {self.shutter_open} {self.shutter_close}')
        print(f'{self.ambient_r} {self.ambient_g} {self.ambient_b}')
        for texture in self.textures:
            print(texture)
        for material in self.materials:
            print(material)
        for object in self.objects:
            print(object)
        

scn = scene()

resolution = lambda width, height: scn.resolution(width, height)
detail = lambda samples, bounces: scn.detail(samples, bounces)
lookfrom = lambda x, y, z: scn.lookfrom(x, y, z)
lookat = lambda x, y, z: scn.lookat(x, y, z)
up = lambda x, y, z: scn.up(x, y, z)
camera = lambda fov, aperture, focal_length, shutter_open, shutter_close: scn.camera(fov, aperture, focal_length, shutter_open, shutter_close)
ambient = lambda r, g, b: scn.ambient(r, g, b)

solid_color = lambda r, g, b: scn.addtexture(f'solid_color {r} {g} {b}')
image = lambda path: scn.addtexture(f'image {path}')
noise = lambda scale: scn.addtexture(f'noise {scale}')

diffuse = lambda tex: scn.addmaterial(f'diffuse {tex}')
dielectric = lambda ir: scn.addmaterial(f'dielectric {ir}')
metal = lambda r, g, b, f: scn.addmaterial(f'metal {r} {g} {b} {f}')
diffuse_light = lambda tex: scn.addmaterial(f'diffuse_light {tex}')
isotropic = lambda albedo: scn.addmaterial(f'isotropic {albedo}')

sphere = lambda x, y, z, r, mat: scn.addobject(f'sphere {x} {y} {z} {r}', mat)
box = lambda x0, y0, z0, x1, y1, z1, mat: scn.addobject(f'box {x0} {y0} {z0} {x1} {y1} {z1}', mat)
zx_rectangle = lambda z0, z1, x0, x1, k, mat: scn.addobject(f'zx_rectangle {z0} {z1} {x0} {x1} {k}', mat)
moving_sphere = lambda x0, y0, z0, x1, y1, z1, t0, t1, r, mat: scn.addobject(f'moving_sphere {x0} {y0} {z0} {x1} {y1} {z1} {t0} {t1} {r}', mat)
#currently sphere boundary only
constant_medium = lambda x, y, z, r, density, phase: scn.addobject(f'constant_medium {x} {y} {z} {r} {density}', phase)

printscene = lambda: scn.print()
