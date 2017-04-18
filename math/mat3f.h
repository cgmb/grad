#ifndef MAT3F_H
#define MAT3F_H

#include <array>
#include <initializer_list>
#include <utility>
#include "math/vector_math.h"

struct mat3f : public std::array<float, 9> {
  mat3f();
  template <class... T>
  mat3f(T... init);

  operator float*();
  operator const float*() const;
};

inline mat3f::mat3f() {
}

template <class... T>
inline mat3f::mat3f(T... init)
  : std::array<float, 9>({init...})
{
}

inline mat3f::operator float*() {
  return this->data();
}

inline mat3f::operator const float*() const {
  return this->data();
}

#endif
