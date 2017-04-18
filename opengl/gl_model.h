#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <vector>
#include <GL/gl.h>
#include "math/vec3f.h"

class GlModel {
public:
  GlModel();

  static GlModel cube();

  void load_buffers(
    const std::vector<vec3f>& vbuffer,
    const std::vector<vec3f>& nbuffer,
    const std::vector<unsigned int>& ibuffer);

  void load_buffers(
    const std::vector<vec3f>& vbuffer,
    const std::vector<unsigned int>& ibuffer);

  void load_buffers(
    const float* vbuffer, size_t vbuffer_size,
    const unsigned int* ibuffer, size_t ibuffer_size);

  void load_buffers(
    const float* vbuffer, size_t vbuffer_size,
    const float* nbuffer, size_t nbuffer_size,
    const unsigned int* ibuffer, size_t ibuffer_size);

  void update_buffers(
    const std::vector<vec3f>& vbuffer,
    const std::vector<vec3f>& nbuffer,
    const std::vector<unsigned int>& ibuffer);

  void update_buffers(
    const float* vbuffer, size_t vbuffer_size,
    const float* nbuffer, size_t nbuffer_size,
    const unsigned int* ibuffer, size_t ibuffer_size);

  void import(GLuint varray_id);
  void cleanup();

  void bind();
  void draw();
  void draw_instanced(size_t instance_count);
  void draw_arrays();

  void set_drawing_mode(GLenum mode);
  void set_count(GLsizei size);

private:
  enum {
    BUFFER_VERTEX,
    BUFFER_INDEX,
    BUFFER_NORMAL,

    BUFFER_COUNT,
  };

  static const GLuint POSITION_VID = 0;
  static const GLuint NORMAL_VID = 1;

private:
  size_t buffer_count() const;

private:
  GLuint varray_id_;
  GLuint buffer_id_[BUFFER_COUNT];
  GLenum drawing_mode_;
  GLsizei count_;
  bool have_normals_;
};

inline GlModel::GlModel()
  : varray_id_(0)
  , buffer_id_()
  , drawing_mode_(0)
  , count_(0)
  , have_normals_(false)
{
}

inline void GlModel::set_drawing_mode(GLenum mode) {
  drawing_mode_ = mode;
}

inline void GlModel::set_count(GLsizei size) {
  count_ = size;
}

#endif
