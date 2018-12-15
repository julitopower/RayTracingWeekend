#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector.hpp>
#include <ray.hpp>

namespace rt {
class Camera {
 public:
  Camera(Vector3f lookfrom, Vector3f lookat, Vector3f vup,
         float vfov, float aspect,
         float aperture, float focus_dist) {
    lens_radius_ = aperture / 2;
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
    w_ = unit_vector(lookfrom - lookat);
    u_ = unit_vector(cross(vup, w_));
    v_ = cross(w_, u_);
    // Notice how the projection plane is in z = -1, and the origin z = 0
    lower_left_corner_ = origin_ - half_width * focus_dist *  u_
                         - half_height *  focus_dist * v_ - focus_dist * w_;
    horizontal_span_ = 2 * half_width * focus_dist * u_;
    vertical_span_ = 2 * half_height * focus_dist * v_;
  }
  Ray ray(float u, float v) const {
    Vector3f rd = lens_radius_ * random_in_unit_disk();
    Vector3f offset = u_ * rd.x() + v_ * rd.y();
    return Ray{origin_ + offset, lower_left_corner_ +
          u * horizontal_span_ + v * vertical_span_ - origin_ - offset};
  }
 private:
  rt::Vector3f origin_;
  rt::Vector3f horizontal_span_;
  rt::Vector3f vertical_span_;
  rt::Vector3f lower_left_corner_;
  rt::Vector3f u_, v_, w_;
  float lens_radius_;
};
}

#endif // CAMERA_HPP
