#version 330 core

in vec4 f_position;

uniform vec2 f_line[3];
uniform sampler1D f_colormap;

out vec4 frag_color;

const float pi = 3.1415926535897932;
const float sigma_sq = 0.25;

float perp_dot2d(vec2 u, vec2 v) {
  return u.x*v.y - u.y*v.x;
}

vec2 perpendicular(vec2 v) {
  return vec2(-v.y, v.x);
}

// returns nearest point to q on the line p + r*t
vec2 nearest_point_on_line(vec2 p, vec2 r, vec2 q) {
  vec2 s = perpendicular(r);
  float rxs = perp_dot2d(r, s);
  float t = clamp(0, perp_dot2d(q-p, s/rxs), 1);
  return p + r*t;
}

float distance_sq_from_line(vec2 p, vec2 r, vec2 q) {
  vec2 x = nearest_point_on_line(p, r, q) - q;
  return dot(x, x);
}

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

void main() {
  vec2 pos = f_position.xy;

  float distance_sq = 1.f / 0.0; // inf
  for (int i = 0; i < 2; ++i) {
    vec2 direction = f_line[i+1] - f_line[i];
    distance_sq = min(distance_sq, distance_sq_from_line(f_line[i], direction, pos));
  }

  frag_color = texture1D(f_colormap, gaussian2(distance_sq));
}
