#ifndef VEC4F_H
#define VEC4F_H

#include <array>
#include "math/vec3f.h"

struct vec4f {
  vec4f() = default;
  vec4f(vec3f v, float w);
  vec4f(float x, float y, float z, float w);

  float x() const;
  float y() const;
  float z() const;
  float w() const;
  vec3f xyz() const;

  const float* data() const;
  float* data();

  const float& operator[](size_t i) const;
  float& operator[](size_t i);

private:
  std::array<float, 4> data_;
};

inline vec4f::vec4f(vec3f v, float w) {
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
  (*this)[3] = w;
}

inline vec4f::vec4f(float x, float y, float z, float w) {
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
  (*this)[3] = w;
}

inline float vec4f::x() const {
  return (*this)[0];
}

inline float vec4f::y() const {
  return (*this)[1];
}

inline float vec4f::z() const {
  return (*this)[2];
}

inline float vec4f::w() const {
  return (*this)[3];
}

inline vec3f vec4f::xyz() const {
  return vec3f{(*this)[0], (*this)[1], (*this)[2]};
}

inline const float* vec4f::data() const {
  return data_.data();
}

inline float* vec4f::data() {
  return data_.data();
}

inline const float& vec4f::operator[](size_t i) const {
  return data_[i];
}

inline float& vec4f::operator[](size_t i) {
  return data_[i];
}

#endif
