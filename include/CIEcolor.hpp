#pragma once

#include "config.hpp"

// Normalize [0,1] to [380,740] nm wavelength
double normalizedToWavelength(double value) {
  return 380.0 + value * (740.0 - 380.0);
}

// CIE 1931 XYZ Approximation (Wyman et al., 2013)
vec3<double> cie1931WavelengthToXYZ(double wavelength) {
  double t1, t2, t3;
  double x, y, z;

  {
    t1 = (wavelength - 442.0) * ((wavelength < 442.0) ? 0.0624 : 0.0374);
    t2 = (wavelength - 599.8) * ((wavelength < 599.8) ? 0.0264 : 0.0323);
    t3 = (wavelength - 501.1) * ((wavelength < 501.1) ? 0.0490 : 0.0382);
    x = 0.362 * exp(-0.5 * t1 * t1) + 1.056 * exp(-0.5 * t2 * t2) -
        0.065 * exp(-0.5 * t3 * t3);
  }

  {
    t1 = (wavelength - 568.8) * ((wavelength < 568.8) ? 0.0213 : 0.0247);
    t2 = (wavelength - 530.9) * ((wavelength < 530.9) ? 0.0613 : 0.0322);
    y = 0.821 * exp(-0.5 * t1 * t1) + 0.286 * exp(-0.5 * t2 * t2);
  }

  {
    t1 = (wavelength - 437.0) * ((wavelength < 437.0) ? 0.0845 : 0.0278);
    t2 = (wavelength - 459.0) * ((wavelength < 459.0) ? 0.0385 : 0.0725);
    z = 1.217 * exp(-0.5 * t1 * t1) + 0.681 * exp(-0.5 * t2 * t2);
  }

  return {x, y, z};
}

// Convert XYZ to linear RGB
vec3<double> xyzToLinearRGB(const vec3<double> &xyz) {
  return {3.2406255 * xyz.x() + -1.537208 * xyz.y() + -0.4986286 * xyz.z(),
          -0.9689307 * xyz.x() + 1.8757561 * xyz.y() + 0.0415175 * xyz.z(),
          0.0557101 * xyz.x() + -0.2040211 * xyz.y() + 1.0569959 * xyz.z()};
}

// Gamma correction (linear to sRGB)
double gammaCorrect(double c) {
  c = std::clamp(c, 0.0, 1.0);
  return (c <= 0.0031308) ? c * 12.92 : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

// Convert normalized value [0,1] to RGB
color spectralToRGB(double normalizedValue) {
  double wavelength = normalizedToWavelength(normalizedValue);
  vec3<double> xyz = cie1931WavelengthToXYZ(wavelength);
  vec3<double> rgb = xyzToLinearRGB(xyz);

  color v = {static_cast<int>(gammaCorrect(rgb.x()) * 255.999),
             static_cast<int>(gammaCorrect(rgb.y()) * 255.999),
             static_cast<int>(gammaCorrect(rgb.z()) * 255.999)};

  return v;
}
