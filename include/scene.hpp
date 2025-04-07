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
