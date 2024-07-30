cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(raytracer 
    src/main.cpp
    src/random.cpp
    src/shader.cpp
    src/scene.cpp
    src/glad.c
)

target_link_libraries(raytracer 
    glfw 
    OpenGL::GL
)

set_target_properties(raytracer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
