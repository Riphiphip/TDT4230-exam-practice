#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec3 normals_in[];

layout(location = 1) out vec2 uv_out;

layout (std140, binding = 0) uniform CameraMatrices {
  mat4 projection;
  mat4 view;
};

uniform float grassWidth = 0.2;
uniform float grassHeight = 0.5;

vec3 findPerpendicular(vec3 vector){
    vec3 candidate1 = normalize(cross(vector, vec3(1.0, 0.0, 0.0)));
    vec3 candidate2 = normalize(cross(vector, vec3(0.0, 0.0, 1.0)));
    
    return candidate1 != vec3(0.0) ? candidate1 : candidate2;
}

void generateBlade(vec3 p, vec3 normal) {
    vec3 tangent = findPerpendicular(normal);

    uv_out = vec2(1.0, 0.0);
    gl_Position = projection*view * vec4(p + tangent * grassWidth/2.0, 1.0);
    EmitVertex();

    uv_out = vec2(0.0, 0.0);
    gl_Position = projection*view * vec4(p - tangent * grassWidth/2.0, 1.0);
    EmitVertex();

    uv_out = vec2(0.5, 1.0);
    gl_Position = projection*view * vec4(p + normal * grassHeight, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main(){
    // // Not really a tangent. Just perpendicular to the normal.
    // for (int i = 0; i < gl_in.length(); ++i){
    //     generateBlade(vec3(gl_in[i].gl_Position), normals_in[i]);
    // }
    generateBlade(vec3(gl_in[0].gl_Position), normals_in[0]);
}
