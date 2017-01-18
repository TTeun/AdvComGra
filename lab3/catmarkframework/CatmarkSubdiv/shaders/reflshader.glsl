#version 410
// Fragment shader

layout (location = 0) in vec3 vertcoords_camera_fs;
layout (location = 1) in vec3 vertnormal_camera_fs;

uniform float refldensity;

out vec4 fColor;

bool onOff(float theta){
  return (sin(refldensity*theta) > 0);
}

void main() {

  vec3 normal;
  normal = normalize(vertnormal_camera_fs);
  vec3 v = vec3(0.0, 0.0, -1.0);
  vec3 r = 2 * (dot(normal, v) * normal - v);
  vec3 a = vec3(0.0, 1.0, 0.0);
  vec3 d = r - dot(r,a) * a;
  vec3 va = normalize(v - dot(v,a) * a);
  vec3 a_perp = cross(a,v);
  float theta = atan(dot(dot(r, a_perp), dot(r, va)));

  fColor = onOff(theta) ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);

}
