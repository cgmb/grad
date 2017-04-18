#ifndef VEC3F_H
#define VEC3F_H

#include <array>
#include "math/vec2f.h"
#include "math/vector_math.h"

struct vec3f {
  vec3f() = default;
  vec3f(vec2f xy, float z);
  vec3f(float x, float y, float z);

  float x() const;
  float y() const;
  float z() const;

  const float* data() const;
  float* data();

  const float& operator[](size_t i) const;
  float& operator[](size_t i);

  vec3f& operator+=(const vec3f& rhs);
  vec3f& operator-=(const vec3f& rhs);
  vec3f& operator/=(float rhs);

private:
  std::array<float, 3> data_;
};

inline vec3f::vec3f(vec2f xy, float z) {
  (*this)[0] = xy[0];
  (*this)[1] = xy[1];
  (*this)[2] = z;
};

inline vec3f::vec3f(float x, float y, float z) {
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
}

inline float vec3f::x() const {
  return (*this)[0];
}

inline float vec3f::y() const {
  return (*this)[1];
}

inline float vec3f::z() const {
  return (*this)[2];
}

inline const float* vec3f::data() const {
  return data_.data();
}

inline float* vec3f::data() {
  return data_.data();
}

inline const float& vec3f::operator[](size_t i) const {
  return data_[i];
}

inline float& vec3f::operator[](size_t i) {
  return data_[i];
}

inline vec3f& vec3f::operator+=(const vec3f& rhs) {
  vec3f& lhs = *this;
  lhs[0] += rhs[0];
  lhs[1] += rhs[1];
  lhs[2] += rhs[2];
  return lhs;
}

inline vec3f& vec3f::operator-=(const vec3f& rhs) {
  vec3f& lhs = *this;
  lhs[0] -= rhs[0];
  lhs[1] -= rhs[1];
  lhs[2] -= rhs[2];
  return lhs;
}

inline vec3f operator+(vec3f lhs, const vec3f& rhs) {
  lhs += rhs;
  return lhs;
}

inline vec3f operator-(vec3f lhs, const vec3f& rhs) {
  lhs -= rhs;
  return lhs;
}

inline vec3f operator*(const vec3f& lhs, const vec3f& rhs) {
  return vec3f(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}

inline vec3f operator-(const vec3f& rhs) {
  return vec3f(-rhs[0], -rhs[1], -rhs[2]);
}

inline vec3f& vec3f::operator/=(float rhs) {
  vec3f& lhs = *this;
  lhs[0] /= rhs;
  lhs[1] /= rhs;
  lhs[2] /= rhs;
  return lhs;
}

inline vec3f operator/(vec3f lhs, float rhs) {
  lhs /= rhs;
  return lhs;
}

inline vec3f operator*(float lhs, const vec3f& rhs) {
  return vec3f(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]);
}

inline vec3f operator*(const vec3f& lhs, float rhs) {
  return rhs * lhs;
}

inline vec3f normalized(vec3f x) {
  normalize_v3f(x.data());
  return x;
}

inline float magnitude(const vec3f& x) {
  return magnitude_v3f(x.data());
}

inline float magnitude_sq(const vec3f& x) {
  return magnitude_sq_v3f(x.data());
}

inline float dot(const vec3f& x, const vec3f& y) {
  return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

inline vec3f cross(const vec3f& x, const vec3f& y) {
  vec3f result;
  result[0] = x[1]*y[2] - x[2]*y[1];
  result[1] = x[2]*y[0] - x[0]*y[2];
  result[2] = x[0]*y[1] - x[1]*y[0];
  return result;
}

inline vec3f interpolate(const vec3f& begin, const vec3f& end, float fraction)
{
  return begin + fraction * (end - begin);
}

#endif
