#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "vec3f.h"
#include "mat4f.h"

struct quaternion {
  quaternion() = default;
  quaternion(float s, const vec3f& v);

  quaternion& operator*=(const quaternion& q);
  mat4f to_mat4f() const;

  static quaternion pure(const vec3f& v);
  static quaternion rotation(float angle, const vec3f& axis);

  float s;
  vec3f v;
};

inline quaternion::quaternion(float s, const vec3f& v) {
  this->s = s;
  this->v = v;
}

inline quaternion operator*(const quaternion& l, const quaternion& r) {
  return quaternion(l.s*r.s - dot(l.v, r.v), l.s*r.v + r.s*l.v + cross(l.v, r.v));
}

inline quaternion& quaternion::operator*=(const quaternion& q) {
  *this = *this * q;
  return *this;
}

inline mat4f quaternion::to_mat4f() const {
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
  return mat4f(1 - 2*v.y()*v.y() - 2*v.z()*v.z(),
                2*v.x()*v.y() - 2*v.z()*s,
                2*v.x()*v.z() + 2*v.y()*s,
                0,
               2*v.x()*v.y() + 2*v.z()*s,
                1 - 2*v.x()*v.x() - 2*v.z()*v.z(),
                2*v.y()*v.z() - 2*v.x()*s,
                0,
               2*v.x()*v.z() - 2*v.y()*s,
                2*v.y()*v.z() + 2*v.x()*s,
                1 - 2*v.x()*v.x() - 2*v.y()*v.y(),
                0,
               0, 0, 0, 1);
}

inline quaternion conjugate(const quaternion& x) {
  return quaternion(x.s, -x.v);
}

inline quaternion quaternion::pure(const vec3f& v) {
  return quaternion(0, v);
}

inline quaternion quaternion::rotation(float angle, const vec3f& axis) {
  float half_angle = angle / 2.f;
  return quaternion(cosf(half_angle), axis*sinf(half_angle));
}

inline vec3f qrotate(const vec3f& point, const vec3f& axis, float angle) {
  quaternion q = quaternion::rotation(angle, axis);
  quaternion p = quaternion::pure(point);
  quaternion r = q * p * conjugate(q);
  return r.v;
}

inline vec3f qrotate(const vec3f& point, const quaternion& q) {
  quaternion p = quaternion::pure(point);
  quaternion r = q * p * conjugate(q);
  return r.v;
}

#endif
