cmake_minimum_required(VERSION 3.18)
project(exam_practice)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(app
    src/main.cpp
    src/meshUtil.cpp
    src/shaderUtil.cpp
    src/sceneGraph.cpp
    src/light.cpp
    src/bezier.cpp
)

set(LIBS 
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
    ${GLM_LIBRARIES}
)


target_link_libraries(app
    ${LIBS}
)


