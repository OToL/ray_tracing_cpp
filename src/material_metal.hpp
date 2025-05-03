#pragma once

#include "material.hpp"
#include "vec3.hpp"

namespace rt {

    class Metal : public Material {

        public:

            Metal(Vec3 const& albedo) :
                m_albedo(albedo)
            {
            }

            bool scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) override; 

        private:

            Vec3 m_albedo;
    };

}
