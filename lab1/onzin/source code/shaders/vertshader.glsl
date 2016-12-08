#version 410
// Vertex shader

layout (location = 0) in vec2 vertcoords_clip_vs;
layout (location = 1) in vec3 vertcolour_in;

layout (location = 1) out vec3 vertcolour_out;

void main() {
  vertcolour_out = vertcolour_in;
  gl_Position = vec4(vertcoords_clip_vs, 0.0, 1.0);

}
