#include "scene.h"
#include <GLFW/glfw3.h>
#include <cmath>

void initializeUniforms(Shader shader) {
    shader.setFloat("INFINITY", INFINITY);
    shader.setFloat("u_fov", 90.0f);
    shader.setVec3("u_initCamPos", 0, 0, -3);
    shader.setVec3("u_initCamDir", 0, 0, 0);
    shader.setVec3("u_upDir", 0, 1, 0);

   /* for (int i = 0; i < 9; i++) {
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].center").c_str(), rng(-2, 2), rng(-1, 1), rng(2, 6));
        shader.setFloat(std::string("u_spheres[").append(std::to_string(i)).append("].radius").c_str(), rng(0.25f, 1));
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.color").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.emission").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
    }*/
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].center").c_str(), 0, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].mat.color").c_str(), 0.8f, 0.2f, 0.2f);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].mat.emission").c_str(), 1, 1, 1);

    shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].center").c_str(), 3, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(1)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].mat.color").c_str(), 0.2f, 0.8f, 0.2f);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].mat.emission").c_str(), 0, 0, 0);

    shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].center").c_str(), -3, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(2)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].mat.color").c_str(), 0.2f, 0.2f, 0.8f);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].mat.emission").c_str(), 0, 0, 0);
}

void updateUniforms(Shader shader) {
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].center").c_str(), 0, std::sin(glfwGetTime()), 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].mat.emissionStrength").c_str(), 4.0f * std::abs(0.3f + std::sin(glfwGetTime())));
}
