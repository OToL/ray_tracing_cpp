#pragma once

#include "vec3.hpp"

namespace rt {

struct Ray {
    Vec3 origin;
    Vec3 dir;

    Ray() {}
    Ray(const Vec3& _orig, const Vec3& _dir) :
        origin(_orig),
        dir(_dir)
    {
    }

    Vec3 point_at_time(float t) const {
        return origin + dir * t;
    }
};

}
