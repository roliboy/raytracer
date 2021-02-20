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
        print(f'{int(self.width)}, {int(self.height)}')
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
        

solid_color = lambda r, g, b: f'solid_color {r} {g} {b}'
#image = lambda path: f'image {path}'
#noise = lambda scale: f'noise {scale}'
#
diffuse = lambda ti: f'diffuse {ti}'
#diffuse_light = lambda ti: f'diffuse_light {ti}'
#dielectric = lambda ir: f'dielectric {ir}'
#metal = lambda r, g, b, f: f'metal {r} {g} {b} {f}'
#
sphere = lambda x, y, z, d: f'sphere {x} {y} {z} {d}'
box = lambda x0, y0, z0, x1, y1, z1: f'box {x0} {y0} {z0} {x1} {y1} {z1}'
#zx_rectangle = lambda z0, z1, x0, x1, k: f'zx_rectangle {z0} {z1} {x0} {x1} {k}'
#moving_sphere = lambda x0, y0, z0, x1, y1, z1, t0, t1, r: f'moving_sphere {x0} {y0} {z0} {x1} {y1} {z1} {t0} {t1} {r}'

scn = scene()
scn.resolution(1920/10, 1080/10)
scn.detail(32, 32)
scn.lookfrom(1, 2, 3)
scn.lookat(2, 3, 4)
scn.up(0, 1, 0)
scn.camera(40, 0.1, 10, 0, 1)
scn.ambient(0, 0, 0)
scn.addobject(sphere(1, 2, 3, 4), scn.addmaterial(diffuse(scn.addtexture(solid_color(1, 2, 3)))))
scn.addobject(box(1, 2, 3, 4, 5, 6), scn.addmaterial(diffuse(scn.addtexture(solid_color(1, 3, 3)))))
scn.print()

# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 2, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(dielectric(1.5)))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 3, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(1, 2, 3)))))
# makeobject(sphere(1, 2, 3, 4), makematerial(diffuse(maketexture(solid_color(0, 3, 3)))))

