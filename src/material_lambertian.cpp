#include "material_lambertian.hpp"

#include "ray.hpp"
#include "hit.hpp"
#include "utils.hpp"

bool rt::Lambertian::scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) {

    rt::Vec3 target = surface_hit.pos + surface_hit.normal + rt::get_random_point_in_unit_sphere();
    ray_out = Ray(surface_hit.pos, target - surface_hit.pos);
    attenuation = m_albedo;

    return true;
} 
