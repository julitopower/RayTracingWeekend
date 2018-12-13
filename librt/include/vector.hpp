#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <math.h>
#include <random>
#include <memory>

namespace rt {

class Vector3f {
 public:
  /*!
   * \brief Default constructor
   */
  Vector3f() {}
  
  /*!
   * \brief Initializes the vector components
   */
  Vector3f(float v0, float v1, float v2) : v_{v0, v1, v2} {}

  // View components as coordinates
  const float& x() const { return v_[0]; }
  const float& y() const { return v_[1]; }
  const float& z() const { return v_[2]; }

  // View components as colors
  const float& r() const { return v_[0]; }
  const float& g() const { return v_[1]; }
  const float& b() const { return v_[2]; }

  /*!
   * \brief Prefix+ is basically a no-op
   */
  const Vector3f& operator+() {return *this; }
  /*
   * \brief Negate vector
   */
  Vector3f operator-() { return Vector3f(-v_[0], -v_[1], -v_[2]); }

  /*
   * \brief Indexing operator
   *
   * Bounds are not checked
   */
  float operator[](int i) const { return v_[i]; };
  float& operator[](int i)  { return v_[i]; };

  // More mutating operators
  Vector3f& operator+=(const Vector3f& rhs);
  Vector3f& operator-=(const Vector3f& rhs);
  Vector3f& operator*=(const Vector3f& rhs);
  Vector3f& operator/=(const Vector3f& rhs);
  Vector3f& operator*=(float t);
  Vector3f& operator/=(float t);

  /*
   * \brief Vector Norm2, or length in a euclidean space
   */
  float norm2() {
    return ::sqrt(squared_length());
  }

  /*
   * \brief Square of length
   */
  float squared_length() {
    return (v_[0] * v_[0]) + (v_[1] * v_[1]) + (v_[2] * v_[2]);
  }

  void sqrt() {
    v_[0] = ::sqrt(v_[0]);
    v_[1] = ::sqrt(v_[1]);
    v_[2] = ::sqrt(v_[2]);
  }
  
  /*
   * \brief ???
   */
  void make_unit_vector();

  friend std::istream& operator>>(std::istream& is, Vector3f& v);
  friend std::ostream& operator<<(std::ostream& os, const Vector3f& v);
 private:
  float v_[3];
};

inline std::istream& operator>>(std::istream& is, Vector3f& v) {
  is >> v.v_[0] >> v.v_[1] >> v.v_[2];
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3f& v) {
  os << v.x() << " " << v.y() << " " << v.z();
  return os;
}

inline void Vector3f::make_unit_vector() {
    const auto k = float{1.0f / norm2()};
    *this *= k;
}

inline Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs) {
  return Vector3f{lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z()};
}

inline Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs) {
  return Vector3f{lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z()};
}

inline Vector3f operator*(const Vector3f& lhs, const Vector3f& rhs) {
  return Vector3f{lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z()};
}

inline Vector3f operator/(const Vector3f& lhs, const Vector3f& rhs) {
  return Vector3f{lhs.x() / rhs.x(), lhs.y() / rhs.y(), lhs.z() / rhs.z()};
}

inline Vector3f operator*(const Vector3f& lhs, float t) {
  return Vector3f{lhs.x() * t, lhs.y() * t, lhs.z() * t};
}

inline Vector3f operator/(const Vector3f& lhs, float t) {
  return Vector3f{lhs.x() / t, lhs.y() / t, lhs.z() / t};
}

inline Vector3f operator*(float t, const Vector3f& rhs) {
  return Vector3f{rhs.x() * t, rhs.y() * t, rhs.z() * t};
}


inline float dot(const Vector3f& lhs, const Vector3f& rhs) {
  return (lhs.x() * rhs.x()) + (lhs.y() * rhs.y()) + (lhs.z() * rhs.z());
}

inline Vector3f cross(const Vector3f& lhs, const Vector3f& rhs) {
  return Vector3f{
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        -(lhs.x() * rhs.z() - lhs.z() * rhs.x()),
        lhs.x() * rhs.y() - lhs.y() * rhs.x()
  };
}

inline Vector3f& Vector3f::operator+=(const Vector3f &rhs) {
  v_[0] += rhs.x();
  v_[1] += rhs.y();
  v_[2] += rhs.z();
  return *this;
}

inline Vector3f& Vector3f::operator*=(const Vector3f &rhs) {
  v_[0] *= rhs.x();
  v_[1] *= rhs.y();
  v_[2] *= rhs.z();
  return *this;
}

inline Vector3f& Vector3f::operator/=(const Vector3f &rhs) {
  v_[0] /= rhs.x();
  v_[1] /= rhs.y();
  v_[2] /= rhs.z();
  return *this;
}

inline Vector3f& Vector3f::operator-=(const Vector3f &rhs) {
  v_[0] -= rhs.x();
  v_[1] -= rhs.y();
  v_[2] -= rhs.z();
  return *this;
}

inline Vector3f& Vector3f::operator*=(float t) {
  v_[0] *= t;
  v_[1] *= t;
  v_[2] *= t;
  return *this;
}

inline Vector3f& Vector3f::operator/=(float t) {
  const auto k = float{1.0f / t};
  v_[0] *= k;
  v_[1] *= k;
  v_[2] *= k;
  return *this;
}

inline Vector3f unit_vector(Vector3f v) {
  return v / v.norm2();
}

inline Vector3f random_in_unit_sphere() {
  Vector3f p;
  auto dis = std::uniform_real_distribution<>{0.0, 1.0};
  std::random_device device;
  do {
    p = (2.0f * Vector3f{
      static_cast<float>(dis(device)),
      static_cast<float>(dis(device)),
          static_cast<float>(dis(device))}) - Vector3f{1, 1, 1};
  } while (p.squared_length() >= 1.0f);
  return p;
}

}

#endif // VECTOR_HPP
