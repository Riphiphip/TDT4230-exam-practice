#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "meshUtil.hpp"

namespace scene
{
    // Empty scene node
    class Node
    {
    public:
        std::vector<Node *> children;
        glm::mat4 localTransform = mat4(1.0);
        virtual void render(mat4 parentTransform) const;

        virtual ~Node(){};
    };

    // Scene node containing a triangle mesh to be rendered.
    class MeshNode : public Node
    {
    public:
        Mesh mesh;
        GLuint programID;
        std::string modelMatUniformName = "model";
        std::string normalMatUniformName = "normalMat";
        std::string textureListUniformName = "textures";

        MeshNode(Mesh mesh, GLuint programID) : mesh{mesh}, programID{programID}
        {
        }

        void render(mat4 parentTransform) const override;
    };

    // Scene node containing a point light.
    class PointLightNode : public Node
    {
    public:
        float intensity;
        vec3 color;
    };
}