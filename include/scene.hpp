#pragma once

#include "hittable.hpp"

// avoid having to specify std::__ everytime
using std::make_shared;
using std::shared_ptr;

template <typename T> class scene : public hittable<T> {
public:
  std::vector<shared_ptr<hittable<T>>> objects;

  scene() {}
  scene(shared_ptr<hittable<T>> object) { add(object); }

  void clear() { objects.clear(); }

  void add(shared_ptr<hittable<T>> object) { objects.push_back(object); }

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
