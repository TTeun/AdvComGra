#version 410

layout (vertices = 16) out;

layout (location = 0) in vec3 vertcoords_camera_vs[];

layout (location = 0) out vec3 vertcoords_camera_te[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main()
{
  vertcoords_camera_te[gl_InvocationID] = vertcoords_camera_vs[gl_InvocationID];

  if (gl_InvocationID == 0)
  {
      gl_TessLevelInner[0] = TessLevelInner;
      gl_TessLevelInner[1] = TessLevelInner;

      gl_TessLevelOuter[0] = TessLevelOuter;
      gl_TessLevelOuter[1] = TessLevelOuter;
      gl_TessLevelOuter[2] = TessLevelOuter;
      gl_TessLevelOuter[3] = TessLevelOuter;
    }
}
