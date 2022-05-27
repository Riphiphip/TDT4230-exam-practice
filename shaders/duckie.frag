#version 430 core

layout(location = 0) in vec3 normal_in;
layout(location = 1) in vec2 uv_in;

out vec4 color;

void main(){
    vec3 normal = normal_in; 
    float shadowCoef = max(dot(normal, vec3(0.5, 1.0, 0.5)), 0.3);
    vec3 col = normal * shadowCoef;
    color = vec4(col, 1.0);
}
