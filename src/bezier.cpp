
#include "bezier.hpp"

Mesh createBezierMesh(std::vector<Vertex> controlPoints, unsigned int nSegments)
{
    std::vector<Texture> textures;
    std::vector<Vertex> vertices;

    unsigned int nPoints = controlPoints.size();

    if (nPoints < 3)
    {
        fprintf(stderr, "Insufficient number of control points\n");
    }

    float t = 0.0;
    float dt = 1.0 / nSegments;

    while (t <= 1.0)
    {
        std::vector<Vertex> deCasPoints = controlPoints;
        for (int deCasLength = nPoints; deCasLength > 0; --deCasLength)
        {
            for (int i = 0; i < deCasLength-1; ++i)
            {
                deCasPoints[i].position = (1-t) * deCasPoints[i].position + t * deCasPoints[i+1].position;
                deCasPoints[i].normal = (1-t) * deCasPoints[i].normal + t * deCasPoints[i+1].normal;
                deCasPoints[i].textureCoords = (1-t) * deCasPoints[i].textureCoords + t * deCasPoints[i+1].textureCoords;
            }
        }
        vertices.push_back(deCasPoints[0]);
        t += dt;
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < vertices.size(); ++i){
        indices.push_back(i);
    }

    Mesh bezierMesh(vertices, indices, textures);
    bezierMesh.drawType = GL_LINE_STRIP;
    return bezierMesh;
}