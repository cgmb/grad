#include "glad/glad.h"
#include "opengl/gl_model.h"
#include "opengl/sample_models.h"

GlModel GlModel::cube() {
  GlModel m;
  m.load_buffers(cube::vbuffer, sizeof(cube::vbuffer),
                 cube::ibuffer, sizeof(cube::ibuffer));
  m.set_drawing_mode(GL_TRIANGLES);
  m.set_count(cube::ibuffer_size);
  return m;
}

void GlModel::load_buffers(
  const std::vector<vec3f>& vbuffer,
  const std::vector<vec3f>& nbuffer,
  const std::vector<unsigned int>& ibuffer) {
  load_buffers(
    &vbuffer[0][0], sizeof(vec3f)*vbuffer.size(),
    &nbuffer[0][0], sizeof(vec3f)*nbuffer.size(),
    &ibuffer[0],sizeof(unsigned int)*ibuffer.size());
}

void GlModel::load_buffers(
  const std::vector<vec3f>& vbuffer,
  const std::vector<unsigned int>& ibuffer) {
  load_buffers(
    &vbuffer[0][0], sizeof(vec3f)*vbuffer.size(),
    &ibuffer[0], sizeof(unsigned int)*ibuffer.size());
}

void GlModel::load_buffers(
  const float* vbuffer, size_t vbuffer_size,
  const unsigned int* ibuffer, size_t ibuffer_size) {
  load_buffers(vbuffer, vbuffer_size,
               nullptr, 0u,
               ibuffer, ibuffer_size);
}

void GlModel::load_buffers(
  const float* vbuffer, size_t vbuffer_size,
  const float* nbuffer, size_t nbuffer_size,
  const unsigned int* ibuffer, size_t ibuffer_size) {
  have_normals_ = nbuffer != nullptr;

  glGenVertexArrays(1, &varray_id_);
  glBindVertexArray(varray_id_);

  glGenBuffers(buffer_count(), buffer_id_);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_[BUFFER_VERTEX]);
  glBufferData(GL_ARRAY_BUFFER, vbuffer_size, vbuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(POSITION_VID, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(POSITION_VID);

  if (have_normals_) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_[BUFFER_NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, nbuffer_size, nbuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(NORMAL_VID, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(NORMAL_VID);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_[BUFFER_INDEX]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibuffer_size, ibuffer, GL_STATIC_DRAW);

  glBindVertexArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlModel::update_buffers(
  const std::vector<vec3f>& vbuffer,
  const std::vector<vec3f>& nbuffer,
  const std::vector<unsigned int>& ibuffer) {
  update_buffers(
    &vbuffer[0][0], sizeof(vec3f)*vbuffer.size(),
    &nbuffer[0][0], sizeof(vec3f)*nbuffer.size(),
    &ibuffer[0],sizeof(unsigned int)*ibuffer.size());
}

void GlModel::update_buffers(
  const float* vbuffer, size_t vbuffer_size,
  const float* nbuffer, size_t nbuffer_size,
  const unsigned int* ibuffer, size_t ibuffer_size) {

  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_[BUFFER_VERTEX]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vbuffer_size, vbuffer);

  if (have_normals_) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_[BUFFER_NORMAL]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nbuffer_size, nbuffer);
  }
/* todo: fix this
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_[BUFFER_INDEX]);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ibuffer_size, ibuffer);
*/
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlModel::cleanup() {
  glDeleteBuffers(buffer_count(), buffer_id_);
  glDeleteVertexArrays(1, &varray_id_);
}

void GlModel::bind() {
  glBindVertexArray(varray_id_);
}

void GlModel::import(GLuint varray_id) {
  varray_id_ = varray_id;
}

void GlModel::draw() {
  glDrawElements(drawing_mode_, count_, GL_UNSIGNED_INT, 0);
}

void GlModel::draw_arrays() {
  glDrawArrays(drawing_mode_, 0, count_);
}

void GlModel::draw_instanced(size_t instance_count) {
  glDrawElementsInstanced(drawing_mode_, count_, GL_UNSIGNED_INT,
    0, instance_count);
}

size_t GlModel::buffer_count() const {
  return have_normals_ ? BUFFER_COUNT  : BUFFER_COUNT - 1;
}
