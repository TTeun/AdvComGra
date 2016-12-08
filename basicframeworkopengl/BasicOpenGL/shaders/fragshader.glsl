#version 410
// Fragment shader

layout (location = 1) in vec3 vertcolour_in;

out vec4 fColor;

void main() {

  fColor = vec4(vertcolour_in, 1.0);

}
