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

vec3<double> reflect_direction(const vec3<double> &direction,
                               const vec3<double> &normal) {
  return 2.0 * normal * dot(normal, direction) - direction;
}

double compute_light_multiplier(const ray<double> &r,
                                const scene<double> &world,
                                point3<double> camera_position,
                                const hit_record<double> &record) {

  double intensity = 0.0;
  auto lights = world.get_lights();
  vec3<double> lightray_direction;
  double t_max = 0.0;

  for (const auto &lightsource : lights) {
    if (lightsource->type == kAmbient) {
      intensity += lightsource->intensity;
    } else {
      if (lightsource->type == kPoint) {
        lightray_direction = lightsource->position - record.p;
        t_max = 1;
      } else {
        lightray_direction = lightsource->position - camera_position;
        t_max = INFINITY;
      }

      // Shadowcheck
      hit_record<double> shadow_record;
      ray<double> shadow_r(r.at(record.t), lightray_direction);
      if (!world.hit(shadow_r, 0.001, t_max, shadow_record)) {

        // Diffusion
        auto normal_dot_lightray = dot(record.normal, lightray_direction);
        if (normal_dot_lightray > 0) {
          intensity += lightsource->intensity * normal_dot_lightray /
                       (record.normal.length() * lightray_direction.length());
        }

        // Specular
        if (record.specular != -1) {
          vec3<double> viewpoint = -r.direction();
          vec3<double> reflection =
              reflect_direction(lightray_direction, record.normal);

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
  }
  return intensity;
}

vec3<double> color_ray(const ray<double> &r, const scene<double> &world,
                       int recursion_depth) {
  hit_record<double> record;
  point3<double> camera_position = {0.0, 0.0, 0.0};
  if (world.hit(r, 0.001, INFINITY, record)) {

    color object_color = record.surface_color;
    vec3<double> local_color = {object_color.x() / 255.0,
                                object_color.y() / 255.0,
                                object_color.z() / 255.0};

    local_color *= compute_light_multiplier(r, world, camera_position, record);

    double reflective = record.reflective;
    if (recursion_depth <= 0 or reflective <= 0) {
      return local_color;
    }
    ray<double> r_reflection(r.at(record.t),
                             reflect_direction(-r.direction(), record.normal));

    vec3<double> reflection_color =
        color_ray(r_reflection, world, recursion_depth - 1);
    return local_color * (1 - record.reflective) +
           reflection_color * record.reflective;
  }

  vec3<double> unit_direction = normalize(r.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);

  vec3<double> color_gradient =
      (1.0 - a) * vec3<double>(1.0, 1.0, 1.0) + a * vec3<double>(0.5, 0.7, 1.0);

  // Translate the [0,1] component values to the byte range [0,255].
  return color_gradient;
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

// Raytracing
void rt1(Canvas &canvas) {

  // Camera position is the origin of each ray.
  point3<double> camera_position = {0.0, 0.0, 0.0};

  // World
  scene<double> world;

  // Lol
  // for (int i = 0; i < 4; i++) {
  //   world.add_hittable(
  //       make_shared<sphere<double>>(point3<double>(0.0, i + -1.0, -1.0), 1.0,
  //                                   color(255, 0, 255), 100, 0.2));
  // }
  // world.add_hittable(make_shared<sphere<double>>(
  //     point3<double>(1.0, -1.0, -1.0), 1.0, color(0, 0, 255), 100, 0.2));
  // world.add_hittable(make_shared<sphere<double>>(
  //     point3<double>(-1.0, -1.0, -1.0), 1.0, color(0, 0, 255), 100, 0.2));

  // Objects
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(0.0, -1.0, 3.0), 1.0, color(255, 0, 0), 500, 0.8)); // RED
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(2.0, 0.0, 4.0), 1.0, color(0, 0, 255), 500, 0.3)); // BLUE
  world.add_hittable(make_shared<sphere<double>>(
      point3<double>(-2.0, 0.0, 4.0), 1.0, color(0, 255, 0), 10, 0.4)); // GREEN
  world.add_hittable(
      make_shared<sphere<double>>(point3<double>(0.0, -5001.0, 0.0), 5000.0,
                                  color(255, 255, 0), 1000, 0.1)); // YELLOW

  // Lights
  world.add_light(
      make_shared<light<double>>(kAmbient, point3<double>(0, 0, 0), 0.2));
  world.add_light(
      make_shared<light<double>>(kPoint, point3<double>(2.0, 1.0, 0.0), 0.6));
  world.add_light(make_shared<light<double>>(
      kDirectonal, point3<double>(1.0, 4.0, 4.0), 0.2));

  // Renderer
  for (int x : std::ranges::iota_view(0, SCREEN_WIDTH)) {
    for (int y : std::ranges::iota_view(0, SCREEN_HEIGHT)) {

      point3<double> viewport_point = screen_to_viewport(x, y);
      vec3<double> ray_direction = normalize(viewport_point - camera_position);

      // Raypoints P = camera_position + t*ray_direction
      ray<double> r(camera_position, ray_direction);
      color pixel_color = create_color_from_vec(color_ray(r, world, 3));
      canvas.set_pixel(x, y, pixel_color);
    }
  }
}
