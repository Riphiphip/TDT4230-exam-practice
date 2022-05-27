
#include <glm/gtc/type_ptr.hpp>

#include "sceneGraph.hpp"

void scene::Node::render(mat4 parentTransform) const
{
    mat4 currentTransform = parentTransform * this->localTransform;
    for (scene::Node *child : this->children)
    {
        child->render(currentTransform);
    }
}

void scene::MeshNode::render(mat4 parentTransform) const
{
    glUseProgram(this->programID);

    mat4 modelMat = parentTransform * this->localTransform;
    GLuint modelU = glGetUniformLocation(this->programID, this->modelMatUniformName.c_str());
    glUniformMatrix4fv(modelU, 1, GL_FALSE, value_ptr(modelMat));

    mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));
    GLuint normalMatU = glGetUniformLocation(this->programID, this->normalMatUniformName.c_str());
    glUniformMatrix3fv(normalMatU, 1, GL_FALSE, value_ptr(normalMat));

    this->mesh.draw();

    for (scene::Node *child : this->children)
    {
        child->render(modelMat);
    }
}