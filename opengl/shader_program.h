#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include "math/matrix_math.h"

struct ShaderProgram {
  ShaderProgram();

  void load(const char* vertex, const char* fragment);
  void load(const char* vertex, const char* fragment, const char* geometry);
  void load_text(const char* vertex, int vertex_size,
                 const char* fragment, int fragment_size);
  void load_text(const char* vertex, int vertex_size,
                 const char* fragment, int fragment_size,
                 const char* geometry, int geometry_size);
  void use() const;
  void destroy();

  GLint uniform_location(const char* uniform_name) const;
  void set_uniform(GLint location, float value) const;
  void set_uniform(GLint location, const mat4f& value) const;
  void set_uniform(GLint location, const vec4f& value) const;
  void set_uniform(GLint location, const vec3f& value) const;
  void set_uniform(GLint location, const vec2f& value) const;

private:
  GLuint program_id_;
};

#endif
