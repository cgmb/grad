#ifndef SAMPLE_MODELS_H
#define SAMPLE_MODELS_H

#include <cassert>
#include <cmath>
#include "mesh.h"

namespace cube {
  const float vbuffer[] = {
    -0.5, -0.5,  0.5, // 0 back top left
     0.5, -0.5,  0.5, // 1 back top right
     0.5,  0.5,  0.5, // 2 back bottom right
    -0.5,  0.5,  0.5, // 3 back bottom left
    -0.5, -0.5, -0.5, // 4 front top left
     0.5, -0.5, -0.5, // 5 front top right
     0.5,  0.5, -0.5, // 6 front bottom right
    -0.5,  0.5, -0.5, // 7 front bottom left
  };

  const int unsigned ibuffer[] = {
    0, 1, 2, 0, 2, 3, // back
    5, 1, 0, 4, 5, 0, // top
    7, 4, 0, 3, 7, 0, // left
    1, 5, 6, 1, 6, 2, // right
    3, 2, 6, 3, 6, 7, // bottom
    6, 5, 4, 7, 6, 4, // front
  };

  const size_t ibuffer_size
    = sizeof(ibuffer) / sizeof(ibuffer[0]);
}

namespace sphere {
  inline Mesh generate(float radius, size_t quality) {
    assert(quality > 2);
    quality *= 2; // indexing algorithm is buggy for odd quality values
    Mesh m;

    // calculate vertexes
    m.vertexes.push_back(vec3f(0, radius, 0));
    for (size_t i = 1; i < quality; ++i) {
      float fi = i/(float)quality;
      for (size_t j = 0; j < quality; ++j) {
        float fj = j/(float)quality;
        float cfi = std::cos(M_PI*fi);
        float sfi = std::sin(M_PI*fi);
        float cfj = std::cos(2*M_PI*fj);
        float sfj = std::sin(2*M_PI*fj);
        vec3f point(radius * sfi * cfj, radius * cfi, radius * sfi * sfj);
        m.vertexes.push_back(point);
      }
    }
    m.vertexes.push_back(vec3f(0, -radius, 0));

    // calculate indexes
    size_t num_col = quality;
    size_t num_row = quality;
    size_t last_row = num_row - 1;
    unsigned int last_vert = m.vertexes.size() - 1;
    for (size_t row = 0; row < num_row; ++row) {
      for (size_t col = 0; col < num_col; ++col) {
        size_t r = row - 1; // first row doesn't take up a full column
        if (row != last_row) {
          if (row == 0) {
            m.indexes.push_back(0);
          } else {
            m.indexes.push_back(r * num_col + 1 + col);
          }
          m.indexes.push_back((r + 1) * num_col + 1 + col);
          m.indexes.push_back((r + 1) * num_col + 1 + (col + 1) % num_col);
        }

        if (row != 0) {
          m.indexes.push_back(r * num_col + 1 + col);
          if (row == last_row) {
            m.indexes.push_back(last_vert);
          } else {
            m.indexes.push_back((r + 1) * num_col + 1 + (col + 1) % num_col);
          }
          m.indexes.push_back(r * num_col + 1 + (col + 1) % num_col);
        }
      }
    }

    return m;
  }
}

#endif
