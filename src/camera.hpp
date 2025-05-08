#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "utils.hpp"

#include <cassert>

namespace rt {

// Right handed i.e.
//   Y is up
//   X is right
//   Z is pointing in the opposite direction of the canera view
struct Camera
{
    Vec3 origin;
    Vec3 vis_plane_bottom_left;
    Vec3 vis_plane_width;
    Vec3 vis_plane_height;
    Vec3 up;
    Vec3 right;
    Vec3 z;
    float lens_radius;

#if 0
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
#endif

    static Camera make(const Vec3 & from, const Vec3 & at, const Vec3 & vup, float vertical_fov_deg, float aspect_ratio, float aperture_deg,
                       float focus_dist);

    rt::Ray get_ray(float u, float v) const
    {
        rt::Vec3 rd = lens_radius * rt::get_random_point_in_unit_disk();
        rt::Vec3 offset = rd.x*right + rd.y*up;
        rt::Vec3 new_origin = origin + offset;

        return rt::Ray(new_origin, vis_plane_bottom_left + vis_plane_width * u + vis_plane_height * v - new_origin);
    }
};
} // namespace rt
