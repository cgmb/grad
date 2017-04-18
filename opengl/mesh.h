#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iosfwd>
#include "math/vec3f.h"

struct Box {
  vec3f min_corner;
  vec3f max_corner;

  vec3f center() const {
    return (min_corner + max_corner) / 2.f;
  }

  vec3f dimensions() const {
    return max_corner - min_corner;
  }
};

struct Mesh {
  std::vector<vec3f> vertexes;
  std::vector<unsigned int> indexes;

  std::vector<vec3f> face_normals;
  std::vector<vec3f> vertex_normals;

  void clear();
  void calculate_normals();
  void calculate_weighted_normals();
  void export_to_obj(std::ostream& os) const;
  Box get_bounding_box() const;
  void simplify();
};

#endif
