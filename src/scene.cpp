#include "scene.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_float2x2.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

namespace Scene {

    float timeElapsed;
    glm::vec2 mousePos;

    int shadowRays;
    int raysPerPixel;
    int maxBounceLimit;

    glm::vec2 screenPixels;
    float aspectRatio;

    float fov;
    glm::vec3 camPos;
    glm::vec3 camLookAt;
    glm::vec3 upDir;
    bool useMouseForCamera;

    Material::Material() = default;
    Material::Material(int type, glm::vec3 color, float fuzz, float refIndex, float emissionStrength) {
        this->type = type;
        this->color = color;
        this->fuzz = fuzz;
        this->refIndex = refIndex;
        this->emissionStrength = emissionStrength;
    }

    Sphere::Sphere() = default;
    Sphere::Sphere(glm::vec3 center, float radius, Material mat) {
        this->center = center;
        this->radius = radius;
        this->mat = mat;
    }

    Triangle::Triangle() = default;
    Triangle::Triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, Material mat) {
        this->pointA = pointA;
        this->pointB = pointB;
        this->pointC = pointC;
        this->mat = mat;
    }

    Light::Light() = default;
    Light::Light(int type, glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float maxIntensity, bool softShadows) {
        this->type = type;
        this->pos = pos;
        this->dir = dir;
        this->color = color;
        this->maxIntensity = maxIntensity;
        this->softShadows = softShadows;
    }

    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    glm::mat2 rot2d(float angle) {
        float s = std::sin(angle);
        float c = std::cos(angle);

        return glm::mat2(c, -s, s, c);
    }

    void initialize(Shader shader, int screenWidth, int screenHeight) {
        screenPixels = glm::vec2(screenWidth, screenHeight);
        aspectRatio = float(screenWidth)/float(screenHeight);
        fov = 70.0f;
        camPos = glm::vec3(0, 0, -10);
        camLookAt = glm::vec3(0, 0, 0);
        upDir = glm::vec3(0, 1, 0);
        raysPerPixel = 10;
        maxBounceLimit = 20;
        shadowRays = 10;
        useMouseForCamera = false;

        initializeUniforms(shader);
    }

    void initializeUniforms(Shader shader) {
        shader.setFloat("INFINITY", INFINITY);
        shader.setVec2("u_screenPixels", screenPixels);
        shader.setFloat("u_aspectRatio", aspectRatio);
        shader.setFloat("u_fov", fov);
        shader.setVec3("u_camPos", camPos);
        shader.setVec3("u_camLookAt", camLookAt);
        shader.setVec3("u_upDir", upDir);
        shader.setInt("u_raysPerPixel", raysPerPixel);
        shader.setInt("u_maxBounceLimit", maxBounceLimit);
        shader.setInt("u_shadowRays", shadowRays);

        /*for (int i = 0; i < 9; i++) {
            shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].center").c_str(), rng(-2, 2), rng(-1, 1), rng(2, 6));
            shader.setFloat(std::string("u_spheres[").append(std::to_string(i)).append("].radius").c_str(), rng(0.25f, 1));
            shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.color").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
            shader.setVec3(std::string("u_spheres[").append(std::to_string(i)).append("].mat.emission").c_str(), rng(0, 1), rng(0, 1), rng(0, 1));
        }*/
        
        shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].center").c_str(), 0, 0, 0);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].radius").c_str(), 1);
        shader.setVec3(std::string("u_spheres[").append(std::to_string(0)).append("].mat.color").c_str(), 1.0f, 1.0f, 1.0f);
        shader.setInt(std::string("u_spheres[").append(std::to_string(0)).append("].mat.type").c_str(), 1);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].mat.fuzz").c_str(), 0.1f);

        shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].center").c_str(), 3, 0, 5);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(1)).append("].radius").c_str(), 1);
        shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].mat.color").c_str(), 0.2f, 0.8f, 0.2f);
        shader.setInt(std::string("u_spheres[").append(std::to_string(1)).append("].mat.type").c_str(), 3);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(1)).append("].mat.emissionStrength").c_str(), 6);

        shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].center").c_str(), -3, 0, 5);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(2)).append("].radius").c_str(), 1);
        shader.setVec3(std::string("u_spheres[").append(std::to_string(2)).append("].mat.color").c_str(), 0.8f, 0.2f, 0.2f);
        shader.setInt(std::string("u_spheres[").append(std::to_string(2)).append("].mat.type").c_str(), 0);

        shader.setVec3(std::string("u_spheres[").append(std::to_string(3)).append("].center").c_str(), 0, -21, 5);
        shader.setFloat(std::string("u_spheres[").append(std::to_string(3)).append("].radius").c_str(), 20);
        shader.setVec3(std::string("u_spheres[").append(std::to_string(3)).append("].mat.color").c_str(), 0.4f, 0.1f, 0.5f);
        shader.setInt(std::string("u_spheres[").append(std::to_string(3)).append("].mat.type").c_str(), 0);

        shader.setVec3(std::string("u_triangles[").append(std::to_string(0)).append("].pointA").c_str(), 0, -1, 5);
        shader.setVec3(std::string("u_triangles[").append(std::to_string(0)).append("].pointB").c_str(), 3, 4, 5);
        shader.setVec3(std::string("u_triangles[").append(std::to_string(0)).append("].pointC").c_str(), 0, 2, 5);
        shader.setInt(std::string("u_triangles[").append(std::to_string(0)).append("].mat.type").c_str(), 3);
        shader.setInt(std::string("u_triangles[").append(std::to_string(0)).append("].mat.emissionStrength").c_str(), 9);
        shader.setVec3(std::string("u_triangles[").append(std::to_string(0)).append("].mat.color").c_str(), 1, 1, 1);
    }

    double mouseX, mouseY;
    double deltaTime = 0.0f;
    void update(Shader shader, GLFWwindow* window, int screenWidth, int screenHeight) {
        timeElapsed = glfwGetTime();
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mousePos = glm::vec2(float(mouseX/screenWidth), float(mouseY/screenHeight));
        updateUniforms(shader, window, screenWidth, screenHeight);
        //glfwSetCursorPos(window, double(screenWidth) / 2, double(screenHeight) / 2);
    }

    void updateUniforms(Shader shader, GLFWwindow* window, int scrWidth, int scrHeight) {
        shader.setFloat("u_time", timeElapsed);
        shader.setVec2("u_mousePos", mousePos);
        shader.setFloat("u_fov", fov);
        shader.setVec3("u_camPos", camPos);
        shader.setVec3("u_camLookAt", camLookAt);
        shader.setInt("u_raysPerPixel", raysPerPixel);
        shader.setInt("u_maxBounceLimit", maxBounceLimit);
        shader.setInt("u_shadowRays", shadowRays);


        //shader.setVec3(std::string("u_lights[").append(std::to_string(0)).append("].pos").c_str(), 0, 3, 5);
        shader.setVec3(std::string("u_lights[").append(std::to_string(0)).append("].color").c_str(), 1, 1, 1);
        shader.setInt(std::string("u_lights[").append(std::to_string(0)).append("].type").c_str(), 0);
        shader.setFloat(std::string("u_lights[").append(std::to_string(0)).append("].maxIntensity").c_str(), 9);
        //shader.setVec3(std::string("u_lights[").append(std::to_string(0)).append("].dir").c_str(), 0, 3, 5);
        shader.setVec3(std::string("u_lights[").append(std::to_string(1)).append("].color").c_str(), 1, 1, 1);
        shader.setInt(std::string("u_lights[").append(std::to_string(1)).append("].type").c_str(), 1);
        shader.setFloat(std::string("u_lights[").append(std::to_string(1)).append("].maxIntensity").c_str(), 1);
        shader.setVec3(std::string("u_lights[").append(std::to_string(0)).append("].pos").c_str(), -(mouseX - float(scrWidth)/2)/100, -(mouseY - float(scrHeight)/2)/100, 5);
        shader.setVec3(std::string("u_lights[").append(std::to_string(1)).append("].dir").c_str(), -(mouseX - float(scrWidth)/2)/20, -(mouseY - float(scrHeight)/2)/20, -5);
        shader.setVec3(std::string("u_spheres[").append(std::to_string(1)).append("].center").c_str(), 3, std::sin(glfwGetTime()), 5);
        //shader.setFloat(std::string("u_spheres[").append(std::to_string(0)).append("].mat.emissionStrength").c_str(), 6.0f * std::abs(0.3f + std::sin(glfwGetTime())));
    }

}
