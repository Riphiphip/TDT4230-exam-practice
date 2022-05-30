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
#include "light.hpp"
#include "bezier.hpp"

using namespace glm;

static unsigned int windowWidth = 500;
static unsigned int windowHeight = 500;
void windowSizeCallback(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

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
    window = glfwCreateWindow(windowWidth, windowHeight, "Exam practice", NULL, NULL);
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

    glfwSetWindowSizeCallback(window, windowSizeCallback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);

    vector<ShaderPath> normalsProgramShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/showNormals.geom",
            .type = GEOMETRY,
        },
        {
            .path = "../shaders/normals.frag",
            .type = FRAGMENT,
        },
    };
    GLuint normalsProgram = createProgram(normalsProgramShaders);

    // Actual scene
    scene::Node sceneRoot;

    vector<ShaderPath> tvBodyShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/tv_body.frag",
            .type = FRAGMENT,
        },
    };
    GLuint tvBodyProgram = createProgram(tvBodyShaders);
    Mesh tvBodyMesh = loadObjFile("../models/TV_body.obj");
    scene::MeshNode tvBodyNode(tvBodyMesh, tvBodyProgram);
    {
        mat4 tvInitialTransform = mat4(1.0);
        // tvInitialTransform = translate(tvInitialTransform, vec3(0.0, 0.0, -5.0));
        tvInitialTransform = rotate(tvInitialTransform, radians(-90.0f), vec3(0.0, 1.0, 0.0));
        tvBodyNode.localTransform = tvInitialTransform;
    }
    sceneRoot.children.push_back(&tvBodyNode);

    vector<ShaderPath> tvScreenShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/screen.frag",
            .type = FRAGMENT,
        },
    };
    GLuint tvScreenProgram = createProgram(tvScreenShaders);
    Mesh tvScreenMesh = loadObjFile("../models/TV_screen.obj");

    GLuint tvScreenWidth = 1000;
    GLuint tvScreenHeight = 1000;
    GLuint tvScreenFrameBuffer;
    glGenFramebuffers(1, &tvScreenFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, tvScreenFrameBuffer);

    GLuint tvScreenTexture;
    glGenTextures(1, &tvScreenTexture);
    tvScreenMesh.textures.push_back({.id = tvScreenTexture, .type = "Framebuffer texture"});
    glBindTexture(GL_TEXTURE_2D, tvScreenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tvScreenWidth, tvScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tvScreenTexture, 0);

    GLuint tvScreenRenderBuffer;
    glGenRenderbuffers(1, &tvScreenRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, tvScreenRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, tvScreenWidth, tvScreenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, tvScreenRenderBuffer);

    scene::MeshNode tvScreenNode(tvScreenMesh, tvScreenProgram);
    tvBodyNode.children.push_back(&tvScreenNode);

    PointLightSet pointLights;

    PointLight light1;
    light1.color = vec3(1.0);
    light1.intensity = 10.0;
    light1.position = vec3(-5.0, -10.0, 10.0);

    pointLights.addLight(light1);

    // Scene on TV
    scene::Node tvShowRoot;

    vector<ShaderPath> duckieShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/duckie.frag",
            .type = FRAGMENT,
        },
    };
    GLuint duckieProgram = createProgram(duckieShaders);
    Mesh duckieMesh = loadObjFile("../models/duckie.obj");
    scene::MeshNode duckieNode(duckieMesh, duckieProgram);
    duckieNode.localTransform = translate(mat4(1.0), vec3(0.0, -1.5, -10.0));

    tvShowRoot.children.push_back(&duckieNode);

    vector<ShaderPath> hatShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/hat.frag",
            .type = FRAGMENT,
        },
    };
    GLuint hatProgram = createProgram(hatShaders);
    Mesh hatMesh = loadObjFile("../models/top_hat.obj");
    scene::MeshNode hatNode(hatMesh, hatProgram);
    hatNode.localTransform = scale(translate(mat4(1.0), vec3(0.0, 2.6, 0.0)), vec3(0.7));

    duckieNode.children.push_back(&hatNode);

    scene::MeshNode duckieNormalsNode(duckieMesh, normalsProgram);
    duckieNode.children.push_back(&duckieNormalsNode);

    vector<ShaderPath> bezierShaders{
        {
            .path = "../shaders/default.vert",
            .type = VERTEX,
        },
        {
            .path = "../shaders/normals.frag",
            .type = FRAGMENT,
        },
    };
    GLuint bezierProgram = createProgram(bezierShaders);

    vector<Vertex> controlPoints{
        {
            .position = vec3(2.0, 2.0, 0.0),
            .normal = vec3(0.0),
            .textureCoords = vec2(-1.0, -1.0),
        },
        {
            .position = vec3(0.0, -2.0, 0.0),
            .normal = vec3(0.0),
            .textureCoords = vec2(0.0, 1.0),
        },
        {
            .position = vec3(-2.0, 2.0, 0.0),
            .normal = vec3(0.0),
            .textureCoords = vec2(1.0, -1.0),
        },
        {
            .position = vec3(-1.0, 3.0, 0.0),
            .normal = vec3(0.0),
            .textureCoords = vec2(1.0, -1.0),
        },
    };

    Mesh bezierMesh = createBezierMesh(controlPoints, 100);
    scene::MeshNode bezierNode(bezierMesh, bezierProgram);

    hatNode.children.push_back(&bezierNode);

    PointLightSet tvShowPointLights;

    PointLight light2;
    light2.color = vec3(1.0);
    light2.intensity = 10.0;
    light2.position = vec3(20.0, 10.0, 10.0);

    tvShowPointLights.addLight(light2);

    GLsizeiptr camMatBufSize = 2 * sizeof(mat4);
    GLuint camMatBlockBinding = 0;
    GLuint camMatBufID;
    glGenBuffers(1, &camMatBufID);
    glBindBuffer(GL_UNIFORM_BUFFER, camMatBufID);
    glBufferData(GL_UNIFORM_BUFFER, camMatBufSize, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, camMatBlockBinding, camMatBufID, 0, camMatBufSize);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do
    {
        // Render TV shows
        glBindFramebuffer(GL_FRAMEBUFFER, tvScreenFrameBuffer);
        glViewport(0, 0, tvScreenWidth, tvScreenHeight);
        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_UNIFORM_BUFFER, camMatBufID);
        mat4 tvShowProjectionMat = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(tvShowProjectionMat));
        mat4 tvShowViewMat = mat4(1.0);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(tvShowViewMat));

        tvShowPointLights.update();

        tvShowRoot.render(mat4(1.0));

        // Render main scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.5, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_UNIFORM_BUFFER, camMatBufID);

        mat4 projectionMat = perspective(radians(45.0f), float(windowHeight) / windowWidth, 0.1f, 100.0f);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(projectionMat));

        mat4 viewMat = lookAt(vec3(0.0, 0.0, 6.0), vec3(0.0), vec3(0.0, 1.0, 0.0));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(viewMat));

        bezierNode.localTransform = rotate(mat4(1.0), float(glfwGetTime()), vec3(0.0, 1.0, 0.0));

        pointLights.update();

        sceneRoot.render(mat4(1.0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
}