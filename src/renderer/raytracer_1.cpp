#pragma once

#include "config.hpp"
#include "hittable.hpp"
#include "scene.hpp"
#include "sphere.hpp"

color create_color_from_vec(const vec3<double> vec) {
  return {int(255.999 * vec.x()), int(255.999 * vec.y()),
          int(255.999 * vec.z())};
}

color color_ray(const ray<double> &r, const hittable<double> &world) {
  hit_record<double> record;
  if (world.hit(r, 0, INFINITY, record)) {
    return record.surface_color;
  }

  vec3<double> unit_direction = normalize(r.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);

  vec3<double> color_gradient =
      (1.0 - a) * vec3<double>(1.0, 1.0, 1.0) + a * vec3<double>(0.5, 0.7, 1.0);

  // Translate the [0,1] component values to the byte range [0,255].
  return create_color_from_vec(color_gradient);
}

// Converts screen space (pixel coordinates) to viewport coordinates
vec3<double> screen_to_viewport(int x, int y) {

  // Center the screen at (0,0) and flip Y-axis
  auto Cx = x - (SCREEN_WIDTH / 2.0);
  auto Cy = (SCREEN_HEIGHT / 2.0) - y;

  // Transform centered coordinates to viewport coordinates in worldspace at
  // projection plane at Z = 1 from camera origin
  auto Vx = Cx * (VIEWPORT_WIDTH / SCREEN_WIDTH);
  auto Vy = Cy * (VIEWPORT_HEIGHT / SCREEN_HEIGHT);
  auto Vz = FOCAL_LENGTH;

  return vec3<double>(Vx, Vy, Vz);
}

// color traceRay(point3<double> origin, vec3<double> direction, double t_min,
//                double t_max) {
//   double closest_t = INFINITY;
//   double closest_sphere = NULL;

//   for (sphere in scene.spheres) {
//     t1, t2 = IntersectRaySphere(origin, direction, sphere);
//     if (t1 in[t_min, t_max] and t1 < closest_t) {
//       closest_t = t1;
//       closest_sphere = sphere;
//     }
//     if (t2 in[t_min, t_max] and t2 < closest_t) {
//       closest_t = t2;
//       closest_sphere = sphere;
//     }
//     if closest_sphere
//       == NULL { return color(255, 255, 255); }
//   }
// }

// Raytracing
void rt1(Canvas &canvas) {

  // Camera position is the origin of each ray.
  point3<double> camera_position = {0.0, 0.0, 0.0};

  // World
  scene<double> world;
  // Objects
  world.add(make_shared<sphere<double>>(point3<double>(0.0, -1.0, 3.0), 1.0,
                                        color(255, 0, 0))); // RED
  world.add(make_shared<sphere<double>>(point3<double>(2.0, 0.0, 4.0), 1.0,
                                        color(0, 0, 255))); // BLUE
  world.add(make_shared<sphere<double>>(point3<double>(-2.0, 0.0, 4.0), 1.0,
                                          color(0, 255, 0))); // GREEN
  // Renderer
  for (int x : std::ranges::iota_view(0, SCREEN_WIDTH)) {
    for (int y : std::ranges::iota_view(0, SCREEN_HEIGHT)) {

      point3<double> viewport_point = screen_to_viewport(x, y);
      vec3<double> ray_direction = normalize(viewport_point - camera_position);

      // Raypoints P = camera_position + t*ray_direction
      ray<double> r(camera_position, ray_direction);
      color pixel_color = color_ray(r, world);
      canvas.set_pixel(x, y, pixel_color);
    }
  }
}
