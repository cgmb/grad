#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/matrix_math.h"
#include "math/quaternion.h"
#include "misc/colormap.h"
#include "opengl/opengl_common.h"
#include "opengl/shader_program.h"

bool g_pause = false;
bool g_wireframe = false;
bool g_dirty = true;
bool g_panning = false;
bool g_rotating = false;
vec2f g_drag_prev;

vec2f g_pan = {0,0};
quaternion g_orient = quaternion::rotation(0, vec3f{0,0,1});

const float k_lines[] = {
   0.25f, 0.25f,
  -0.25f, 0.00f,
  -0.50f, 0.00f,
};

extern const char* const k_shaders_gradient_vert;
extern const char* const k_shaders_gradient_frag;

struct GradientShaderParams {
  enum {
    MVP_VID,
    LINE_VID,
    COLORMAP_FID,
    COUNT,
  };
  ShaderProgram shader;
  GLint ids[COUNT];
} g_gradient;

const char* k_gradient_attributes[GradientShaderParams::COUNT] = {
  "v_mvp",
  "f_line",
  "f_colormap",
};

GLuint g_varray_id;
GLuint g_viridis_id;

float g_aspect_scale_x = 1.0;
float g_aspect_scale_y = 1.0;
int g_window_size_x = 512;
int g_window_size_y = 512;
int g_frame_size_x = g_window_size_x;
int g_frame_size_y = g_window_size_y;
float g_scale = 1.f;

void display() {
  glViewport(0, 0, g_frame_size_x, g_frame_size_y);
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4f p = mat4f::ortho()
    * mat4f::scale({g_scale, g_scale, g_scale})
    * mat4f::scale({g_aspect_scale_x, g_aspect_scale_y, 1});
  mat4f v = mat4f::trans({g_pan.x(), g_pan.y(), 0})
    * g_orient.to_mat4f();

  mat4f mvp = p * v;

  g_gradient.shader.use();
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_1D, g_viridis_id);
  g_gradient.shader.set_uniform(g_gradient.ids[GradientShaderParams::MVP_VID], mvp);
  glUniform2fv(g_gradient.ids[GradientShaderParams::LINE_VID], 3, k_lines);
  glBindVertexArray(g_varray_id);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void update_world() {
  if (g_pause) {
    return;
  }

  g_dirty = true;
}

void print_opengl_version() {
  printf("OpenGL version: %s\nOpenGL Shading Language version: %s\n",
      (const char*)glGetString(GL_VERSION),
      (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void query_uniform_locations(ShaderProgram* shader, size_t count,
    const char** attributes, GLint* ids) {
  for (size_t i = 0; i < count; ++i) {
    ids[i] = shader->uniform_location(attributes[i]);
    if (ids[i] < 0) {
      fprintf(stderr, "Attribute '%s' not found in shader\n", attributes[i]);
    }
  }
}

void init_colormap() {
  glActiveTexture(GL_TEXTURE0 + 0);

  glGenTextures(1, &g_viridis_id);
  glBindTexture(GL_TEXTURE_2D, g_viridis_id);

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, k_viridis_count, 0,
    GL_RGB, GL_FLOAT, k_viridis);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void init() {
  print_opengl_version();
  init_colormap();

  g_gradient.shader.load_with_fallback("shaders/gradient.vert", k_shaders_gradient_vert,
                                       "shaders/gradient.frag", k_shaders_gradient_frag);
  query_uniform_locations(&g_gradient.shader, GradientShaderParams::COUNT,
      k_gradient_attributes, g_gradient.ids);

  const float vbuffer[] = {
    -1.0f,-1.0f, 0.f,
    -1.0f, 1.0f, 0.f,
     1.0f,-1.0f, 0.f,
     1.0f, 1.0f, 0.f,
  };
  glGenVertexArrays(1, &g_varray_id);
  glBindVertexArray(g_varray_id);
  GLuint buffer_id;
  glGenBuffers(1, &buffer_id);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbuffer), vbuffer, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cleanup() {
}

void toggle_pause() {
  g_pause = !g_pause;
}

void toggle_wireframe() {
  g_wireframe = !g_wireframe;
  if (g_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

void zoom_in() {
  g_scale *= 1.125f;
  g_dirty = true;
}

void zoom_out() {
  g_scale = std::max(1.f/32, g_scale / 1.125f);
  g_dirty = true;
}

void quit() {
  exit(0);
}

void key_press(GLFWwindow* window, int key, int scancode, int action,
                   int mods) {
  (void)window; (void)mods; (void)scancode; (void)action;
  if (action != GLFW_PRESS) {
    return;
  }

  if (key == GLFW_KEY_P) {
    toggle_pause();
  } else if (key == GLFW_KEY_EQUAL) {
    zoom_in();
  } else if (key == GLFW_KEY_MINUS) {
    zoom_out();
  } else if (key == GLFW_KEY_W) {
    toggle_wireframe();
  } else if (key == GLFW_KEY_Q) {
    quit();
  } else {
    printf("Unrecognised key: '%c'\n", key);
  }
}

void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset) {
  (void)window; (void)xoffset;
  if (yoffset < 0) {
    zoom_in();
  } else if (yoffset > 0) {
    zoom_out();
  }
}

vec2f editor_window_coord(int x, int y) {
  return vec2f(x/float(g_window_size_x), 1.f-y/float(g_window_size_y));
}

void mouse_press(GLFWwindow* window, int button, int action, int mods) {
  (void)mods;
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      g_panning = true;
      g_drag_prev = editor_window_coord((int)x, (int)y);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      g_rotating = true;
      g_drag_prev = editor_window_coord((int)x, (int)y);
    }
  } else if (action == GLFW_RELEASE) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      g_panning = false;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      g_rotating = false;
    }
  }
}

