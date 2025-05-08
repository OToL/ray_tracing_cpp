#include "utils.hpp"


// rejection implementation i.e. we generate random point in a cube until it fits in unit sphere
rt::Vec3 rt::get_random_point_in_unit_sphere()
{
    rt::Vec3 res;
    do {
        res = 2 * rt::Vec3(drand48(), drand48(), drand48()) - rt::Vec3(1,1,1);
    } while(rt::dot_product(res, res) >= 1);


    return res;
}

rt::Vec3 rt::get_random_point_in_unit_disk() {
    rt::Vec3 res;

    do {
        res = 2 * rt::Vec3(drand48(), drand48(), 0) - rt::Vec3(1,1,0);
    } while(rt::dot_product(res, res) >= 1);

    return res;
}

// https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
bool rt::refract(const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted) {
    const Vec3 unit_v = normalize_vec(v);
    const float dt = dot_product(unit_v, normal);
    const float discriminant = 1.f - ni_over_nt * ni_over_nt * (1 - dt*dt);

    if (discriminant > 0) {
        refracted = ni_over_nt * (unit_v - normal*dt) - normal*std::sqrt(discriminant);
        return true;
    }

    return false;
}

float rt::schlick(float cosine, float refractive_idx)
{
    float r0 = (1-refractive_idx) / (1 + refractive_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*std::pow((1-cosine), 5);
}

float rt::deg_to_rad(float angle_deg)
{
    return angle_deg * M_PI/180.f;
}

