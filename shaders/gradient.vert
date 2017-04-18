#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 v_mvp;

out vec4 f_position;

void main() {
  f_position = position;
  gl_Position = v_mvp * position;
}
