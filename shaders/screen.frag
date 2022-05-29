#version 450 core

layout(location = 0) in vec3 normal_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 position_in;

out vec4 color;

uniform sampler2D textures[3];

vec3 getDiffuse(vec2 uv) {
  vec4 texColor = texture(textures[0], uv);
  uv.y += sin(10.0 * uv.x)/10.0;
  bool inBand = abs(mod(uv.y-0.5, 0.1)) < 0.05;
  return vec3(texColor);
}

void main(){
  vec3 normal = normalize(normal_in);
  vec2 uv = uv_in;

  float diffuseCoef = 0.001;
  vec3 diffuseCol = getDiffuse(uv);

  vec3 diffuse = diffuseCol;
  vec3 col = diffuse;
  color = vec4(col, 1.0);
}
