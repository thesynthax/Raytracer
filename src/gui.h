#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ImGuiIO& initGUI(GLFWwindow* window);
void initFrame();
void drawFrame(ImGuiIO& io);
void deactivateGUI();
