#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>

using namespace std;

enum ShaderType
{
    COMPUTE = GL_COMPUTE_SHADER,
    VERTEX = GL_VERTEX_SHADER,
    TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
    TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
};

struct ShaderSource
{
    string name;
    string source;
    ShaderType type;
};

struct ShaderPath
{
    string path;
    ShaderType type;
};


GLuint createProgram(vector<ShaderSource> sources);
GLuint createProgram(vector<ShaderPath> paths);