#include "camera.hpp"
#include "utils.hpp"

rt::Camera rt::Camera::make(const Vec3& from, float vertical_fov_deg, float aspect_ratio)
{
    const float vfov_rad = rt::deg_to_rad(vertical_fov_deg);
    const float half_height = std::tan(vfov_rad/2.f);
    const float half_width = aspect_ratio * half_height;

    const Camera cam =  {
        .origin = from,
        .vis_plane_bottom_left = Vec3(-half_width, -half_height, -1.f),
        .vis_plane_width = Vec3(2*half_width, 0, 0),
        .vis_plane_height = Vec3(0, half_height*2, 0),
    };

    printf("bottom left %f %f %f \n", cam.vis_plane_bottom_left.x, cam.vis_plane_bottom_left.y, cam.vis_plane_bottom_left.z);
    printf("width %f %f %f \n", cam.vis_plane_width.x, cam.vis_plane_width.y, cam.vis_plane_width.z);
    printf("height %f %f %f \n", cam.vis_plane_height.x, cam.vis_plane_height.y, cam.vis_plane_height.z);
    printf("origin %f %f %f \n", cam.origin.x, cam.origin.y, cam.origin.z);

    return cam;
}

rt::Camera rt::Camera::make(const Vec3& look_from, const Vec3& look_at, const Vec3& vup, float vertical_fov_deg, float aspect_ratio)
{
#if 0
    const float vfov_rad = vertical_fov_deg*M_PI/180;
    const float half_height = std::tan(vfov_rad/2.f);
    const float half_width = aspect_ratio * half_height;

    // goes in the opposite direction
    const Vec3 camera_z = normalize_vec(from - at);
    const Vec3 camera_x = normalize_vec(cross_product(Vec3(0,1,0), camera_z));
    const Vec3 camera_y = cross_product(camera_z, camera_x);

    printf("camera_z %f %f %f \n", camera_z.x, camera_z.y, camera_z.z);
    printf("camera_y %f %f %f \n", camera_y.x, camera_y.y, camera_y.z);
    printf("camera_x %f %f %f \n", camera_x.x, camera_x.y, camera_x.z);

    rt::Camera cam = {
        .origin = from,
        .vis_plane_bottom_left = from - half_width*camera_x - half_height*camera_y - camera_z,
        .vis_plane_width = 2*half_width*camera_x,
        .vis_plane_height = 2*half_height*camera_y,
    };

    printf("bottom left %f %f %f \n", cam.vis_plane_bottom_left.x, cam.vis_plane_bottom_left.y, cam.vis_plane_bottom_left.z);
    printf("width %f %f %f \n", cam.vis_plane_width.x, cam.vis_plane_width.y, cam.vis_plane_width.z);
    printf("height %f %f %f \n", cam.vis_plane_height.x, cam.vis_plane_height.y, cam.vis_plane_height.z);
    printf("origin %f %f %f \n", cam.origin.x, cam.origin.y, cam.origin.z);
    
    return cam;
#else
    Vec3 u,v,w;
    float theta = vertical_fov_deg*M_PI/180;
    float half_height = std::tan(theta/2);
    float half_width = aspect_ratio * half_height;

    Vec3 origin = look_from;
    w = normalize_vec(look_from - look_at);
    u = normalize_vec(cross_product(vup, w));
    v = normalize_vec(cross_product(w, u));

    return {
        .origin = origin,
        .vis_plane_bottom_left = origin - half_width*u - half_height*v - w,
        .vis_plane_width = 2*half_width*u,
        .vis_plane_height = 2*half_height*v
    };
#endif
}

