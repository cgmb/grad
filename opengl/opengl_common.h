#ifndef OPENGL_COMMON_H
#define OPENGL_COMMON_H

#include <GL/gl.h>

char* load_file(
  const char* filename,
  int* length
);

void compile_shader(
  GLuint shader_id,
  const char* shader_name);

void link_program(
  GLuint program_id
);

GLuint load_shaders(
  const char* vertex_shader_name,
  const char* fragment_shader_name
);

GLuint load_shaders(
  const char* vertex_shader_name,
  const char* fragment_shader_name,
  const char* geometry_shader_name
);

GLuint load_shaders_with_fallback(
  const char* vertex_shader_name,
  const char* default_vertex_shader_text,
  const char* fragment_shader_name,
  const char* default_fragment_shader_text
);

GLuint load_shaders_from_string(
  const char* vert_shader_text, int vert_shader_length,
  const char* frag_shader_text, int frag_shader_length,
  const char* vertex_shader_name, const char* fragment_shader_name
);

GLuint load_shaders_from_string(
  const char** shader_text_arr, int* shader_length_arr,
  const char** shader_name_arr, GLenum* shaderType,
  size_t shader_count
);

int check_version_string(
  const char* name,
  const char* version,
  long major_req,
  long minor_req,
  const char* version_req
);

#endif
