#include "glad/glad.h"
#include "shader.h"
#include <string>
#include <cmath>
#include "random.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Scene {

    extern float timeElapsed;
    extern glm::vec2 mousePos;

    extern int shadowRays;
    extern int raysPerPixel;
    extern int maxBounceLimit;

    extern glm::vec2 screenPixels;
    extern float aspectRatio;

    extern float fov;
    extern glm::vec3 camPos;
    extern glm::vec3 camLookAt;
    extern glm::vec3 upDir;
    extern float rayOriginToScreenDistance;
    extern bool useMouseForCamera;

    struct Material {
        int type; //0 = lambertian, 1 = metal, 2 = dielectric, 3 = emissive
        glm::vec3 color; //for type = 0,1,3
        float fuzz; //for type = 1
        float refIndex; //for type = 2
        float emissionStrength; //for type = 3
        Material(int type, glm::vec3 color, float fuzz, float refIndex, float emissionStrength);
        Material();
    };
    struct Sphere {
        glm::vec3 center;
        float radius;
        Material mat;
        Sphere(glm::vec3 center, float radius, Material mat);
        Sphere();
    };
    struct Triangle {
        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 pointC;
        Material mat;
        Triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, Material mat);
        Triangle();
    };
    struct Light {
        int type; //0 = point, 1 = directional
        glm::vec3 pos; //for type = 0
        glm::vec3 dir; //for type = 1
        glm::vec3 color;
        float maxIntensity; //inverse square law for type = 0, constant for type = 1
        bool softShadows;
        Light(int type, glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float maxIntensity, bool softShadows);
        Light();
    };

    void initialize(Shader shader, int screenWidth, int screenHeight);
    void initializeUniforms(Shader shader);
    void update(Shader shader, GLFWwindow* window, int screenWidth, int screenHeight);
    void updateUniforms(Shader shader, GLFWwindow* window, int scrWidth, int scrHeight);
    void sphereSelect();
}
