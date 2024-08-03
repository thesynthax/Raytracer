#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "scene.h"

namespace GUI {
void initGUI(GLFWwindow* window);
void renderGUI();
void deactivateGUI();
void mainMenuUI();
void cameraSettingsUI();
void selectedObjectSettingsUI();
std::string arrayElementName(const char* arrayName, int index, const char* keyName);
void shaderFloatParameter(Shader shader, const char* name, const char* displayName, float* floatPtr);
void shaderSliderParameter(Shader shader, const char* name, const char* displayName, float* floatPtr);
void shaderVecParameter(Shader shader, const char* name, const char* displayName, float* floatPtr);
void shaderColorParameter(Shader shader, const char* name, const char* displayName, float* floatPtr);
}
