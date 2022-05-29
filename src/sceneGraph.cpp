
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
    glUseProgram(programID);
    mat4 modelMat = parentTransform * localTransform;
    GLuint modelU = glGetUniformLocation(programID, modelMatUniformName.c_str());
    glUniformMatrix4fv(modelU, 1, GL_FALSE, value_ptr(modelMat));

    mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));
    GLuint normalMatU = glGetUniformLocation(programID, normalMatUniformName.c_str());
    glUniformMatrix3fv(normalMatU, 1, GL_FALSE, value_ptr(normalMat));

    for (unsigned int i = 0; i < mesh.textures.size(); ++i){
        GLuint textureID = mesh.textures[i].id;
        glBindTextureUnit(i, textureID);
        std::string textureUName = textureListUniformName + "[";
        textureUName += std::to_string(i);
        textureUName += "]";
        GLuint textureU = glGetUniformLocation(programID, textureUName.c_str());
        glUniform1i(textureU, i);
    }

    mesh.draw();

    for (scene::Node *child : this->children)
    {
        child->render(modelMat);
    }
}