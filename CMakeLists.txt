cmake_minimum_required(VERSION 3.5.0)
project(raytracer_reloaded VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(raytracer_reloaded 
    src/main.cpp
    src/glad.c
)

target_link_libraries(raytracer_reloaded 
    glfw 
    OpenGL::GL
)

set_target_properties(raytracer_reloaded PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
