#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include "opengl/opengl_common.h"

char* load_file(const char* filename, int* length) {
  FILE* f = fopen(filename, "rb");
  char* contents = NULL;
  long fsize = 0; 
  *length = 0;
  if (!f) {
    goto exit;
  }
  if (fseek(f, 0, SEEK_END)) {
    goto close_exit;
  }
  fsize = ftell(f);
  if (fsize > INT_MAX) {
    goto close_exit;
  }
  *length = fsize;
  if (fseek(f, 0, SEEK_SET)) {
    goto close_exit;
  }
  contents = (char*)malloc(fsize + 1);
  if (!contents) {
    goto close_exit;
  }
  if (fread(contents, fsize, 1, f)) {
    goto close_exit;
  }
  contents[fsize] = '\0';
close_exit:
  fclose(f);
exit:
  return contents;
}

void compile_shader(GLuint shader_id, const char* shader_name) {
  glCompileShader(shader_id);

  GLint result = GL_FALSE;
  int info_log_length;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (result == GL_FALSE) {
    char* error_message = (char*)malloc(info_log_length + 1);
    error_message[info_log_length] = '\0';
    glGetShaderInfoLog(shader_id, info_log_length, NULL, error_message);
    fprintf(stderr, "Shader compile failure!\nError in %s:\n%s\n", 
      shader_name, error_message);
    free(error_message);
  }
}

void link_program(GLuint program_id) {
  glLinkProgram(program_id);

  GLint result = GL_FALSE;
  int info_log_length;
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (result == GL_FALSE) {
    char* error_message = (char*)malloc(info_log_length + 1);
    error_message[info_log_length] = '\0';
    glGetProgramInfoLog(program_id, info_log_length, NULL, error_message);
    fprintf(stderr, "Shader link failure!\n%s\n", error_message);
    free(error_message);
  }
}

GLuint load_shaders_from_string(
const char** shader_text_arr, int* shader_length_arr,
const char** shader_name_arr, GLenum* shader_type_arr,
size_t shader_count) {
  GLuint* shaders = (GLuint*)malloc(shader_count*sizeof(GLuint));
  for (size_t i = 0; i < shader_count; ++i) {
    shaders[i] = glCreateShader(shader_type_arr[i]);
  }

  for (size_t i = 0; i < shader_count; ++i) {
    glShaderSource(shaders[i], 1,
      (const GLchar**)&shader_text_arr[i], &shader_length_arr[i]);
    compile_shader(shaders[i], shader_name_arr[i]);
  }

  GLuint program = glCreateProgram();
  for (size_t i = 0; i < shader_count; ++i) {
    glAttachShader(program, shaders[i]);
  }
  link_program(program);

  for (size_t i = 0; i < shader_count; ++i) {
    glDeleteShader(shaders[i]);
  }
  free(shaders);

  return program;
}

GLuint load_shaders_from_string(
const char* vert_shader_text, int vert_shader_length,
const char* frag_shader_text, int frag_shader_length,
const char* vertex_shader_name, const char* fragment_shader_name) {
  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vert_shader, 1, 
    (const GLchar**)&vert_shader_text, &vert_shader_length);
  glShaderSource(frag_shader, 1,
    (const GLchar**)&frag_shader_text, &frag_shader_length);

  compile_shader(vert_shader, vertex_shader_name);
  compile_shader(frag_shader, fragment_shader_name);

  GLuint program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  link_program(program);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return program;
}

GLuint load_shaders(
const char* vertex_shader_name,
const char* fragment_shader_name) {
  int vert_shader_length;
  char* vert_shader_text = load_file(vertex_shader_name, &vert_shader_length);
  if (!vert_shader_text) {
    fprintf(stderr, "Failed to load file: %s\n", vertex_shader_name);
  }
  int frag_shader_length;
  char* frag_shader_text = load_file(fragment_shader_name, &frag_shader_length);
  if (!frag_shader_text) {
    fprintf(stderr, "Failed to load file: %s\n", fragment_shader_name);
  }

  GLuint shader = load_shaders_from_string(
    vert_shader_text, vert_shader_length,
    frag_shader_text, frag_shader_length,
    vertex_shader_name, fragment_shader_name);

  free(vert_shader_text);
  free(frag_shader_text);

  return shader;
}

GLuint load_shaders(
const char* vertex_shader_name,
const char* fragment_shader_name,
const char* geometry_shader_name) {
  int vert_shader_length;
  char* vert_shader_text = load_file(vertex_shader_name, &vert_shader_length);
  if (!vert_shader_text) {
    fprintf(stderr, "Failed to load file: %s\n", vertex_shader_name);
  }
  int frag_shader_length;
  char* frag_shader_text = load_file(fragment_shader_name, &frag_shader_length);
  if (!frag_shader_text) {
    fprintf(stderr, "Failed to load file: %s\n", fragment_shader_name);
  }
  int geom_shader_length;
  char* geom_shader_text = load_file(geometry_shader_name, &geom_shader_length);
  if (!geom_shader_text) {
    fprintf(stderr, "Failed to load file: %s\n", geometry_shader_name);
  }

  const int shader_count = 3;
  const char* shader_text_arr[3] = { vert_shader_text, frag_shader_text, geom_shader_text };
  int shader_length_arr[3] = { vert_shader_length, frag_shader_length, geom_shader_length };
  GLenum shader_type_arr[3] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
  const char* shader_name_arr[3] = { vertex_shader_name, fragment_shader_name, geometry_shader_name };
  GLuint shader = load_shaders_from_string(shader_text_arr, shader_length_arr,
    shader_name_arr, shader_type_arr, shader_count);

  free(vert_shader_text);
  free(frag_shader_text);
  free(geom_shader_text);

  return shader;
}

int check_version_string(const char* name, const char* version,
  long major_req, long minor_req, const char* version_req) {
  printf("%s version: %s\n", name, version);
  char* end;
  long major_num = strtol(version, &end, 10);
  if (*end == '.' && errno != EINVAL && errno != ERANGE) {
    if (major_num > major_req) {
      // all is well
    } else if (major_num < major_req) {
      fprintf(stderr, "Requires %s %s\n", name, version_req);
      return 1;
    } else {
      long minor_num = strtol(end + 1, NULL, 10);
      if (errno != EINVAL && errno != ERANGE) {
        if (minor_num < minor_req) {
          fprintf(stderr, "Requires %s %s\n", name, version_req);
          return 1;
        } else {
          // all is well
        }
      } else {
        fprintf(stderr, "%s minor version parse error\n", name);
        return 1;
      }
    }
  } else {
    fprintf(stderr, "%s major version parse error\n", name);
    return 1;
  }
  return 0;
}
