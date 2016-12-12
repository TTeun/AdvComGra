#version 410

layout (quads, equal_spacing) in;

layout (location = 0) in vec3 vertcoords_camera_tcs[];

layout (location = 0) out vec3 vertcoords_camera_fs;
layout (location = 3) out vec4 tePatchDistance;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y; // Coordinates in the unit (u,v) plane

    vec3 a = mix(vertcoords_camera_tcs[0], vertcoords_camera_tcs[1], u);
    vec3 b = mix(vertcoords_camera_tcs[2], vertcoords_camera_tcs[3], u);
    vertcoords_camera_fs = mix(a, b, v);

    tePatchDistance = vec4(u, v, 1-u, 1-v);

    gl_Position = projectionmatrix *  vec4(vertcoords_camera_fs.xyz, 1.0);
}
