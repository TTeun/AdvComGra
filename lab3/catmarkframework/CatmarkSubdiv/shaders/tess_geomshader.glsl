#version 410

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec3 vertcoords_camera_tes[3];
layout (location = 3) in vec4 tePatchDistance[3];


layout (location = 1) out vec3 vertnormal_camera_gs;
layout (location = 3) out vec4 gPatchDistance;

void main()
{
    vec3 A = vertcoords_camera_tes[2] - vertcoords_camera_tes[0];
    vec3 B = vertcoords_camera_tes[1] - vertcoords_camera_tes[0];
    vertnormal_camera_gs = normalmatrix * normalize(cross(A, B));

    gPatchDistance = tePatchDistance[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
