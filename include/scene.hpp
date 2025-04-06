#pragma once

#include "hittable.hpp"

// avoid having to specify std::__ everytime
using std::make_shared;
using std::shared_ptr;

template <typename T> class Scene : public Hittable<T> {
public:
  std::vector<shared_ptr<Hittable<T>>> m_objects;
  std::vector<shared_ptr<Light<T>>> m_lights;

  Scene() {}
  Scene(shared_ptr<Hittable<T>> object) { add_hittable(object); }
  Scene(shared_ptr<Light<T>> light) { add_light(light); }

  void clear_objects() { m_objects.clear(); }

  void clear_lights() { m_lights.clear(); }

  void add_hittable(shared_ptr<Hittable<T>> object) {
    m_objects.push_back(object);
  }
  void add_light(shared_ptr<Light<T>> light) { m_lights.push_back(light); }

  // Whiteness multiplier from the visible lightsources in the scene.
  T albedo(const Ray<T> &r, const Point3<T> &camera_position,
           const HitRecord<T> &record) const {

    T albedo = 0.0;
    Vec3<T> direction;
    T t_max = 0.0;

    for (const auto &lightsource : m_lights) {
      if (lightsource->m_type == kAmbient) {
        albedo += lightsource->m_intensity;
      } else {
        if (lightsource->m_type == kPoint) {
          direction = lightsource->m_position - record.m_hitPoint;
          t_max = 1;
        } else {
          direction = lightsource->m_position;
          t_max = INFINITY;
        }

        // Shadowcheck
        HitRecord<T> shadowRecord;
        Ray<T> shadowRay(r.at(record.m_t), direction, 0);
        if (!hit(shadowRay, 0.001, t_max, shadowRecord)) {

          // Diffusion
          T diffusion = dot(record.m_normal, direction) /
                        (record.m_normal.length() * direction.length());
          if (diffusion > 0) {
            albedo += lightsource->m_intensity * diffusion;
          }

          // Specular
          if (record.specular() != -1) {

            Vec3<T> reflection = reflected(direction, record.m_normal);

            auto reflectedDiffusion =
                dot(reflection, -r.direction()) /
                (reflection.length() * -r.direction().length());

            if (reflectedDiffusion > 0) {
              albedo += lightsource->m_intensity *
                        pow(reflectedDiffusion, record.specular());
            }
          }
        }
      }
    }
    return albedo;
  }

  bool hit(const Ray<T> &r, T ray_tmin, T ray_tmax,
           HitRecord<T> &rec) const override {
    HitRecord<T> temp_rec;
    bool hit_anything = false;
    auto current_closest = ray_tmax;

    for (const auto &object : m_objects) {
      if (object->hit(r, ray_tmin, current_closest, temp_rec)) {
        hit_anything = true;
        current_closest = temp_rec.m_t;
        rec = temp_rec;
      }
    }
    return hit_anything;
  }
};
