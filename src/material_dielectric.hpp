#pragma once

#include "material.hpp"
#include "vec3.hpp"

namespace rt {

    class Dielectric : public Material {

        public:

            Dielectric(float refractive_idx) :
                m_refractive_idx(refractive_idx)
            {
            }

            bool scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) override; 

        private:

            float m_refractive_idx;
    };

}
