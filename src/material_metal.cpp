#include "material_metal.hpp"

#include "ray.hpp"
#include "hit.hpp"

bool rt::Metal::scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) {

    const Vec3 reflected = reflect(normalize_vec(ray_in.dir), surface_hit.normal);
    ray_out = Ray(surface_hit.pos, reflected);
    attenuation = m_albedo;

    return dot_product(ray_out.dir, surface_hit.normal) > 0;
} 

