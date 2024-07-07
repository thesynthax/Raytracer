#include "scene.h"
#include <GLFW/glfw3.h>
#include <cmath>

void initializeUniforms(Shader shader) {
    shader.setFloat("INFINITY", INFINITY);
    shader.setFloat("u_fov", 70.0f);
    shader.setVec3("u_initCamPos", 0, 0, -1);
    shader.setVec3("u_initCamDir", 0, 0, 0);
    shader.setVec3("u_upDir", 0, 1, 0);

    /*for (int i = 0; i < 9; i++) {
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].center").c_str(), rng(-2, 2), rng(-1, 1), rng(2, 6));
        shader.setFloat(std::string("u_spheres[").append(std::to_string(i)).append("].radius").c_str(), rng(0.25f, 1));
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.color").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
        shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.emission").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
    }*/
    
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].center").c_str(), 0, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].mat.color").c_str(), 1.0f, 1.0f, 1.0f);
    shader.setInt(std::string("u_spheres[").append(std::to_string(0)).append("].mat.type").c_str(), 1);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].mat.fuzz").c_str(), 0.1f);

    shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].center").c_str(), 3, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(1)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].mat.color").c_str(), 0.2f, 0.8f, 0.2f);
    shader.setInt(std::string("u_spheres[").append(std::to_string(1)).append("].mat.type").c_str(), 3);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(1)).append("].mat.emissionStrength").c_str(), 1);

    shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].center").c_str(), -3, 0, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(2)).append("].radius").c_str(), 1);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].mat.color").c_str(), 0.2f, 0.2f, 0.8f);
    shader.setInt(std::string("u_spheres[").append(std::to_string(2)).append("].mat.type").c_str(), 2);
    shader.setInt(std::string("u_spheres[").append(std::to_string(2)).append("].mat.refIndex").c_str(), 2);

    shader.setVec3(std::string("u_spheres[").append(std::to_string(3)).append("].center").c_str(), 0, -21, 5);
    shader.setFloat(std::string("u_spheres[").append(std::to_string(3)).append("].radius").c_str(), 20);
    shader.setVec3(std::string("u_spheres[").append(std::to_string(3)).append("].mat.color").c_str(), 0.4f, 0.1f, 0.5f);
    shader.setInt(std::string("u_spheres[").append(std::to_string(3)).append("].mat.type").c_str(), 0);    
}

void updateUniforms(Shader shader) {
    //shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].center").c_str(), 0, std::sin(glfwGetTime()), 5);
    //shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].mat.emissionStrength").c_str(), 6.0f * std::abs(0.3f + std::sin(glfwGetTime())));
}
