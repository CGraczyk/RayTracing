#pragma once

#include "vec3.hpp"

template <typename T> class ray {
public:
  ray() {}

  ray(const point3<T> &origin, const vec3<T> &direction)
      : origin_(origin), direction_(direction) {}

  const point3<T> &origin() const { return origin_; }
  const vec3<T> &direction() const { return direction_; }

  point3<T> at(auto t) const { return origin_ + t * direction_; }

private:
  point3<T> origin_;
  vec3<T> direction_;
};