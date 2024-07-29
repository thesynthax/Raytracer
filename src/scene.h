#include <glad/glad.h>
#include "shader.h"
#include <string>
#include <cmath>
#include "random.h"
#include <GLFW/glfw3.h>

void setPoint();
void setSoftShadows();
void setUseMouseForCamera();
void initializeUniforms(Shader shader);
void updateUniforms(Shader shader, GLFWwindow* window, int scrWidth, int scrHeight);
