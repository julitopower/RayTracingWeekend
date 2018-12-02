#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector.hpp>
#include <ray.hpp>

namespace rt {
class Camera {
 public:
  Ray ray(float u, float v) const {
    return rt::Ray{origin_, lower_left_corner_ +
          u * horizontal_span_ + v * vertical_span_}; 
  }
 private:
  const rt::Vector3f origin_{0.0, 0.0, 0.0};
  const rt::Vector3f horizontal_span_{4.0, 0.0, 0.0};
  const rt::Vector3f vertical_span_{0.0, 2.0, 0.0};
  const rt::Vector3f lower_left_corner_{-2.0, -1.0, -1.0};  
};
}

#endif // CAMERA_HPP
