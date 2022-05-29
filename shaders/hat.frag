#version 450 core

layout(location = 0) in vec3 normal_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 position_in;

out vec4 color;

struct PointLight {
  float intensity;
  vec3 color;
  vec3 position;
};

layout (std140, binding = 1) uniform PointLights {
  uint pointLightCount;
  PointLight pointLights[100];
};

float attenuate(float dist){
  return pow(1.0/(dist+1.0), 2.0);
}

void main(){
  vec3 normal = normalize(normal_in);

  float diffuseCoef = 0.001;
  vec3 diffuseCol = vec3(0.1);

  vec3 diffuse = vec3(0.0);

  for (int i = 0; i < pointLightCount; ++i){
    PointLight light = pointLights[i];
    float distToLight = length(light.position - position_in);
    float attenuation = 1.0/attenuate(distToLight);

    vec3 lightDir = normalize(light.position - position_in);
    diffuse += diffuseCoef *  attenuation * light.intensity * max(dot(normal, lightDir), 0.0) * diffuseCol;
  }

  vec3 col = diffuse;
  color = vec4(col, 1.0);
}
