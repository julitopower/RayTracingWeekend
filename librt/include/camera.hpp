#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector.hpp>
#include <ray.hpp>

namespace rt {
class Camera {
 public:
  Camera(Vector3f lookfrom, Vector3f lookat, Vector3f vup, float vfov, float aspect) {
    Vector3f u, v, w;
    // transorm degress to radians
    const float theta = vfov * M_PI / 180;
    ////////////////////////////////////////////////////////////////////////////////
    // This is because the "cosine"_side of the triangle has length 1
    //
    // tan = sin(x) / cos(x) = sin_side / cosine_side
    //
    // We are trying to calculate the sin_side, thus sin_side = tan(x) * 1
    ////////////////////////////////////////////////////////////////////////////////
    const float half_height = tan(theta / 2);
    const float half_width = aspect * half_height;

    origin_ = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    // Notice how the projection plane is in z = -1, and the origin z = 0
    lower_left_corner_ = origin_ - half_width * u - half_height * v - w;
    horizontal_span_ = 2 * half_width * u;
    vertical_span_ = 2 * half_height * v;
  }
  Ray ray(float u, float v) const {
    return Ray{origin_, lower_left_corner_ +
          u * horizontal_span_ + v * vertical_span_ - origin_};
  }
 private:
  rt::Vector3f origin_;
  rt::Vector3f horizontal_span_;
  rt::Vector3f vertical_span_;
  rt::Vector3f lower_left_corner_;
};
}

#endif // CAMERA_HPP