void mouse_move(GLFWwindow* window, double x, double y) {
  (void)window;
  if (g_panning) {
    vec2f coord = editor_window_coord((int)x, (int)y);
    g_pan[0] += 2 * (coord[0] - g_drag_prev[0]) / (g_aspect_scale_x * g_scale);
    g_pan[1] += 2 * (coord[1] - g_drag_prev[1]) / (g_aspect_scale_y * g_scale);
    g_drag_prev = coord;
    g_dirty = true;
  } else if (g_rotating) {
    vec2f coord = editor_window_coord((int)x, (int)y);
    float x = (coord[0] - g_drag_prev[0]) / (g_aspect_scale_x * g_scale);
    float y = (coord[1] - g_drag_prev[1]) / (g_aspect_scale_y * g_scale);
    vec3f axis{y,x,0};
    float m = magnitude(axis);
    g_orient = quaternion::rotation(m, normalized(axis)) * g_orient;
    g_drag_prev = coord;
    g_dirty = true;
  }
}

void reshape_framebuffer(GLFWwindow* window, int w, int h) {
  (void)window;
  glViewport(0, 0, w, h);
  g_dirty = true;
}

void reshape_window(GLFWwindow* window, int w, int h) {
  (void)window;
  g_window_size_x = w;
  g_window_size_y = h;
  if (w > h) {
    g_aspect_scale_x = h /(float)w;
    g_aspect_scale_y = 1.0;
  } else {
    g_aspect_scale_x = 1.0;
    g_aspect_scale_y = w / (float)h;
  }
  g_frame_size_x = w;
  g_frame_size_y = h;
  g_dirty = true;
}

GLFWwindow* g_window;

void reshape(int w, int h) {
  reshape_window(g_window, w, h);
}

void do_update() {
  update_world();
  glfwPollEvents();

  if (g_dirty) {
    display();
    g_dirty = false;
  }
}

int main(int argc, char** argv) {
  (void)argc; (void)argv;
  if (!glfwInit()) {
    fprintf(stderr, "GLFW initialization failed\n");
    exit(1);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow* window = glfwCreateWindow(g_window_size_x, g_window_size_y,
      "Kelp", NULL, NULL);
  g_window = window;
  if (!window) {
    exit(1);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "GLAD initialization failed\n");
    exit(1);
  }

  glfwSetWindowSizeCallback(window, reshape_window);
  glfwSetFramebufferSizeCallback(window, reshape_framebuffer);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwGetFramebufferSize(window, &g_window_size_x, &g_window_size_y);
  glfwSetKeyCallback(window, key_press);
  glfwSetCursorPosCallback(window, mouse_move);
  glfwSetMouseButtonCallback(window, mouse_press);
  glfwSetScrollCallback(window, mouse_scroll);

  init();

  reshape_window(window, g_window_size_x, g_window_size_y);
  reshape_framebuffer(window, g_window_size_x, g_window_size_y);

  while (!glfwWindowShouldClose(window)) {
    update_world();
    glfwPollEvents();

    if (g_dirty) {
      display();
      glfwSwapBuffers(window);
      g_dirty = false;
    }
  }
  return 0;
}
