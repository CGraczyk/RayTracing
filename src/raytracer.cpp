#pragma once

#include "camera.hpp"
#include "config.hpp"
#include "hittable.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "sphere.hpp"

inline color create_color_from_vec(const vec3<double> vec) {
  return {int(255.999 * vec.x()), int(255.999 * vec.y()),
          int(255.999 * vec.z())};
}

inline vec3<double> reflect_direction(const vec3<double> &direction,
                                      const vec3<double> &normal) {
  return 2.0 * normal * dot(normal, direction) - direction;
}

vec3<double> trace(const ray<double> &r, const scene<double> &world,
                   const point3<double> &camera_position, int recursion_depth) {
  hit_record<double> record;
  if (world.hit(r, 0.001, INFINITY, record)) {

    vec3<double> local_color = {record.surface_color.x() / 255.0,
                                record.surface_color.y() / 255.0,
                                record.surface_color.z() / 255.0};

    local_color *= world.light_multiplier(r, camera_position, record);

    if (recursion_depth <= 0 or record.reflective <= 0) {
      return local_color;
    }
    ray<double> r_reflection(r.at(record.t),
                             reflect_direction(-r.direction(), record.normal));

    vec3<double> reflection_color =
        trace(r_reflection, world, camera_position, recursion_depth - 1);
    return local_color * (1 - record.reflective) +
           reflection_color * record.reflective;
  }

  // Create Background Sky Gradient.
  auto background_gradient = 0.5 * (normalize(r.direction()).y() + 1.0);

  // Translate the color values in [0,1] to the byte range [0,255] after return
  return (1.0 - background_gradient) * vec3<double>(1.0, 1.0, 1.0) +
         background_gradient * vec3<double>(0.5, 0.7, 1.0);
}

void populate_scene(scene<double> &world) {

  // Objects
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(0.0, -1.0, 3.0), 1.0, color(255, 0, 0), 100, 0.5)); // RED
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(2.0, 0.0, 4.0), 1.0, color(0, 0, 255), 500, 0.3)); // BLUE
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(-2.0, 0.0, 4.0), 1.0, color(0, 255, 0), 10, 0.1)); // GREEN
  world.add_hittable(
      make_shared<sphere<double>>(point3<double>(0.0, -5001.0, 0.0), 5000.0,
                                  color(255, 255, 0), 1000, 0.1)); // YELLOW

  // Lights
  world.add_light(
      make_shared<light<double>>(kAmbient, point3<double>(0, 0, 0), 0.2));
  world.add_light(
      make_shared<light<double>>(kPoint, point3<double>(-4.0, 3.0, -1.0), 0.6));
  world.add_light(make_shared<light<double>>(
      kDirectonal, point3<double>(4.0, 10.0, 2.0), 0.2));
}

// Raytracing
void raytracer(Canvas &canvas) {

  double yaw = pi / 10;
  double pitch = pi / 8;
  double roll = pi / 8;

  // Camera position for the ray origin.
  camera cam(point3<double>(-2.0, 1.0, 0.0), yaw, pitch, roll);

  int n_reflections = 3;

  // World
  scene<double> world;
  populate_scene(world);

  // Renderer
  for (int x : std::ranges::iota_view(0, SCREEN_WIDTH)) {
    for (int y : std::ranges::iota_view(0, SCREEN_HEIGHT)) {

      // Create ray for each pixel on the screen
      ray<double> r = cam.create_ray_to_pixel(x, y);

      // Compute the color of the pixel by raytracing the scene
      canvas.set_pixel(
          x, y,
          create_color_from_vec(trace(r, world, cam.position, n_reflections)));
    }
  }
}
