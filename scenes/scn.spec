one entity per line

entity = object + material + texture

object = sphere, moving_sphere, box, constant_medium
    sphere: x, y, z, radius
    moving_sphere: x0, y0, z0, x1, y1, z1, time0, time1, radius
    box: x0, y0, z0, x1, y1, z1
    constant_medium: object, density

material = dielectric, diffuse, metal, diffuse_light, isotropic

texture = checker_texture image_texture noise_texture solid_color
