#include <ios>
#include <fstream>
#include <sstream>

#include "shaderUtil.hpp"

GLuint createProgram(vector<ShaderSource> sources)
{
    GLuint programID = glCreateProgram();

    vector<GLuint> shaderIDs;
    for (ShaderSource source : sources)
    {
        printf("Compiling %s.\n", source.name.c_str());
        GLuint shaderID = glCreateShader(source.type);
        char const *sourcePtr = source.source.c_str();
        glShaderSource(shaderID, 1, &sourcePtr, NULL);
        glCompileShader(shaderID);

        GLint result = GL_FALSE;
        int infoLogLength;

        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            std::vector<char> shaderErrorMessage(infoLogLength + 1);
            glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
            printf("%s\n", &shaderErrorMessage[0]);
        }

        glAttachShader(programID, shaderID);

        shaderIDs.push_back(shaderID);
    }

    glLinkProgram(programID);
    
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

    for (GLuint shaderID: shaderIDs){
        glDetachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }

    return programID;
}

GLuint createProgram(vector<ShaderPath> paths){
    vector<ShaderSource> sources;

    for (ShaderPath path: paths){
        ShaderSource source;
        ifstream shaderStream(path.path, ios::in);
        if (shaderStream.is_open()){
            stringstream sstr;
            sstr << shaderStream.rdbuf();
            source.source = sstr.str();
            shaderStream.close();
        } else {
            fprintf(stderr, "Could not open shader source file %s.", path.path.c_str());
        }
        source.name = path.path;
        source.type = path.type;
        sources.push_back(source);
    }

    return createProgram(sources);
}