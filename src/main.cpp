#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "meshUtil.hpp"
#include "shaderUtil.hpp"
#include "sceneGraph.hpp"

using namespace glm;

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Could not initialize glfw.\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(500, 500, "Exam practice", NULL, NULL);
    if (window == nullptr)
    {
        fprintf(stderr, "Failed to open glfw window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Could not initialize GLEW\n");
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);

    scene::Node sceneRoot;

    vector<ShaderPath> duckieShaders{
        {.path = "../shaders/default.vert",
         .type = VERTEX},
        {.path = "../shaders/duckie.frag",
         .type = FRAGMENT},
    };

    Mesh duckie = loadObjFile("../models/duckie.obj");
    GLuint duckieProgram = createProgram(duckieShaders);
    scene::MeshNode duckieNode(duckie, duckieProgram);
    
    sceneRoot.children.push_back(&duckieNode);

    Mesh hat = loadObjFile("../models/top_hat.obj");
    scene::MeshNode hatNode(hat, duckieProgram);
    mat4 hatMat = mat4(1.0);
    hatMat = scale(hatMat, vec3(0.8));
    hatMat = translate(hatMat, vec3(0.0, 3.3, 0.0));
    hatNode.localTransform = hatMat;
    duckieNode.children.push_back(&hatNode);


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(duckieProgram);
        fmat4 projectionMat = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);

        GLuint projectionU = glGetUniformLocation(duckieProgram, "projection");
        glUniformMatrix4fv(projectionU, 1, GL_FALSE, value_ptr(projectionMat));

        mat4 modelMat = scale(mat4(1.0), vec3(1.0));
        modelMat = translate(modelMat, vec3(0.0, -0.5, -10.0));
        modelMat = rotate(modelMat, radians((float)(10 * glfwGetTime())), vec3(0.5f, 1.0f, 0.0f));
        duckieNode.localTransform = modelMat;

        sceneRoot.render(mat4(1.0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
}