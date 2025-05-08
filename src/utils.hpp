#pragma once

#include "vec3.hpp"

namespace rt {

rt::Vec3 get_random_point_in_unit_sphere();

rt::Vec3 get_random_point_in_unit_disk();

// ni: refraction index in 
// nt: refraction index to 
bool refract(const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted);

// glass reflectivity varies with angle
float schlick(float cosine, float refractive_idx);

float deg_to_rad(float angle_deg);

} // namespace rt
