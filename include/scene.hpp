#pragma once

#include "hittable.hpp"

// avoid having to specify std::__ everytime
using std::make_shared;
using std::shared_ptr;

template <typename T> class scene : public hittable<T> {
public:
  std::vector<shared_ptr<hittable<T>>> objects;
  std::vector<shared_ptr<light<T>>> lights;

  scene() {}
  scene(shared_ptr<hittable<T>> object) { add_hittable(object); }
  scene(shared_ptr<light<T>> light) { add_light(light); }

  void clear() { objects.clear(); }

  void add_hittable(shared_ptr<hittable<T>> object) {
    objects.push_back(object);
  }
  void add_light(shared_ptr<light<T>> light) { lights.push_back(light); }

  T light_multiplier(const ray<T> &r, const point3<T> &camera_position,
                     const hit_record<T> &record) const {

    T intensity = 0.0;
    vec3<T> lightray_direction;
    T t_max = 0.0;

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
        hit_record<T> shadow_record;
        ray<T> shadow_r(r.at(record.t), lightray_direction);
        if (!hit(shadow_r, 0.001, t_max, shadow_record)) {

          // Diffusion
          T normal_dot_lightray = dot(record.normal, lightray_direction);
          if (normal_dot_lightray > 0) {
            intensity += lightsource->intensity * normal_dot_lightray /
                         (record.normal.length() * lightray_direction.length());
          }

          // Specular
          if (record.specular != -1) {
            vec3<T> viewpoint = -r.direction();
            vec3<T> reflection =
                reflect_direction(lightray_direction, record.normal);

            auto reflection_dot_viewpoint = dot(reflection, -r.direction());

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

  bool hit(const ray<T> &r, T ray_tmin, T ray_tmax,
           hit_record<T> &rec) const override {
    hit_record<T> temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_tmax;

    for (const auto &object : objects) {
      if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }
    return hit_anything;
  }
};
