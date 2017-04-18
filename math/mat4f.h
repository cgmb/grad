#ifndef MAT4F_H
#define MAT4F_H

#include <array>
#include "math/vector_math.h"
#include "math/vec4f.h"

struct mat4f {
  mat4f();
  mat4f(float a, float b, float c, float d,
        float e, float f, float g, float h,
        float i, float j, float k, float l,
        float m, float n, float o, float p);

  operator float*();
  operator const float*() const;

  static mat4f identity();
  static mat4f ortho();
  static mat4f proj(float r);
  static mat4f trans(const vec3f& t);
  static mat4f scale(const vec3f& s);
  static mat4f rotx(float rx);
  static mat4f roty(float ry);
  static mat4f rotz(float rz);

  mat4f& operator*=(const mat4f& rhs);
  float& at(unsigned x, unsigned y);
  const float& at(unsigned x, unsigned y) const;

  std::array<float, 16> data;
};

inline mat4f::mat4f() {
}

inline mat4f::mat4f(
  float a, float b, float c, float d,
  float e, float f, float g, float h,
  float i, float j, float k, float l,
  float m, float n, float o, float p)
  : data({a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p})
{
}

inline mat4f::operator float*() {
  return data.data();
}

inline mat4f::operator const float*() const {
  return data.data();
}

inline mat4f operator*(const mat4f& lhs, const mat4f& rhs) {
  mat4f result;
  m4f_mul_m4fo(lhs, rhs, result);
  return result;
}

inline mat4f& mat4f::operator*=(const mat4f& rhs) {
  mat4f result = *this * rhs;
  *this = result;
  return *this;
}

inline mat4f mat4f::identity() {
  return mat4f(1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1);
}

inline mat4f mat4f::ortho() {
  return mat4f(1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 1);
}

inline mat4f mat4f::proj(float r) {
  return mat4f(1, 0, 0,     0,
               0, 1, 0,     0,
               0, 0, 1,     0,
               0, 0, 1.f/r, 1);
}

inline mat4f mat4f::trans(const vec3f& t) {
  mat4f m;
  m4f_fill_trans_m4fo(t[0], t[1], t[2], m);
  return m;
}

inline mat4f mat4f::scale(const vec3f& s) {
  mat4f m;
  m4f_fill_scale_m4fo(s[0], s[1], s[2], m);
  return m;
}

inline mat4f mat4f::rotx(float rx) {
  mat4f m;
  m4f_fill_rotx_m4fo(rx, m);
  return m;
}

inline mat4f mat4f::roty(float ry) {
  mat4f m;
  m4f_fill_roty_m4fo(ry, m);
  return m;
}

inline mat4f mat4f::rotz(float rz) {
  mat4f m;
  m4f_fill_rotz_m4fo(rz, m);
  return m;
}

inline float& mat4f::at(unsigned x, unsigned y) {
  return data[4*y + x];
}

inline const float& mat4f::at(unsigned x, unsigned y) const {
  return data[4*y + x];
}

inline mat4f transpose(const mat4f& t) {
  mat4f m;
  for (unsigned int i = 0; i < 4u; ++i) {
    for (unsigned int j = 0; j < 4u; ++j) {
      m.at(j, i) = t.at(i, j);
    }
  }
  return m;
}

inline vec4f operator*(const mat4f& m, const vec4f& v) {
  vec4f r{0,0,0,0};
  for (unsigned int i = 0; i < 4u; ++i) {
    for (unsigned int j = 0; j < 4u; ++j) {
      r[i] += m.at(j, i) * v[j];
    }
  }
  return r;
}

#endif
