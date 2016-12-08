#version 410
// Vertex shader

layout (location = 0) in vec3 vertcoords_world_vs;
layout (location = 1) in vec3 vertcolour_in;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

layout (location = 1) out vec3 vertcolour_out;

void main() {

  gl_Position = projectionmatrix * modelviewmatrix * vec4(vertcoords_world_vs, 1.0);
  vertcolour_out = vertcolour_in;
}
