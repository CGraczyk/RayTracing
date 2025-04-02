#pragma once

#include "config.hpp"

class camera {
public:
  camera(point3<double> position_, double yaw_, double pitch_, double roll_)
      : position(position_), yaw(yaw_), pitch(pitch_), roll(roll_) {}

  // This function applies a 3D rotation to the vector `v` based on the given
  // yaw,
  // pitch, and roll angles.
  // - pitch (rotation around Z-axis): rotates the vector in the Y-Z plane
  // (up-down direction).
  // - yaw (rotation around Y-axis): rotates the vector in the X-Z plane
  // ((left-right).
  // - roll (rotation around X-axis): rolls the vector around the own Z-axis
  // (rotate in the X-Y plane).
  // Example values:
  //    yaw = pi / 6 <=> rotate to the right by 30°,
  //    pitch = pi / 3 <=> rotate down by 60°,
  //    roll = pi / 4 <=> rotate counterclockwise by 45°.

  vec3<double> rotate3D(const vec3<double> &v, double pitch, double yaw,
                        double roll) {
    double cr = cos(pitch),
           sr = sin(pitch); // Cosine and sine of roll (Z-axis rotation)
    double cp = cos(yaw),
           sp = sin(yaw); // Cosine and sine of pitch (X-axis rotation)
    double cy = cos(roll),
           sy = sin(roll); // Cosine and sine of yaw (Y-axis rotation)

    return {v[0] * (cy * cp) + v[1] * (cy * sp * sr - sy * cr) +
                v[2] * (cy * sp * cr + sy * sr),
            v[0] * (sy * cp) + v[1] * (sy * sp * sr + cy * cr) +
                v[2] * (sy * sp * cr - cy * sr),
            v[0] * (-sp) + v[1] * (cp * sr) + v[2] * (cp * cr)};
  }

  ray<double> create_ray_to_pixel(int x, int y, double offset_x,
                                  double offset_y) {
    // Center the screen at (0,0) and flip Y-axis
    double Cx = x - (SCREEN_WIDTH / 2.0);
    double Cy = (SCREEN_HEIGHT / 2.0) - y;

    // Transform centered coordinates to viewport coordinates in worldspace at
    // projection plane at Z = 1 from camera origin
    double pixel_size_x = VIEWPORT_WIDTH / SCREEN_WIDTH;
    double pixel_size_y = VIEWPORT_HEIGHT / SCREEN_HEIGHT;

    // Viewport coordinates
    double Vx = (Cx + offset_x) * pixel_size_x;
    double Vy = (Cy + offset_y) * pixel_size_y;
    double Vz = FOCAL_LENGTH;

    vec3<double> direction = normalize(vec3<double>(Vx, Vy, Vz));

    return ray<double>(position, rotate3D(direction, pitch, yaw, roll));
  }

  point3<double> position;
  double yaw;
  double pitch;
  double roll;
};