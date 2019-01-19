#version 330 core

in vec4 f_position;

uniform vec2 f_line[3];
uniform sampler1D f_colormap;

out vec4 frag_color;

const float pi = 3.1415926535897932;
const float sigma_sq = 0.25;

float gaussian2(float distance2) {
  if (distance2 > 50.0 * (2.0 * sigma_sq)) {
    return 0.0;
  } else {
    float normalized_distance2 = distance2 / (2.0 * sigma_sq);
    return (1.0 / (2.0 * pi * sigma_sq)) * exp(-normalized_distance2);
  }
}

float gaussian(vec2 p) {
  return gaussian2(dot(p,p));
}

const float k_h = 1.5;
const float k_h_sq = k_h*k_h;
const float k_spiky = 4/(pi*pow(k_h, 8.0));

float squared(float x) {
  return x*x;
}

float cubed(float x) {
  return x*x*x;
}

float spiky_kernel(float distance_sq) {
  if (distance_sq >= k_h) {
    return 0.0;
  }
  return k_spiky * cubed(k_h_sq - sqrt(distance_sq));
}

float length_sq(vec2 x) {
  return dot(x,x);
}

float spiky_kernel(vec2 diff) {
  return spiky_kernel(length_sq(diff));
}

vec2 spiky_kernel_gradient(vec2 diff) {
  float dist_sq = length_sq(diff);
  if (dist_sq >= k_h_sq) {
    return vec2(0,0);
  }
  return -3 * k_spiky * squared(k_h - sqrt(dist_sq)) * normalize(diff);
}

void main() {
  vec2 pos = f_position.xy;
  vec2 diff = pos - f_line[0];
  frag_color = texture1D(f_colormap, spiky_kernel(pos));
}
