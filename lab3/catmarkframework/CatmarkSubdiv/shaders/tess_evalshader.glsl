#version 410

layout (quads, equal_spacing) in;

layout (location = 0) in vec3 vertcoords_camera_tcs[];

layout (location = 0) out vec3 vertcoords_camera_fs;
layout (location = 3) out vec4 tePatchDistance;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

float oneDCubic(float p0, float p1, float p2, float p3, float t) // Cubic interpolation of a curve
{
    float a = 0.5 * (-p0  + 3.0 * (p1 - p2) + p3);
    float b = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    float c = 0.5 * (-p0 + p2);

    return a*t*t*t + b*t*t + c*t + p1; // The cubic polynomial
}

float twoDCubic(mat4 p, float u, float v) // Cubic interpolation on a surface
{
    vec4 tmp;
    // Cubic interpolation along u position
    tmp[0] = oneDCubic(p[0][0], p[1][0], p[2][0], p[3][0], u);
    tmp[1] = oneDCubic(p[0][1], p[1][1], p[2][1], p[3][1], u);
    tmp[2] = oneDCubic(p[0][2], p[1][2], p[2][2], p[3][2], u);
    tmp[3] = oneDCubic(p[0][3], p[1][3], p[2][3], p[3][3], u);
    return oneDCubic(tmp[0], tmp[1], tmp[2], tmp[3], v); // cubic interpolation along v position
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y; // Coordinates in the unit (u,v) plane

    tePatchDistance = vec4(u, v, 1-u, 1-v);

    mat4 Px = mat4(
        vertcoords_camera_tcs[0].x, vertcoords_camera_tcs[1].x, vertcoords_camera_tcs[2].x, vertcoords_camera_tcs[3].x,
        vertcoords_camera_tcs[4].x, vertcoords_camera_tcs[5].x, vertcoords_camera_tcs[6].x, vertcoords_camera_tcs[7].x,
        vertcoords_camera_tcs[8].x, vertcoords_camera_tcs[9].x, vertcoords_camera_tcs[10].x, vertcoords_camera_tcs[11].x,
        vertcoords_camera_tcs[12].x, vertcoords_camera_tcs[13].x, vertcoords_camera_tcs[14].x, vertcoords_camera_tcs[15].x
        );

    mat4 Py = mat4(
        vertcoords_camera_tcs[0].y, vertcoords_camera_tcs[1].y, vertcoords_camera_tcs[2].y,vertcoords_camera_tcs[3].y,
        vertcoords_camera_tcs[4].y, vertcoords_camera_tcs[5].y, vertcoords_camera_tcs[6].y,vertcoords_camera_tcs[7].y,
        vertcoords_camera_tcs[8].y, vertcoords_camera_tcs[9].y, vertcoords_camera_tcs[10].y,vertcoords_camera_tcs[11].y,
        vertcoords_camera_tcs[12].y, vertcoords_camera_tcs[13].y, vertcoords_camera_tcs[14].y,vertcoords_camera_tcs[15].y
        );

    mat4 Pz = mat4(
        vertcoords_camera_tcs[0].z, vertcoords_camera_tcs[1].z, vertcoords_camera_tcs[2].z,vertcoords_camera_tcs[3].z,
        vertcoords_camera_tcs[4].z, vertcoords_camera_tcs[5].z, vertcoords_camera_tcs[6].z,vertcoords_camera_tcs[7].z,
        vertcoords_camera_tcs[8].z, vertcoords_camera_tcs[9].z, vertcoords_camera_tcs[10].z,vertcoords_camera_tcs[11].z,
        vertcoords_camera_tcs[12].z, vertcoords_camera_tcs[13].z, vertcoords_camera_tcs[14].z,vertcoords_camera_tcs[15].z
        );

    // Cubic Hermite interpolation, Ensure C1 continuity for all quads surrounded by quads.
    float x = twoDCubic(Px, u, v);
    float y = twoDCubic(Py, u, v);
    float z = twoDCubic(Pz, u, v);

    vec3 a = mix(vertcoords_camera_tcs[5], vertcoords_camera_tcs[6], u);
    vec3 b = mix(vertcoords_camera_tcs[9], vertcoords_camera_tcs[10], u);

    gl_Position = projectionmatrix * vec4(x, y, z, 1.0);
    vertcoords_camera_fs =vec3(x, y, z);

}
