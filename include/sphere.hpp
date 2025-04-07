#pragma once

#include "config.hpp"
#include "hittable.hpp"

template <typename T> class Sphere : public Hittable<T> {
public:
  Sphere(const Point3<T> &center, T radius, shared_ptr<Material> mat)
      : m_center(center), m_radius(std::fmax(0.0, radius)), m_material(mat) {}

  bool hit(const Ray<T> &r, T ray_tmin, T ray_tmax,
           HitRecord<T> &h_record) const override {

    Vec3<T> camera_orientation = m_center - r.origin();
    // Calculate ray hitting sphere by PQ formula from at^2 + bt + c = 0:
    auto a = r.direction().length_squared();
    auto b = dot(camera_orientation, r.direction()); // factor 2 canceled out
    auto c = camera_orientation.length_squared() - (m_radius * m_radius);

    // {t_1,t_2} = -b +- sqrt(b^2 - a*c)/a is solvable if
    auto discriminant = (b * b) - a * c;
    if (discriminant < 0.0)
      return false;

    // Ensure correct parameter space for t.
    // (behind camera t<0; 0<=t <=1 behind projection plane; t>1 in front)
    auto sqrtd = std::sqrt(discriminant);
    auto root = (b - sqrtd) / a;
    if (root <= ray_tmin or ray_tmax <= root) {
      root = (b + sqrtd) / a;
      if (root <= ray_tmin or ray_tmax <= root)
        return false;
    }

    h_record.m_t = root;
    h_record.m_hitPoint = r.at(root);
    Vec3<T> outward_normal = (h_record.m_hitPoint - m_center) / m_radius;
    h_record.set_faceNormal(r, outward_normal);
    h_record.m_material = m_material;

    return true;
  }

private:
  Point3<T> m_center;
  T m_radius;
  shared_ptr<Material> m_material;
};