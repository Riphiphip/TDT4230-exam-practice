#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "../lib/OBJ_Loader.hpp"

using namespace std;
using namespace glm;

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 textureCoords;
};

struct Texture
{
    GLuint id;
    string type;
};

class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    GLenum drawType = GL_TRIANGLES;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    Mesh();
    void draw() const;

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

Mesh loadObjFile(string path);
