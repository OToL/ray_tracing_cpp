#include "hit.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"
#include "camera.hpp"
#include "material_lambertian.hpp"
#include "material_metal.hpp"
#include "material_dielectric.hpp"

#include <cassert>
#include <random>
#include <raylib.h>
#include <vector>
#include <cassert>
#include <memory>
// #include <print>

// TODO: investigate

rt::Vec3 get_sky_color(const rt::Ray & r)
{
    const rt::Vec3 unit_dir = rt::normalize_vec(r.dir);
    const float t = 0.5 * (unit_dir.y + 1.f);

    // interpolation from white to light blue
    // (light_blue - white) * t + white
    // light_blue*t + white * (1-t)
    return rt::Vec3(1.f, 1.f, 1.f) * (1.f - t) + rt::Vec3(0.5, 0.7, 1.0) * t;
}

struct Surface
{
    enum Type
    {
        Sphere
    };

    Type type;
    std::unique_ptr<rt::Material> material;
    std::uint32_t id;
};

std::vector<rt::Sphere> g_spheres;
std::vector<Surface> g_surfaces;

// x*x + y*y + z*z = r*r
// dot(ray(t) - sphere.center, ray(t) - sphere.center) = sphere.radius * sphere.radius
//      --> ray(t) = ray.origin + t*ray.dir
// (ray.origin + ray.dir*t - sphere.center) . (ray.origin + ray.dir*t - sphere.center) = sphere.radius * sphere.radius
// (ray.dir*t + (ray.origin - sphere.center)) . (ray.dir*t + (ray.origin - sphere.center)) = sphere.radius * sphere.radius
// t*t*dot(ray.dir,ray.dir) + t*dot(ray.dir, ray.origin-sphere.center) + t*dot(ray.origin-sphere.center, ray.dir) + dot(ray.origin-sphere.center,
// ray.origin-sphere.center) - sphere.radius*sphere.radius = 0 t*t*dot(ray.dir,ray.dir) + 2t*dot(ray.dir, ray.origin-sphere.center) +
// dot(ray.origin-sphere.center, ray.origin-sphere.center) - sphere.radius*sphere.radius = 0
// --> quadratic equation
// --> is there any root ? determinant: b*b - 4*a*c
bool hit_sphere(const rt::Ray & r, float min_t, float max_t, const rt::Sphere & sphere, rt::Hit & hit)
{
    const rt::Vec3 oc = r.origin - sphere.center;
    const float a = rt::dot_product(r.dir, r.dir);
    const float b = rt::dot_product(oc, r.dir);
    const float c = rt::dot_product(oc, oc) - sphere.radius * sphere.radius;

    // t*t*dot(ray.dir, ray.dir) 2*t*dot(ray.dir, oc) + (dot(oc, oc)) - radius*radius) = 0
    // (-b + sqrt(b*b - 4*a*c))/2*a
    // (-2 * dot(ray.dir, oc) + sqrt(4*dot(ray.dir, oc)*dot(ray.dir, oc) - 4 * a*c)) / 2*a
    // -2*dot(ray.dir, oc) + 2*sqrt(dot(ray.dir, oc)*dot(ray.dir, oc) - a*c) / 2*a
    // -dot(ray.dir, oc) + sqrt(dot(ray.dir, oc)*dot(ray.dir, oc) - a*c) / a

    const float descriminant = b * b - a * c;
    if (descriminant >= 0)
    {
        float tmp = (-b - std::sqrt(descriminant)) / a;

        if (tmp > max_t || tmp < min_t)
        {
            tmp = (-b + std::sqrt(descriminant)) / a;
            if (tmp > max_t || tmp < min_t)
            {
                return false;
            }
        }

        hit.t = tmp;
        hit.pos = r.point_at_time(hit.t);
        // since the hit is on the sphere, the length of the diff hit-sphere.center is the sphere radius
        hit.normal = (hit.pos - sphere.center) / sphere.radius;

        return true;
    }

    return false;
}

bool hit_world(const rt::Ray & r, float min_t, float max_t, rt::Hit & hit)
{
    bool has_hit = false;
    hit = {.t = max_t};

    for (auto const & surface : g_surfaces)
    {
        switch (surface.type)
        {
            case Surface::Sphere:
            {
                rt::Hit local_hit;
                if (hit_sphere(r, min_t, max_t, g_spheres[surface.id], local_hit))
                {
                    has_hit = true;
                    if (local_hit.t < hit.t)
                    {
                        hit = local_hit;
                        hit.material = surface.material.get();
                    }
                }

                break;
            }
            default:
            {
                assert(false);
                break;
            }
        }
    }

    return has_hit;
}

rt::Vec3 get_hit_world_color(const rt::Ray & r, float min_t, float max_t, int depth = 0)
{
    rt::Vec3 hit_color;
    rt::Hit ray_hit;

    if (hit_world(r, min_t, max_t, ray_hit))
    {
        rt::Ray scattered;
        rt::Vec3 attenuation;

        if ((depth < 50) && ray_hit.material->scatter(r, ray_hit, attenuation, scattered))
        {
            return attenuation * get_hit_world_color(scattered, min_t, max_t, depth + 1);
        }
        else
        {
            return rt::Vec3(0, 0, 0);
        }
    }
    else
    {
        hit_color = get_sky_color(r);
    }

    return hit_color;
}

void add_surface(rt::Sphere const & sphere, std::unique_ptr<rt::Material> && material)
{
    g_spheres.push_back(sphere);
    g_surfaces.emplace_back(Surface::Sphere, std::move(material), static_cast<std::uint32_t>(g_spheres.size() - 1));
}

