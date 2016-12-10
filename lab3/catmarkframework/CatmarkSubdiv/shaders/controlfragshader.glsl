#version 410
// Fragment shader

layout (location = 0) in vec3 vertcoords_camera_fs;
layout (location = 1) in vec3 vertnormal_camera_fs;

out vec4 fColor;

void main() {


  fColor = vec4(1.0, 1.0, 0.0, 0.6);

}
