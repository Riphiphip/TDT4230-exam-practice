#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

layout(location = 0) out vec3 normal_out;
layout(location = 1) out vec2 uv_out;
layout(location = 2) out vec3 position_out;

layout (std140, binding = 0) uniform CameraMatrices {
  mat4 projection;
  mat4 view;
};

uniform mat4 model;
uniform mat3 normalMat;

void main(){
  normal_out = normalize(normalMat * vertexNormal);
  gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0);
  position_out = vec3(gl_Position);
}
