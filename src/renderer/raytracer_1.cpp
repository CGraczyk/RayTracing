#pragma once

#include "config.hpp"
#include "hittable.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "sphere.hpp"

color create_color_from_vec(const vec3<double> vec) {
  return {int(255.999 * vec.x()), int(255.999 * vec.y()),
          int(255.999 * vec.z())};
}

double compute_light_multiplier(const ray<double> &r,
                                const scene<double> &world,
                                point3<double> camera_position,
                                const hit_record<double> &record) {

  double intensity = 0.0;
  auto lights = world.get_lights();
  vec3<double> lightray;

  for (const auto &lightsource : lights) {
    if (lightsource->type == kAmbient) {
      intensity += lightsource->intensity;
    } else {
      if (lightsource->type == kPoint) {
        lightray = lightsource->position - record.p;
      } else {
        lightray = lightsource->position - camera_position;
      }

      // Diffusion
      auto normal_dot_lightray = dot(record.normal, lightray);
      if (normal_dot_lightray > 0) {
        intensity += lightsource->intensity * normal_dot_lightray /
                     (record.normal.length() * lightray.length());
      }

      // Specular
      if (record.specular != -1) {
        vec3<double> viewpoint = -r.direction();
        vec3<double> reflection =
            2.0 * record.normal * dot(record.normal, lightray) - lightray;
        auto reflection_dot_viewpoint = dot(reflection, viewpoint);

        if (reflection_dot_viewpoint > 0) {
          intensity += lightsource->intensity *
                       pow(reflection_dot_viewpoint /
                               (reflection.length() * viewpoint.length()),
                           record.specular);
        }
      }
    }
  }
  return intensity;
}

color color_ray(const ray<double> &r, const scene<double> &world) {
  hit_record<double> record;
  point3<double> camera_position = {0.0, 0.0, 0.0};
  if (world.hit(r, 0, INFINITY, record)) {

    color object_color = record.surface_color;
    vec3<double> colcont = {object_color.x() / 255.0, object_color.y() / 255.0,
                            object_color.z() / 255.0};

    double lighting_multiplier =
        compute_light_multiplier(r, world, camera_position, record);
    return create_color_from_vec(colcont * lighting_multiplier);
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
  for (int i = 0; i < 1; i++) {
    world.add_hittable(
        make_shared<sphere<double>>(point3<double>(0.3, i * 1.0 - 1.0, 3.0),
                                    1.0, color(255, 0, 0), 500)); // RED
  }

  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(2.0, 1.0, 8.0), 1.0, color(0, 0, 255), 200)); // BLUE
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(-2.0, 0.0, 5.0), 1.0, color(0, 255, 0), 200)); // GREEN

  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(0.0, -5001.0, 0.0), 5000.0, color(255, 255, 0),
      10)); // YELLOW

  // Lights
  world.add_light(
      make_shared<light<double>>(kAmbient, point3<double>(0, 0, 0), 0.2));
  world.add_light(
      make_shared<light<double>>(kPoint, point3<double>(-6.0, 10.0, 4.0), 0.4));
  world.add_light(make_shared<light<double>>(
      kDirectonal, point3<double>(2.0, 10.0, -5.0), 0.6));

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
