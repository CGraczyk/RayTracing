#pragma once

#include "config.hpp"

enum light_type {
  kAmbient = 0,
  kPoint = 1,
  kDirectonal = 2,
};

template <typename T> class light {
public:
  enum light_type type;
  point3<T> position;
  T intensity;

  light(enum light_type type_, const point3<T> &position_, T intensity_)
      : type(type_), position(position_), intensity(intensity_) {}
};