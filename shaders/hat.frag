#version 430 core

layout(location = 0) in vec3 normal_in;
layout(location = 1) in vec2 uv_in;

out vec4 color;

struct PointLight {
  float intensity;
  vec3 color;
  vec3 position;
};

layout (std140, binding = 1) uniform PointLights {
  uint pointLightCount;
  PointLight pointLights[];
};

void main(){
    vec3 normal = normal_in; 
    float shadowCoef = max(dot(normal, vec3(0.5, 1.0, 0.5)), 0.3);
    vec3 col = vec3(pointLights[1].color);

    col *= shadowCoef;
    color = vec4(col, 1.0);
}
