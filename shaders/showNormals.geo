#version 450 core

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

layout(location = 0) in vec3 normals_in[];

float magnitude = 1.0;

void generateLine(uint vertex) {
    gl_Position = gl_in[vertex].gl_Position;
    EmitVertex();
    gl_Position = (gl_in[vertex].gl_Position + vec4(normalize(normals_in[vertex]), 0.0) * magnitude);
    EmitVertex();
    EndPrimitive();
}

void main(){
    generateLine(0);
    generateLine(1);
    generateLine(2);
}
