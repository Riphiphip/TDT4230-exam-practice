#include <GL/glew.h>

#include <meshUtil.hpp>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, indices.size());
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, textureCoords));
}

Mesh loadObjFile(string path){
    objl::Loader loader;
    loader.LoadFile(path);

    vector<Vertex> vertices;
    for (objl::Vertex v_old: loader.LoadedVertices){
        Vertex v;
        v.position = vec3(v_old.Position.X, v_old.Position.Y ,v_old.Position.Z);
        v.normal = vec3(v_old.Normal.X, v_old.Normal.Y ,v_old.Normal.Z);
        v.textureCoords = vec2(v_old.TextureCoordinate.X, v_old.TextureCoordinate.Y);
        vertices.push_back(v);
    }

    vector<Texture> textures;

    return Mesh(vertices, loader.LoadedIndices, textures);
}