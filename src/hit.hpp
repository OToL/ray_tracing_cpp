#pragma once

#include "vec3.hpp"

namespace rt {

    class Material;

    struct Hit {
        Vec3 pos;
        Vec3 normal;
        float t;
        Material* material;
    };

}
