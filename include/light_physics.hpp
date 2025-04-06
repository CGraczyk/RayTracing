
#pragma once

#include "config.hpp"
#include "hittable.hpp"

inline Vec3<double> reflect_direction(const Vec3<double> &direction,
                                      const Vec3<double> &normal) {
  return 2.0 * normal * dot(normal, direction) - direction;
}