#include "material_dielectric.hpp"

#include "ray.hpp"
#include "hit.hpp"
#include "utils.hpp"

bool rt::Dielectric::scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) {

    Vec3 outward_normal;
    const Vec3 reflected = reflect(ray_in.dir, surface_hit.normal);
    float ni_over_nt;

    // no attenuation
    attenuation = Vec3::make(1.f);

    Vec3 refracted;
    float reflect_probability;
    float cosine;

    // we are inside the material i.e. we go from inside the material to the air (refractive idx = 1.0)
    if (dot_product(ray_in.dir, surface_hit.normal) > 0) {
        outward_normal = -surface_hit.normal;
        // m_refractive_idx / 1.0
        ni_over_nt = m_refractive_idx;
        cosine = m_refractive_idx * dot_product(ray_in.dir, surface_hit.normal) / ray_in.dir.length();
    }
    else {
        outward_normal = surface_hit.normal;
        ni_over_nt = 1.f / m_refractive_idx;
        cosine = -dot_product(ray_in.dir, surface_hit.normal) / ray_in.dir.length();
    }

    if (rt::refract(ray_in.dir, outward_normal, ni_over_nt, refracted)) {
        reflect_probability = rt::schlick(cosine, m_refractive_idx);
    }
    else {
        ray_out = Ray(surface_hit.pos, reflected);
        reflect_probability = 1.0;
    }

    if (drand48() < reflect_probability) {
        ray_out = Ray(surface_hit.pos, reflected);
    }
    else {
        ray_out = Ray(surface_hit.pos, refracted);
    }

    return true;
} 

