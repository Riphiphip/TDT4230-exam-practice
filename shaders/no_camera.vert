#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

layout(location = 0) out vec3 normal_out;
layout(location = 1) out vec2 uv_out;
layout(location = 2) out vec3 position_out;

uniform mat4 model;
uniform mat3 normalMat;

void main(){
  normal_out = normalize(normalMat * vertexNormal);
  uv_out = vertexUV;
  gl_Position = model * vec4(vertexPosition_modelspace, 1.0);
  position_out = vec3(gl_Position);
}
