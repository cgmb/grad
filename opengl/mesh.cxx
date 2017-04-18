#include <algorithm>
#include <cmath>
#include <map>
#include <ostream>
#include "opengl/mesh.h"

namespace {
  inline vec3f triangle_normal(
    const vec3f& a, const vec3f& b, const vec3f& c) {
    vec3f ab = b - a;
    vec3f ac = c - a;
    return normalized(cross(ab, ac));
  }

  inline float triangle_angle(
    const vec3f& a, const vec3f& b, const vec3f& c) {
    vec3f ab = b - a;
    vec3f ac = c - a;
    return std::acos(dot(ab, ac) / (magnitude(ab) * magnitude(ac)));
  }
}

void Mesh::clear() {
  vertexes.clear();
  indexes.clear();
  face_normals.clear();
  vertex_normals.clear();
}

void Mesh::calculate_normals() {
  face_normals.resize(indexes.size()/3u);
  vertex_normals.resize(vertexes.size());

  // calculate face normals and collect data for vertex normals
  // also calculate face centers
  std::fill(vertex_normals.begin(), vertex_normals.end(), vec3f(0,0,0));
  for (size_t i = 0u; i < face_normals.size(); ++i) {
    unsigned int i1 = indexes[3*i];
    unsigned int i2 = indexes[3*i + 1];
    unsigned int i3 = indexes[3*i + 2];

    vec3f normal = -triangle_normal(
      vertexes[i1], vertexes[i2], vertexes[i3]);
    face_normals[i] = normal;

    // associate this face normal with each vertex
    vertex_normals[i1] += normal;
    vertex_normals[i2] += normal;
    vertex_normals[i3] += normal;
  }

  std::transform(vertex_normals.begin(), vertex_normals.end(),
    vertex_normals.begin(), normalized);
}

void Mesh::calculate_weighted_normals() {
  face_normals.resize(indexes.size()/3u);
  vertex_normals.resize(vertexes.size());

  // calculate face normals and collect data for vertex normals
  // also calculate face centers
  std::fill(vertex_normals.begin(), vertex_normals.end(), vec3f(0,0,0));
  for (size_t i = 0u; i < face_normals.size(); ++i) {
    unsigned int i1 = indexes[3*i];
    unsigned int i2 = indexes[3*i + 1];
    unsigned int i3 = indexes[3*i + 2];

    vec3f normal = -triangle_normal(
      vertexes[i1], vertexes[i2], vertexes[i3]);
    face_normals[i] = normal;

    float w1 = triangle_angle(
      vertexes[i1], vertexes[i2], vertexes[i3]);
    float w2 = triangle_angle(
      vertexes[i2], vertexes[i3], vertexes[i1]);
    float w3 = triangle_angle(
      vertexes[i3], vertexes[i1], vertexes[i2]);

    // associate this face normal with each vertex
    vertex_normals[i1] += w1 * normal;
    vertex_normals[i2] += w2 * normal;
    vertex_normals[i3] += w3 * normal;
  }

  std::transform(vertex_normals.begin(), vertex_normals.end(),
    vertex_normals.begin(), normalized);
}

namespace {
  bool are_equal(const vec3f& lhs, const vec3f& rhs) {
    return lhs.x() == rhs.x() &&
           lhs.y() == rhs.y() &&
           lhs.z() == rhs.z();
  }
}

void Mesh::simplify() {
  // find all our duplicates and drop them into a
  // duplicate to original map
  std::map<size_t,size_t> duplicates;
  // the bubble-sort of mesh algorithms -_-;
  for (size_t i = 0; i < vertexes.size(); ++i) {
    for (size_t j = i + 1; j < vertexes.size(); ++j) {
      if (are_equal(vertexes[i], vertexes[j])) {
        duplicates.emplace(j, i);
      }
    }
  }

  if (duplicates.empty()) {
    return; // nothing to simplify
  }

  // replace indexes that reference duplicates
  for (size_t i = 0; i < indexes.size(); ++i) {
    auto it = duplicates.find(indexes[i]);
    if (it != duplicates.end()) {
      indexes[i] = it->second;
    }
  }

  // erase useless vertexes and calculate index updates
  size_t offset = 0;
  std::vector<size_t> offset_for(vertexes.size());
  auto d_it = duplicates.begin();
  for (size_t i = 1; i < vertexes.size(); ++i) {
    while (i > d_it->first) {
      ++d_it;
    }

    vertexes[i - offset] = vertexes[i];

    if (i == d_it->first) {
      ++offset;
    }

    offset_for[i] = offset;
  }
  vertexes.resize(vertexes.size() - offset);

  // update indexes
  for (size_t i = 0; i < indexes.size(); ++i) {
    indexes[i] = indexes[i] - offset_for[indexes[i]];
  }
}

void Mesh::export_to_obj(std::ostream& out) const {
  for (vec3f v : this->vertexes) {
    out << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
  }
  for (size_t i = 0; i < this->indexes.size(); i += 3) {
    out << "f ";
    out << this->indexes[i] + 1 << " ";
    out << this->indexes[i+1] + 1 << " ";
    out << this->indexes[i+2] + 1 << " ";
    out << "\n";
  }
}

Box Mesh::get_bounding_box() const {
  Box box;
  if (vertexes.empty()) {
    box.min_corner = vec3f{0,0,0};
    box.max_corner = vec3f{0,0,0};
    return box;
  }

  box.min_corner = vertexes[0];
  box.max_corner = vertexes[0];
  for (vec3f v : vertexes) {
    for (int i = 0; i < 3; ++i) {
      if (v[i] < box.min_corner[i]) {
        box.min_corner[i] = v[i];
      } else if (v[i] > box.max_corner[i]) {
        box.max_corner[i] = v[i];
      }
    }
  }
  return box;
}
