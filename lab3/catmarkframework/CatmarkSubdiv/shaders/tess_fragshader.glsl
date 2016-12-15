#version 410
// Fragment shader

uniform bool showGridLines;

layout (location = 1) in vec3 vertnormal_camera_fs;
layout (location = 3) in vec4 gPatchDistance;

out vec4 fColor;

void main() {
  // Same as main fragment shader but now we get patchDistance from the geom shader to determine if we are on the side of a quad or inside (for showGridLines)

  vec3 lightpos = vec3(3.0, 0.0, 2.0);
  vec3 lightcolour = vec3(1.0);
  float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);

  vec3 matcolour;
  if (d2 == 0 && showGridLines)
    matcolour = vec3(0.0, 1.0, 0.0);
  else
    matcolour = vec3(0.53, 0.80, 0.87);

  vec3 matspeccolour = vec3(1.0);

  float matambientcoeff = 0.2;
  float matdiffusecoeff = 0.6;
  float matspecularcoeff = 0.4;

  vec3 normal;
  normal = normalize(vertnormal_camera_fs);

  vec3 surftolight = normalize(lightpos - vertnormal_camera_fs);
  float diffusecoeff = max(0.0, dot(surftolight, normal));

  vec3 camerapos = vec3(0.0);
  vec3 surftocamera = normalize(camerapos - vertnormal_camera_fs);
  vec3 reflected = 2 * diffusecoeff * normal - surftolight;
  float specularcoeff = max(0.0, dot(reflected, surftocamera));

  vec3 compcolour = min(1.0, matambientcoeff + matdiffusecoeff * diffusecoeff) * lightcolour * matcolour;
       compcolour += matspecularcoeff * specularcoeff * lightcolour * matspeccolour;

  fColor = vec4(compcolour, 1.0);


}
