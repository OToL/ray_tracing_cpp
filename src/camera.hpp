#pragma once

#include "vec3.hpp"
#include "ray.hpp"

#include <cassert>

namespace rt {

    // Right handed i.e.
    //   Y is up
    //   X is right
    //   Z is pointing in the opposite direction of the canera view 
    struct Camera {

        Vec3 origin;
        Vec3 vis_plane_bottom_left;
        Vec3 vis_plane_width;
        Vec3 vis_plane_height;

        static Camera make(Vec3 const& origin, Vec3 const& vis_plane_bottom_left)
        {
            return {
                .origin = origin,
                .vis_plane_bottom_left = vis_plane_bottom_left,
                .vis_plane_width = Vec3(-vis_plane_bottom_left.x*2, 0, 0),
                .vis_plane_height = Vec3(0, -2*vis_plane_bottom_left.y, 0)
            };
        }
        static Camera make(const Vec3& from, float vertical_fov, float aspect_ratio_deg);
        static Camera make(const Vec3& from, const Vec3& at, const Vec3& vup, float vertical_fov_deg, float aspect_ratio);

        rt::Ray get_ray(float u, float v) const {
            return rt::Ray(origin, vis_plane_bottom_left + vis_plane_width * u + vis_plane_height * v - origin);
        }
    };
}
