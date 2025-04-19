#pragma once

#include "camera.hpp"
#include "config.hpp"
#include "hittable.hpp"
#include "inv_sphere.hpp"
#include "light_physics.hpp"
#include "sphere.hpp"

Vec3<double> trace(const Ray<double> &ray, const Scene<double> &world,
                   const Point3<double> &cam_position) {
  HitRecord<double> record;

  // Collision test for objects in the world.
  if (world.hit(ray, 0.0001, INFINITY, record)) {
    Vec3<double> local_color = {record.surfaceColor().x() / 255.0,
                                record.surfaceColor().y() / 255.0,
                                record.surfaceColor().z() / 255.0};

    // Compute whiteness multiplier.
    local_color *= albedo(ray, world, cam_position, record);

    // Compute transparency.
    if (record.transparency() > 0.0) {

      Ray<double> transparent_ray(ray.at(record.m_t), ray.direction(),
                                  ray.bounces());

      // Compute refraction from material.
      if (ray.refractionIndex() != record.refractionIndex()) {

        double n = ray.refractionIndex() / record.refractionIndex();
        const double cosI = -dot(record.m_normal, normalize(ray.direction()));
        const double sinT2 = n * n * (1.0 - cosI * cosI);
        if (sinT2 <= 1.0) {

          const double cosT = sqrt(1.0 - sinT2);
          Vec3<double> refractedDirection = n * normalize(ray.direction()) +
                                            (n * cosI - cosT) * record.m_normal;

          transparent_ray = Ray<double>(ray.at(record.m_t), refractedDirection,
                                        ray.bounces());
          transparent_ray.set_refractionIndex(record.refractionIndex());
        }
      }
      // Compute transparent ray color
      Vec3<double> transparent_color =
          trace(transparent_ray, world, cam_position);

      local_color = local_color * (1.0 - record.transparency()) +
                    transparent_color * record.transparency();
    }

    // Compute surface reflections after first transparent ray split.
    if (ray.bounces() > 0 and record.reflectiveness() > 0.0) {

      // Compute surface reflections.
      Ray<double> reflectedRay(ray.at(record.m_t),
                               reflected(-ray.direction(), record.m_normal),
                               ray.bounces() - 1);

      Vec3<double> reflection_color = trace(reflectedRay, world, cam_position);

      return local_color * (1.0 - record.reflectiveness()) +
             reflection_color * record.reflectiveness();
    }

    return local_color;
  }

  // Create Background Sky Gradient.
  auto background_gradient = 0.5 * (normalize(ray.direction()).y() + 1.0);
  return (1.0 - background_gradient) * Vec3<double>(1.0, 1.0, 1.0) +
         background_gradient * Vec3<double>(0.5, 0.7, 1.0);
}

inline Point3<double> pull(const Vec3<double> &obj, const Vec3<double> &well,
                           double k) {
  return k * (well - obj) + obj;
}

void populate_scene(Scene<double> &world) {

  // Create object materials
  auto material_red =
      make_shared<Material>(Color(255, 0, 0), 100, 0.5, 0.1, 1.0);
  auto material_blue =
      make_shared<Material>(Color(0, 0, 255), 500, 0.5, 0.1, 1.0);
  auto material_green =
      make_shared<Material>(Color(0, 255, 0), 10, 0.5, 0.1, 1.0);

  auto material_ground =
      make_shared<Material>(Color(255, 255, 0), 1000, 0.0, 0.1, 1.0);

  auto material_refractedGlass =
      make_shared<Material>(Color(255, 255, 255), 1000, 0.9, 0.1, 1.5);
  auto material_mirror =
      make_shared<Material>(Color(255, 255, 255), 1000, 1.0, 1.0, 1.0);

  // Origin
  // world.add_hittable(make_shared<Sphere<double>>(
  //     Point3<double>(0.0, 0.0, 0.0), 0.01,
  //     make_shared<Material>(Color(0, 0, 0), 0, 0.0, 0.0, 1.0)));

  // Objects
  world.add_hittable(make_shared<Sphere<double>>(Point3<double>(0.0, 0.0, 1.0),
                                                 1.0, material_red));
  world.add_hittable(make_shared<Sphere<double>>(Point3<double>(2.3, 1.0, 3.0),
                                                 1.0, material_blue));
  world.add_hittable(make_shared<Sphere<double>>(Point3<double>(-2.5, 1.0, 3.5),
                                                 1.0, material_green));
  world.add_hittable(make_shared<Sphere<double>>(
      Point3<double>(0.0, -5000.0, 0.0), 5000.0, material_ground));

  world.add_hittable(make_shared<Sphere<double>>(Point3<double>(0.0, 2.0, 9.0),
                                                 2.0, material_refractedGlass));
  world.add_hittable(make_shared<InvSphere<double>>(
      Point3<double>(0.0, 1.0, 3.5), 0.8, material_mirror));

  // Lights
  world.add_light(
      make_shared<Light<double>>(kAmbient, Point3<double>(0, 0, 0), 0.2));
  world.add_light(
      make_shared<Light<double>>(kPoint, Point3<double>(-4.0, 8.0, -1.0), 0.6));
  world.add_light(make_shared<Light<double>>(
      kDirectonal, Point3<double>(40.0, 200.0, -15.0), 0.4));
}

