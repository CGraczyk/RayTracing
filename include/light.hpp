#pragma once

#include "config.hpp"

enum lightType {
  kAmbient = 0,
  kPoint = 1,
  kDirectonal = 2,
};

template <typename T> class Light {
public:
  enum lightType m_type;
  Point3<T> m_position;
  T m_intensity;

  Light(enum lightType type, const Point3<T> &position, T intensity)
      : m_type(type), m_position(position), m_intensity(intensity) {}
};