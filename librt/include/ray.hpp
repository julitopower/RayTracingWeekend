#ifndef RAY_HPP
#define RAY_HPP

#include <vector.hpp>

namespace rt {

/*
 * \brief A ray is essentially a straing line with a defined origin and direction
 *
 * Any point in the straight line can be calculated as p = base + dir * t, where
 * t is a parameter
 */
class Ray {
 public:
  explicit Ray(){}
  explicit Ray(const Vector3f& base, const Vector3f& dir) :
      base_{base}, dir_{dir} {}

  const Vector3f& origin() const { return base_; }
  const Vector3f& dir() const { return dir_; }
  Vector3f point_at(float t) {
    return base_ + t * dir_;
  }
 private:
  Vector3f base_;
  Vector3f dir_;
};
}

#endif // RAY_HPP
