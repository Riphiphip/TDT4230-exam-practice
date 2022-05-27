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

    Mesh cube = loadObjFile("../models/cube.obj");
    scene::MeshNode cubeNode(cube, duckieProgram);
    duckieNode.children.push_back(&cubeNode);


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

        mat4 cubeNodeMat = mat4(1.0);
        cubeNodeMat = translate(cubeNodeMat, vec3(0.0, 2.0 + sin(10.0f*glfwGetTime()), 0.0));
        cubeNodeMat = rotate(cubeNodeMat, radians((float)(30 * glfwGetTime())), vec3(1.0f, 0.0f, 0.0f));
        cubeNode.localTransform = cubeNodeMat;

        sceneRoot.render(mat4(1.0));

        // mat3 normalMat = transpose(inverse(mat3(modelMat)));
        // GLuint normalMatU = glGetUniformLocation(duckieProgram, "normalMat");
        // glUniformMatrix3fv(normalMatU, 1, GL_FALSE, value_ptr(normalMat));

        // duckie.draw();
        // // triangle.draw(duckieProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
}