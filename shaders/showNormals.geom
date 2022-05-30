#version 450 core

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

layout(location = 0) in vec3 normals_in[];

layout (std140, binding = 0) uniform CameraMatrices {
  mat4 projection;
  mat4 view;
};

float magnitude = 0.5;

void generateLine(uint vertex) {
    gl_Position = projection * view * gl_in[vertex].gl_Position;
    EmitVertex();
    gl_Position = projection * view * (gl_in[vertex].gl_Position + vec4(normalize(normals_in[vertex]), 0.0) * magnitude);
    EmitVertex();
    EndPrimitive();
}

void main(){
    generateLine(0);
    generateLine(1);
    generateLine(2);
}
