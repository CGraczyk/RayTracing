#pragma once

#include "config.hpp"

template <typename T> class hit_record {
public:
  point3<T> p;
  vec3<T> normal;
  color surface_color;
  T t;
  bool front_face;
};

template <typename T> class hittable {
public:
  virtual ~hittable() = default;
  virtual bool hit(const ray<T> &r, T ray_tmin, T ray_tmax,
                   hit_record<T> &h_record) const = 0;
};
