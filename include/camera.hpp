#pragma once

#include "config.hpp"

class Camera {
public:
  Camera(Point3<double> position, double yaw, double pitch, double roll)
      : m_position(position), m_yaw(yaw), m_pitch(pitch), m_roll(roll) {}

  // This function applies a 3D rotation to the vector `v` based on the given
  // yaw, pitch, and roll angles.
  // - yaw (rotation around Y-axis): rotates the vector in the X-Z plane
  // (( left-right ).
  // - pitch (rotation around X-axis): rotates the vector in the Y-Z plane
  // ( up-down ).
  // - roll (rotation around Z-axis): rotates the vector in the X-Y plane.
  // ( counter-/clockwise )
  // Example values:
  //    yaw = pi / 6 <=> rotate to the right by 30°,
  //    pitch = pi / 3 <=> rotate down by 60°,
  //    roll = pi / 4 <=> rotate counterclockwise by 45°.

  Vec3<double> rotate3D(const Vec3<double> &v) {

    double c_yaw = cos(m_roll),
           s_yaw = sin(m_roll); // Cosine and sine of yaw (Y-axis rotation)
    double c_pitch = cos(m_yaw),
           s_pitch = sin(m_yaw); // Cosine and sine of pitch (X-axis rotation)
    double c_roll = cos(m_pitch),
           s_roll = sin(m_pitch); // Cosine and sine of roll (Z-axis rotation)

    return {v[0] * (c_yaw * c_pitch) +
                v[1] * (c_yaw * s_pitch * s_roll - s_yaw * c_roll) +
                v[2] * (c_yaw * s_pitch * c_roll + s_yaw * s_roll),
            v[0] * (s_yaw * c_pitch) +
                v[1] * (s_yaw * s_pitch * s_roll + c_yaw * c_roll) +
                v[2] * (s_yaw * s_pitch * c_roll - c_yaw * s_roll),
            v[0] * (-s_pitch) + v[1] * (c_pitch * s_roll) +
                v[2] * (c_pitch * c_roll)};
  }

  Ray<double> create_pixelRay(int x, int y, double offset_x, double offset_y) {
    // Center the sc_rolleen at (0,0) and flip Y-axis
    double cam_x = x - (SCREEN_WIDTH / 2.0);
    double cam_y = (SCREEN_HEIGHT / 2.0) - y;

    // Transform centered coordinates to viewport coordinates in worlds_pitchace
    // at projection plane at Z = 1 from camera origin
    double pixel_size_x = VIEWPORT_WIDTH / SCREEN_WIDTH;
    double pixel_size_y = VIEWPORT_HEIGHT / SCREEN_HEIGHT;

    // Viewport coordinates
    double Vx = (cam_x + offset_x) * pixel_size_x;
    double Vy = (cam_y + offset_y) * pixel_size_y;
    double Vz = FOCAL_LENGTH;

    Vec3<double> direction = normalize(Vec3<double>(Vx, Vy, Vz));

    return Ray<double>(m_position, rotate3D(direction), N_REFLECTIONS);
  }

  const Point3<double> &position() const { return m_position; }

private:
  Point3<double> m_position;
  double m_yaw;
  double m_pitch;
  double m_roll;
};