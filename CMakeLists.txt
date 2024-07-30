cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

file(GLOB raytracer_src CONFIGURE_DEPENDS 
    "src/*.h" 
    "src/*.c" 
    "src/*.cpp" 
    "src/imgui/*.h" 
    "src/imgui/*.cpp" 
    "src/glad/*.h" 
    "src/glad/*.c" 
    "src/glfw/*.h" 
    "src/glfw/*.cpp" 
    "src/glm/*.hpp" 
    "src/glm/*.cpp" 
    "src/glm/gtc/*.hpp" 
    "src/glm/gtc/*.cpp"
)
add_executable(raytracer ${raytracer_src})

target_link_libraries(raytracer 
    glfw 
    OpenGL::GL
)

set_target_properties(raytracer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
