#pragma once

#include "config.hpp"
#include "hittable.hpp"

template <typename T> class sphere : public hittable<T> {
public:
  sphere(const point3<T> &center, T radius, color color_)
      : center(center), radius(std::fmax(0.0, radius)), surface_color(color_) {}

  bool hit(const ray<T> &r, T ray_tmin, T ray_tmax,
           hit_record<T> &h_record) const override {

    vec3<T> camera_orientation = center - r.origin();
    // Calculate ray hitting sphere by PQ formula from at^2 + bt + c = 0:
    auto a = r.direction().length_squared();
    auto b = dot(camera_orientation, r.direction()); // factor 2 canceled out
    auto c = camera_orientation.length_squared() - (radius * radius);

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

    h_record.t = root;
    h_record.p = r.at(root);
    h_record.normal = (h_record.p - center) / radius;
    h_record.surface_color = surface_color;

    return true;
  }

private:
  point3<T> center;
  T radius;
  color surface_color;
};