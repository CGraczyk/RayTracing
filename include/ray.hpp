#pragma once

#include "vec3.hpp"

template <typename T> class Ray {
public:
  Ray() {}

  Ray(const Point3<T> &origin, const Vec3<T> &direction, int bounces)
      : m_origin(origin), m_direction(direction), m_bounces(bounces) {
    m_refractionIndex = 1.0;
  }

  const Point3<T> &origin() const { return m_origin; }
  const Vec3<T> &direction() const { return m_direction; }

  const Point3<T> at(T t) const { return m_origin + t * m_direction; }
  int bounces() const { return m_bounces; }

  const double refractionIndex() const { return m_refractionIndex; }

  void set_refractionIndex(double refractionIndex) {
    m_refractionIndex = refractionIndex;
  }

private:
  Point3<T> m_origin;
  Vec3<T> m_direction;
  int m_bounces;
  double m_refractionIndex;
};