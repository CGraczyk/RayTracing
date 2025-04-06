#pragma once

#include "config.hpp"

template <typename T> class Vec3 {
public:
  T e[3];

  // default constructor [0,0,0]
  Vec3() : e{0, 0, 0} {}

  // parametrized constructor to construct with vec v(1.0,2.0,3.0)
  Vec3(T x, T y, T z) : e{x, y, z} {}

  // Accessors x,y,z
  T x() const { return e[0]; }
  T y() const { return e[1]; }
  T z() const { return e[2]; }

  // Negation operator -v
  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

  // Indexing, const for read, non-const for modification, avoid copy by
  // returning references.
  const T &operator[](int i) const { return e[i]; }
  T &operator[](int i) { return e[i]; }

  // Compound Assignment
  Vec3 &operator+=(const Vec3 &other) {
    e[0] += other.e[0];
    e[1] += other.e[1];
    e[2] += other.e[2];
    return *this;
  }

  Vec3 &operator*=(T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3 &operator/=(T t) { return *this *= T(1.0) / t; }

  // utility functions
  T length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

  T length() const { return std::sqrt(length_squared()); }

  bool is_zero() const {
    return length_squared() < 1e-8; // Avoids precision issues
  }
};

// Alias for clarifying positions vs vectors.
template <typename T> using Point3 = Vec3<T>;

// Alias for clarifying color as vectors.
using Color = Vec3<int>;

// Vector Utility Functions
// Operator Overload

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Vec3<T> &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline Vec3<T> operator+(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template <typename T> inline Vec3<T> operator*(const Vec3<T> &v, T t) {
  return Vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

template <typename T> inline Vec3<T> operator*(T t, const Vec3<T> &v) {
  return Vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

template <typename T> inline Vec3<T> operator/(const Vec3<T> &v, T t) {
  return Vec3<T>(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

template <typename T> inline Vec3<T> normalize(const Vec3<T> &v) {
  return v / v.length();
}

template <typename T> inline T distance(const Vec3<T> &u, const Vec3<T> &v) {
  return (u - v).length();
}

template <typename T>
inline Vec3<T> reflected(const Vec3<T> &direction, const Vec3<T> &normal) {
  return 2.0 * normal * dot(normal, direction) - direction;
}

// Compute the angle theta between vectors by dot(u,v) = u.length()* v.length()
// * cos(theta)
template <typename T> inline T dot(const Vec3<T> &u, const Vec3<T> &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Cross Product to compute the normal of plane:
// u[1]*v[2] - u[2]*v[1]
// u[2]*v[0] - u[0]*v[2]
// u[0]*v[1] - u[1]*v[0]
// The magnitude of the Cross Product is the area spanned by the vectors.
template <typename T> inline Vec3<T> cross(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                 u.e[2] * v.e[0] - u.e[0] * v.e[2],
                 u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}