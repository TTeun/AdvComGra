#version 410
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 4) out;

layout (location = 1) out vec3 out_color;

float sq(float t){
  return t * t;
}

vec3 toColor(float col)
{ // Turn the float col into a color
  // This map is not one-to-one, i.e., blue represents both high and low curvature
  // but the color is meant purely a visual aid to see the smoothness of a curve

  if (col < 0.33)
    return vec3(3 * col, 0, 1 - 3 * col); // col < 0.33

  col -= 0.33;
  if (col < 0.33)
    return vec3(1 - 3 * col, 3 * col, 0); // col < 0.66

  col -= 0.33;
    return vec3(0, 1 - 3 * col, 3 * col); // col > 0.66
}

void main() {

  // Find the curvature k
  vec4 a = gl_in[2].gl_Position - gl_in[1].gl_Position;
  vec4 b = gl_in[0].gl_Position - gl_in[1].gl_Position;
  vec4 c = gl_in[2].gl_Position - gl_in[0].gl_Position;

  float alpha = dot(normalize(a), normalize(b));
  float angle = acos(alpha);
  float k = 2 * sin(angle) / sqrt(dot(c,c));

  float col = k / 8.0; // Some scaling to make the result more visible
  col = sqrt(col);

  out_color = toColor(col);

  gl_Position = gl_in[1].gl_Position;
  EmitVertex();

  // And repeat for the next vertex
  a = gl_in[3].gl_Position - gl_in[2].gl_Position;
  b = gl_in[1].gl_Position - gl_in[2].gl_Position;
  c = gl_in[3].gl_Position - gl_in[1].gl_Position;

  alpha = dot(normalize(a), normalize(b));
  angle = acos(alpha);
  k = 2 * sin(angle) / sqrt(dot(c,c));

  col = k / 8.0;
  col = sqrt(col);

  out_color = toColor(col);

  gl_Position = gl_in[2].gl_Position;
  EmitVertex();
  EndPrimitive();

}
