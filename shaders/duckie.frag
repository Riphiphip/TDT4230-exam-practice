#version 430 core

layout(location = 0) in vec3 normal_in;
layout(location = 1) in vec2 uv_in;

out vec4 color;

void main(){
    vec3 normal = normal_in; 
    color = vec4(normal, 1.0);
}
