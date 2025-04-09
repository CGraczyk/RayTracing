#pragma once

#include "config.hpp"

struct Material {
  Color m_surfaceColor{};
  int m_specular = 0;
  double m_reflectiveness = 0.0;
  double m_transparency = 1.0;
  double m_refractionIndex = 1.0;
};

template <typename T> struct HitRecord {
  Point3<T> m_hitPoint;
  Vec3<T> m_normal;
  std::shared_ptr<Material> m_material;
  T m_t;
  bool m_frontFace;

  const Color &surfaceColor() const { return m_material->m_surfaceColor; }
  const int specular() const { return m_material->m_specular; }
  const double reflectiveness() const { return m_material->m_reflectiveness; }
  const double transparency() const { return m_material->m_transparency; }
  const double refractionIndex() const { return m_material->m_refractionIndex; }

  void set_faceNormal(const Ray<T> &r, const Vec3<T> &outwardNormal) {
    m_frontFace = dot(r.direction(), outwardNormal) <= 0;
    m_normal = m_frontFace ? outwardNormal : -outwardNormal;
  }

  void set_transparency(double transparency) {
    if (!(m_material->m_transparency > 1.0))
      m_material->m_transparency = transparency;
  }
  void set_reflectiveness(double reflectiveness) {
    if (!(m_material->m_transparency > 1.0))
      m_material->m_reflectiveness = reflectiveness;
  }
};

template <typename T> class Hittable {
public:
  virtual ~Hittable() = default;
  virtual bool hit(const Ray<T> &r, T ray_tmin, T ray_tmax,
                   HitRecord<T> &record) const = 0;
};
