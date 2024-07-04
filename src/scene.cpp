#include "scene.h"

void initializeUniforms(Shader shader) {
    shader.setFloat("u_fov", 90.0f);
    shader.setVec3("u_initCamPos", 0, 0, -3);
    shader.setVec3("u_initCamDir", 0, 0, 0);
    shader.setVec3("u_upDir", 0, 1, 0);
}
