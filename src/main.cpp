#include <cassert>
#include <raylib.h>
// #include <print>

#include "ray.hpp"
#include "vec3.hpp"

// TODO: investigate
rt::Vec3 color(const rt::Ray &r) {
  const rt::Vec3 unit_dir = rt::normalize_vec(r.dir);
  const float t = 0.5 * (unit_dir.y + 1.f);

  // interpolation from white to light blue
  // (light_blue - white) * t + white
  // light_blue*t + white * (1-t)
  return rt::Vec3(1.f, 1.f, 1.f) * (1.f - t) + rt::Vec3(0.5, 0.7, 1.0) * t;
}

// (B*t + (A - C)) . (B*t + (A - C)) = R*R
// t*t*dot(B,B) + t*dot(B, A-C) + t*dot(A-C, B) + dot(A-C, A-C) = R*R
// t*t*dot(B,B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R = 0 

int main() {

  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 400;

  // Right hand convention with Z poiting in the opposite screen direction
  // i.e. Y up, X right and Z opposite
  // Projection plane has the same propotion as the physical screen i.e. 2x
  // larger
  const rt::Vec3 proj_plane_bottom_left(-2, -1, -1);
  // from -2 to +2
  const rt::Vec3 proj_plane_width(4, 0, 0);
  // from -1 to +1
  const rt::Vec3 proj_plane_height(0, 2, 0);
  // eye is at 0,0,0 and points to the middle of the proj plane
  const rt::Vec3 view_origin(0, 0, 0);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raytracing");
  SetTargetFPS(60);

  Image img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, RED);
  Texture2D tex = LoadTextureFromImage(img);

  Color *img_data = (Color *)img.data;

  // TODO: try using views
  for (int row_idx = SCREEN_HEIGHT - 1; row_idx >= 0; --row_idx) {
    for (int col_idx = 0; col_idx != SCREEN_WIDTH; ++col_idx) {
      const int idx = (SCREEN_HEIGHT - 1 - row_idx) * SCREEN_WIDTH + col_idx;

      const float u = static_cast<float>(col_idx) / (SCREEN_WIDTH - 1);
      const float v = static_cast<float>(row_idx) / (SCREEN_HEIGHT - 1);

      rt::Ray my_ray(view_origin, proj_plane_bottom_left +
                                      proj_plane_width * u +
                                      proj_plane_height * v);
      const rt::Vec3 col = color(my_ray);

      img_data[idx].r = col.x * 255;
      img_data[idx].g = col.y * 255;
      img_data[idx].b = col.z * 255;
      img_data[idx].a = 255;
    }
  }

  UpdateTexture(tex, img_data);

  while (!WindowShouldClose()) {
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
