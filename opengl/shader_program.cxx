#include "opengl/shader_program.h"
#include "opengl/opengl_common.h"

ShaderProgram::ShaderProgram()
  : program_id_(0)
{
}

void ShaderProgram::load(const char* vertex, const char* fragment) {
  program_id_ = load_shaders(vertex, fragment);
}

void ShaderProgram::load(const char* vertex, const char* fragment, const char* geometry) {
  program_id_ = load_shaders(vertex, fragment, geometry);
}

void ShaderProgram::load_with_fallback(const char* vertex, const char* vertex_fallback,
  const char* fragment, const char* fragment_fallback) {
  program_id_ = load_shaders_with_fallback(
    vertex, vertex_fallback, fragment, fragment_fallback);
}

void ShaderProgram::load_text(const char* vertex, int vertex_size,
                               const char* fragment, int fragment_size) {
  const int shader_count = 2;
  const char* shader_text_arr[2] = { vertex, fragment };
  int shader_length_arr[2] = { vertex_size, fragment_size };
  GLenum shader_type_arr[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
  const char* shader_name_arr[2] = { "vertex", "fragment" };
  program_id_ = load_shaders_from_string(shader_text_arr, shader_length_arr,
    shader_name_arr, shader_type_arr, shader_count);
}

void ShaderProgram::load_text(const char* vertex, int vertex_size,
                               const char* fragment, int fragment_size,
                               const char* geometry, int geometry_size) {
  const int shader_count = 3;
  const char* shader_text_arr[3] = { vertex, fragment, geometry };
  int shader_length_arr[3] = { vertex_size, fragment_size, geometry_size };
  GLenum shader_type_arr[3] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
  const char* shader_name_arr[3] = { "vertex", "fragment", "geometry" };
  program_id_ = load_shaders_from_string(shader_text_arr, shader_length_arr,
    shader_name_arr, shader_type_arr, shader_count);
}

void ShaderProgram::use() const {
  glUseProgram(program_id_);
}

void ShaderProgram::destroy() {
  glDeleteProgram(program_id_);
  program_id_ = 0;
}

GLint ShaderProgram::uniform_location(const char* uniform_name) const {
  return glGetUniformLocation(program_id_, uniform_name);
}

void ShaderProgram::set_uniform(GLint location, float value) const {
  glUniform1f(location, value);
}

void ShaderProgram::set_uniform(GLint location, const mat4f& value) const {
  glUniformMatrix4fv(location, 1, GL_FALSE, transpose(value));
}

void ShaderProgram::set_uniform(GLint location, const vec4f& value) const {
  glUniform4fv(location, 1, value.data());
}

void ShaderProgram::set_uniform(GLint location, const vec3f& value) const {
  glUniform3fv(location, 1, value.data());
}

void ShaderProgram::set_uniform(GLint location, const vec2f& value) const {
  glUniform2fv(location, 1, value.data());
}
