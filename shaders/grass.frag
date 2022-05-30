#version 450 core

layout(location = 1) in vec2 uv_in;

out vec4 color;

void main(){
    vec3 col = uv_in.y * vec3(0.0, 1.0, 0.0);
    color = vec4(col, 1.0);
}
