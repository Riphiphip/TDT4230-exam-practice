#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

unsigned const int defaultPointLightBinding = 1;

#define SCALAR_MEMBER_SIZE 16

struct PointLight
{
    float intensity;
    float pad0;
    float pad1;
    float pad2;
    glm::vec3 color;
    float pad3;
    glm::vec3 position;
    float pad4;
};

class PointLightSet
{
public:
    unsigned int addLight(PointLight light);
    size_t getSize();
    void setLight(size_t index, PointLight);
    void clear();
    void update(); // Updates uniforms with values in set

private:
    GLuint bufferID;
    unsigned int binding = defaultPointLightBinding;
    bool hasValidBuffer = false; // Keep track of wether or not the buffer ID is initialized
    bool bufferSizeChanged = true;
    std::vector<PointLight> lights;
};