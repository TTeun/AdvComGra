#version 410

layout (quads, equal_spacing) in;

layout (location = 0) in vec3 vertcoords_camera_tcs[];

layout (location = 0) out vec3 vertcoords_camera_fs;
layout (location = 3) out vec4 tePatchDistance;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y; // Coordinates in the unit (u,v) plane



    vec3 a = mix(vertcoords_camera_tcs[5], vertcoords_camera_tcs[6], u);
    vec3 b = mix(vertcoords_camera_tcs[9], vertcoords_camera_tcs[10], u);
    gl_Position = projectionmatrix * vec4(mix(a, b, v), 1.0);



//    mat3 Px = mat3(vertcoords_camera_tcs[1].x, vertcoords_camera_tcs[2].x, vertcoords_camera_tcs[3].x,
//                   vertcoords_camera_tcs[0].x, vertcoords_camera_tcs[8].x, vertcoords_camera_tcs[4].x,
//                   vertcoords_camera_tcs[7].x, vertcoords_camera_tcs[6].x, vertcoords_camera_tcs[5].x );

//    mat3 Py = mat3(vertcoords_camera_tcs[1].y, vertcoords_camera_tcs[2].y, vertcoords_camera_tcs[3].y,
//                   vertcoords_camera_tcs[0].y, vertcoords_camera_tcs[8].y, vertcoords_camera_tcs[4].y,
//                   vertcoords_camera_tcs[7].y, vertcoords_camera_tcs[6].y, vertcoords_camera_tcs[5].y );

//    mat3 Pz = mat3(vertcoords_camera_tcs[1].z, vertcoords_camera_tcs[2].z, vertcoords_camera_tcs[3].z,
//                   vertcoords_camera_tcs[0].z, vertcoords_camera_tcs[8].z, vertcoords_camera_tcs[4].z,
//                   vertcoords_camera_tcs[7].z, vertcoords_camera_tcs[6].z, vertcoords_camera_tcs[5].z );

//    mat3 M = 0.5 *mat3(1,  1, 0,
//                        -2, 2, 0,
//                        1, -2, 1);

//    mat3 MT = transpose(M);

//    vec3 U = vec3(u * u, u, 1);
//    vec3 V = vec3(v * v, v, 1);

//    float X = dot(M * Px * MT * V, U);
//    float Y = dot(M * Py * MT * V, U);
//    float Z = dot(M * Pz * MT * V, U);

//    vertcoords_camera_fs = vec3(X, Y, Z);

    tePatchDistance = vec4(u, v, 1-u, 1-v);

   // gl_Position = projectionmatrix *  vec4(vertcoords_camera_fs.xyz, 1.0);
}
