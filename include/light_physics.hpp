
#pragma once

#include "config.hpp"
#include "hittable.hpp"
#include "scene.hpp"

inline Vec3<double> reflected(const Vec3<double> &direction,
                              const Vec3<double> &normal) {
  return 2.0 * normal * dot(normal, direction) - direction;
}

// Whiteness multiplier calculated from the visible lightsources in the scene.
double albedo(const Ray<double> &ray, const Scene<double> &world,
              const Point3<double> &camera_position,
              const HitRecord<double> &record) {

  double albedo = 0.0;
  double ambient = 0.0;
  Vec3<double> direction;
  double t_max = 0.0;

  for (const auto &lightsource : world.m_lights) {
    if (lightsource->m_type == kAmbient) {
      ambient += lightsource->m_intensity;
    } else {
      if (lightsource->m_type == kPoint) {
        direction = lightsource->m_position - record.m_hitPoint;
        t_max = 1;
      } else {
        direction = lightsource->m_position;
        t_max = INFINITY;
      }

      // Shadowcheck: If the shadowray does not hit an object or the object is
      // transparent, light needs to be computed from the current lightsource.
      // Thus "no hit and transparent true" should compute light.
      HitRecord<double> shadowRecord;
      Ray<double> shadowRay(ray.at(record.m_t), direction, 0);

      if (!world.hit(shadowRay, 0.00001, t_max, shadowRecord)) {

        // Diffusion
        double diffusion = dot(record.m_normal, direction) /
                           (record.m_normal.length() * direction.length());
        if (dot(record.m_normal, direction) > 0) {
          albedo += lightsource->m_intensity * diffusion;
        }

        // Specular
        if (record.specular() != -1) {

          Vec3<double> reflection = reflected(direction, record.m_normal);

          double reflectedDiffusion =
              dot(reflection, -ray.direction()) /
              (reflection.length() * -ray.direction().length());

          if (dot(reflection, -ray.direction()) > 0) {
            albedo += lightsource->m_intensity *
                      pow(reflectedDiffusion, record.specular());
          }
        }
      }
    }
  }
  return albedo + ambient;
}