// Raytracing
void raytracer(Canvas &canvas) {

  // Camera position for the ray origin.
  double zoom = 1;
  Camera cam(Point3<double>(0.0, 2.0, 1.0), 0, 22.5, 0);

  // World init
  Scene<double> world;
  populate_scene(world);

  std::vector<int> iter_aa(N_ANTIALIASING);
  for (uint32_t i = 0; i < N_ANTIALIASING; i++)
    iter_aa[i] = i;

  // Logging
  std::clog << "\r Start Raytracer.                 \n";
  ProgressBar bar(SCREEN_WIDTH * SCREEN_HEIGHT);

  // Renderer
  size_t step = 0;

#if MultiThreading

  for (int x : std::ranges::iota_view(0, SCREEN_WIDTH)) {
    for (int y : std::ranges::iota_view(0, SCREEN_HEIGHT)) {
      Vec3<double> pixel_color(0.0, 0.0, 0.0);

      // Supersampling
      if (N_ANTIALIASING > 0) {
        std::for_each(std::execution::par_unseq, iter_aa.begin(), iter_aa.end(),
                      [&](uint8_t i) {
                        std::for_each(
                            std::execution::par_unseq, iter_aa.begin(),
                            iter_aa.end(), [&, i](uint32_t j) {
                              // Grid-based jitter within pixel in range [-1,1]
                              double offset_x =
                                  ((i + 0.5) / N_ANTIALIASING - 0.5) * 2;
                              double offset_y =
                                  ((j + 0.5) / N_ANTIALIASING - 0.5) * 2;

                              // Create supersampled ray for each pixel on the
                              // screen
                              Ray<double> r =
                                  cam.create_pixelRay(x, y, offset_x, offset_y);
                              // Compute the color of the pixel by raytracing
                              // the scene
                              pixel_color += trace(r, world, cam.position());
                            });
                      });
        pixel_color /= N_ANTIALIASING * N_ANTIALIASING;
      } else {
        Ray<double> ray = cam.create_pixelRay(x, y, 0.0, 0.0);
        // Compute the color of the pixel by raytracing the scene
        pixel_color += trace(ray, world, cam.position());
      }

      // Compute the color of the pixel by raytracing the scene
      canvas.set_pixel(x, y, pixel_color);
      bar.update(++step);
    }
  }
  bar.done();
  std::clog << "\rDone.                 \n";
  return;
#endif

  for (int x : std::ranges::iota_view(0, SCREEN_WIDTH)) {
    for (int y : std::ranges::iota_view(0, SCREEN_HEIGHT)) {
      Vec3<double> pixel_color(0.0, 0.0, 0.0);

      // Supersampling
      if (N_ANTIALIASING > 0) {
        for (int i = 0; i < N_ANTIALIASING; i++) {
          for (int j = 0; j < N_ANTIALIASING; j++) {

            // Grid-based jitter within pixel in range [-1,1]
            double offset_x = ((i + 0.5) / N_ANTIALIASING - 0.5) * 2;
            double offset_y = ((j + 0.5) / N_ANTIALIASING - 0.5) * 2;

            // Create supersampled ray for each pixel on the screen
            Ray<double> r = cam.create_pixelRay(x, y, offset_x, offset_y);
            // Compute the color of the pixel by raytracing the scene
            pixel_color += trace(r, world, cam.position());
          }
        }
        pixel_color /= N_ANTIALIASING * N_ANTIALIASING;
      } else {
        Ray<double> ray = cam.create_pixelRay(x, y, 0.0, 0.0);
        // Compute the color of the pixel by raytracing the scene
        pixel_color += trace(ray, world, cam.position());
      }

      // Compute the color of the pixel by raytracing the scene
      canvas.set_pixel(x, y, pixel_color);
      bar.update(++step);
    }
  }
  bar.done();
  std::clog << "\rDone.                 \n";
}
