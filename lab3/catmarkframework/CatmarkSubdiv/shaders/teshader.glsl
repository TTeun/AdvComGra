#version 410

layout(quads, equal_spacing) in;

layout (location = 0) in vec3 vertcoords_camera_tcs[];
layout (location = 1) in vec3 vertnormal_camera_tcs[];

layout (location = 0) out vec3 vertcoords_camera_fs;
layout (location = 1) out vec3 vertnormal_camera_fs;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y;

    vec3 a = mix(vertcoords_camera_tcs[0], vertcoords_camera_tcs[1], u);
    vec3 b = mix(vertcoords_camera_tcs[2], vertcoords_camera_tcs[3], u);
    vertcoords_camera_fs = mix(a, b, v);

    a = mix(vertnormal_camera_tcs[0], vertnormal_camera_tcs[1], u);
    b = mix(vertnormal_camera_tcs[2], vertnormal_camera_tcs[3], u);
    vertnormal_camera_fs = mix(a, b, v);

    gl_Position = projectionmatrix *  vec4(vertcoords_camera_fs.xyz, 1.0);
}
