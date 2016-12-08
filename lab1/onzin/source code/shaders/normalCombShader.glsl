#version 410
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 4) out;

layout (location = 1) out vec3 out_color;

float sq(float t){
  return t * t;
}

void main() {
  // Get curvature
  vec4 a = gl_in[2].gl_Position - gl_in[1].gl_Position;
  vec4 b = gl_in[0].gl_Position - gl_in[1].gl_Position;
  vec4 c = gl_in[2].gl_Position - gl_in[0].gl_Position;

  float alpha = dot(normalize(a), normalize(b));
  float angle = acos(alpha);
  float k = 2 * sin(angle) / sqrt(dot(c,c));


  k = sqrt(k);
  if (k < 10){
    vec4 vertex = gl_in[1].gl_Position + gl_in[2].gl_Position;
    vertex *= 0.5;

    out_color = vec3(0.2, 0.2, 0.2);
    gl_Position = vertex; // Comb points start halfway an edge
    EmitVertex();

    vec4 dif = gl_in[1].gl_Position - gl_in[2].gl_Position;
    dif = 0.15 * normalize(dif);
    vertex.x += k * dif.y;
    vertex.y -= k * dif.x;  // Vertex is now normal to the edge
    out_color = vec3(0.2, 0.2, 0.2);
    gl_Position = vertex;
    EmitVertex();
    EndPrimitive();
  }

}
