#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>

bool refresh;

void initGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    mainMenuUI();
    cameraSettingsUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void deactivateGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void mainMenuUI() {
    ImGui::Begin("Raytracer");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

    ImGui::Text("Main Settings");
    ImGui::PushItemWidth(-1);

    ImGui::Text("Samples per pixel");
    ImGui::SameLine();
    if (ImGui::InputInt("##raysPerPixel", &Scene::raysPerPixel)) {
        refresh = true;
    }

    ImGui::Text("Light Bounce Limit");
    ImGui::SameLine();
    if (ImGui::InputInt("##maxBounceLimit", &Scene::maxBounceLimit)) {
        refresh = true;
    }

    ImGui::Text("Shadow Resolution");
    ImGui::SameLine();
    if (ImGui::InputInt("##shadowRays", &Scene::shadowRays)) {
        refresh = true;
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void cameraSettingsUI() {
    ImGui::Begin("Camera");
    ImGui::PushItemWidth(-1);

    ImGui::Text("Use Mouse for Camera");
    ImGui::SameLine();
    if (ImGui::Checkbox("##mouseCam", &Scene::useMouseForCamera)) {
        refresh = true;
    }

    if (!Scene::useMouseForCamera) {
        ImGui::Text("Field of View");
        ImGui::SameLine();
        if (ImGui::InputFloat("##fov", &Scene::fov)) {
            refresh = true;
        }
        
        ImGui::Text("Position");
        ImGui::SameLine();
        if (ImGui::SliderFloat3("##cameraPosition", &Scene::camPos.x, -10, 10)) {
            refresh = true;
        }

        ImGui::Text("LookAtDirection");
        ImGui::SameLine();
        if (ImGui::SliderFloat3("##CameraLookAt", &Scene::camLookAt.x, -10, 10)) {
            refresh = true;
        }
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void selectedObjectSettingsUI() {

}

void sceneSettingsUI() {

}

void shaderFloatParameter(Shader shader, const char* name, const char* displayName, float* floatPtr) {
    ImGui::Text(displayName);
    ImGui::SameLine();
    if (ImGui::InputFloat(std::string("##").append(name).c_str(), floatPtr)) {
        shader.setFloat(name, *floatPtr);
        refresh = true;
    }
}
void shaderSliderParameter(Shader shader, const char* name, const char* displayName, float* floatPtr) {
    ImGui::Text(displayName);
    ImGui::SameLine();
    if (ImGui::SliderFloat(std::string("##").append(name).c_str(), floatPtr, 0.0f, 1.0f)) {
        shader.setFloat(name, *floatPtr);
        refresh = true;
    }
}
void shaderVecParameter(Shader shader, const char* name, const char* displayName, float* floatPtr) {
    ImGui::Text(displayName);
    ImGui::SameLine();
    if (ImGui::InputFloat3(std::string("##").append(name).c_str(), floatPtr)) {
        shader.setVec3(name, *(floatPtr), *(floatPtr + 1), *(floatPtr + 2));
        refresh = true;
    }
}
void shaderColorParameter(Shader shader, const char* name, const char* displayName, float* floatPtr) {
    ImGui::Text(displayName);
    ImGui::SameLine();
    if (ImGui::ColorPicker3(name, floatPtr)) {
        shader.setVec3(name, *(floatPtr), *(floatPtr + 1), *(floatPtr + 2));
        refresh = true;
    }
}


