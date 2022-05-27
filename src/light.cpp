
#include <stdio.h>
#include <glm/glm.hpp>

#include "light.hpp"

unsigned int PointLightSet::addLight(PointLight light)
{
    lights.push_back(light);
    bufferSizeChanged = true;
    return lights.size() - 1; // Return index of newly added light
}

void PointLightSet::setLight(size_t index, PointLight light)
{
    if (index < 0 || index >= getSize())
    {
        fprintf(stderr, "Index out of bounds");
        return;
    }

    lights[index] = light;
}

size_t PointLightSet::getSize()
{
    return lights.size();
}

void PointLightSet::update()
{
    GLuint lightCount = getSize();
    GLsizeiptr bufferSize = SCALAR_MEMBER_SIZE + lightCount * sizeof(PointLight);
    if (bufferSizeChanged)
    {
        if (hasValidBuffer)
        {
            glDeleteBuffers(1, &bufferID);
            hasValidBuffer = false;
        }
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
        glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        hasValidBuffer = true;
        bufferSizeChanged = false;
    }
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, bufferID, 0, bufferSize);
    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightCount), &lightCount);
    glBufferSubData(GL_UNIFORM_BUFFER, SCALAR_MEMBER_SIZE, lightCount * sizeof(PointLight), &lights[0]);
};
