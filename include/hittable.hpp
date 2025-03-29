#pragma once

#include "config.hpp"

template <typename T> class hit_record {
public:
  point3<T> p;
  vec3<T> normal;
  color surface_color;
  int specular;
  T t;
  bool front_face;

  void set_face_normal(const ray<T> &r, const vec3<T> &outward_normal) {
    // Sets the hit record normal vector.
    // Note: the parameter outward normal is expected to be unit length.

    front_face = dot(r.direction(), outward_normal) <= 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

template <typename T> class hittable {
public:
  virtual ~hittable() = default;
  virtual bool hit(const ray<T> &r, T ray_tmin, T ray_tmax,
                   hit_record<T> &h_record) const = 0;
};