enum class AntiAliasingMode : std::uint8_t
{
    None,
    Neighbors,
    Rand
};

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 400;

    float const R = cos(M_PI/4);
    // rt::Camera camera = rt::Camera::make(rt::Vec3(0, 0, 0), rt::Vec3(-2, -1, -1));
    // rt::Camera camera = rt::Camera::make(rt::Vec3(0,0,1), 90, static_cast<float>(SCREEN_WIDTH)/SCREEN_HEIGHT);
    rt::Camera camera = rt::Camera::make(rt::Vec3(-2,2,1), rt::Vec3(0,0,-1), rt::Vec3(0,1, 0), 90, static_cast<float>(SCREEN_WIDTH)/SCREEN_HEIGHT);
    // rt::Camera camera = rt::Camera::make(rt::Vec3(0,0,1), rt::Vec3(0,0,-1), rt::Vec3(0,1, 0), 90, static_cast<float>(SCREEN_WIDTH)/SCREEN_HEIGHT);

    // add_surface({.center = rt::Vec3(-R, 0, -1), .radius = R}, std::make_unique<rt::Lambertian>(rt::Vec3(0, 0, 1)));
    // add_surface({.center = rt::Vec3(R, 0, -1), .radius = R}, std::make_unique<rt::Lambertian>(rt::Vec3(1, 0, 0)));

    // supporting sphere
    add_surface({.center = rt::Vec3(0, -100.5, -1), .radius = 100}, std::make_unique<rt::Lambertian>(rt::Vec3(0.8, 0.8, 0.0)));

    add_surface({.center = rt::Vec3(0, 0, -1), .radius = 0.5}, std::make_unique<rt::Lambertian>(rt::Vec3(0.8, 0.3, 0.3)));
    add_surface({.center = rt::Vec3(1, 0, -1), .radius = 0.5}, std::make_unique<rt::Metal>(rt::Vec3(0.8, 0.6, 0.2)));
    add_surface({.center = rt::Vec3(-1, 0, -1), .radius = 0.5}, std::make_unique<rt::Dielectric>(1.5));
    add_surface({.center = rt::Vec3(-1, 0, -1), .radius = -0.45}, std::make_unique<rt::Dielectric>(1.5));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raytracing");
    SetTargetFPS(60);

    Image img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, RED);
    Texture2D tex = LoadTextureFromImage(img);

    Color * img_data = (Color *)img.data;

    // seed: 42
    std::mt19937 rd(42);
    std::uniform_real_distribution<float> dist(0, 1);

    static AntiAliasingMode aa_mode = AntiAliasingMode::Rand;
    const std::uint32_t nb_rand_samples = 15;

    printf("camera origin %f %f %f\n", camera.origin.x, camera.origin.y, camera.origin.z);

    // TODO: try using views
    for (int row_idx = 0; row_idx != SCREEN_HEIGHT; ++row_idx)
    {
        for (int col_idx = 0; col_idx != SCREEN_WIDTH; ++col_idx)
        {
            const int idx = row_idx * SCREEN_WIDTH + col_idx;

            rt::Vec3 hit_color = rt::Vec3(0, 0, 0);

            if (aa_mode == AntiAliasingMode::None)
            {
                const float u = static_cast<float>(col_idx) / (SCREEN_WIDTH - 1);
                const float v = static_cast<float>(SCREEN_HEIGHT - row_idx - 1) / (SCREEN_HEIGHT - 1);

                rt::Ray my_ray = camera.get_ray(u, v);
                hit_color += get_hit_world_color(my_ray, 0.00001, std::numeric_limits<float>::max());
            }
            else if (aa_mode == AntiAliasingMode::Rand)
            {
                for (int samp_idx = 0; samp_idx != nb_rand_samples; ++samp_idx)
                {
                    const float u = static_cast<float>(col_idx + dist(rd)) / (SCREEN_WIDTH - 1);
                    const float v = static_cast<float>(SCREEN_HEIGHT - row_idx + dist(rd) - 1) / (SCREEN_HEIGHT - 1);

                    rt::Ray my_ray = camera.get_ray(u, v);
                    hit_color += get_hit_world_color(my_ray, 0.00001, std::numeric_limits<float>::max());
                }

                hit_color /= nb_rand_samples;
            }
            else if (aa_mode == AntiAliasingMode::Neighbors)
            {
                for (int offset_u = -1; offset_u <= 1; offset_u++)
                {
                    for (int offset_v = -1; offset_v <= 1; offset_v++)
                    {
                        const float u = static_cast<float>(col_idx + offset_u) / (SCREEN_WIDTH - 1);
                        const float v = static_cast<float>(SCREEN_HEIGHT - row_idx + offset_v - 1) / (SCREEN_HEIGHT - 1);

                        rt::Ray my_ray = camera.get_ray(u, v);
                        hit_color += get_hit_world_color(my_ray, 0.00001, std::numeric_limits<float>::max());
                    }
                }

                hit_color /= 9;
            }

            // gamma correct the value
            // value^(1/gamma)
            // i.e. gamma 2 = value^1/2 = sqrt(value)
            // --> can mapp values [0,1] to the same range (i.e. [0,1]) but make the intermediate values greater
            hit_color = rt::Vec3(sqrt(hit_color.x), sqrt(hit_color.y), sqrt(hit_color.z));
            img_data[idx].r = hit_color.x * 255;
            img_data[idx].g = hit_color.y * 255;
            img_data[idx].b = hit_color.z * 255;
            img_data[idx].a = 255;
        }
    }

    UpdateTexture(tex, img_data);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(tex, 0, 0, WHITE);

        EndDrawing();
    }

    CloseWindow();

    UnloadTexture(tex);
    UnloadImage(img);

    return 0;
}
