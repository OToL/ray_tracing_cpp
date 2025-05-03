#pragma once

namespace rt {

struct Ray;
struct Hit;
struct Vec3;

class Material {

    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray& ray_in, const Hit& surface_hit, Vec3& attenuation, Ray& ray_out) = 0;
};

}
