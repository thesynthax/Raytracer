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
    selectedObjectSettingsUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void deactivateGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

/*
std::string arrayElementName(const char* arrayName, int index, const char* keyName) {
    return std::string(arrayName).append("[").append(std::to_string(index)).append("].").append(keyName);
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
*/

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
        ImGui::Text("Camera to Screen Distance");
        ImGui::SameLine();
        if (ImGui::SliderFloat("##rayOriginToScreen", &Scene::rayOriginToScreenDistance, 0, 2)) {
            refresh = true;
        }
        
        ImGui::Text("Field of View");
        ImGui::SameLine();
        if (ImGui::SliderFloat("##fov", &Scene::fov, 0, 180)) {
            refresh = true;
        }
        
        ImGui::Text("Position");
        ImGui::SameLine();
        if (ImGui::SliderFloat3("##cameraPosition", &Scene::camPosFromSettings.x, -10, 10)) {
            refresh = true;
        }

        ImGui::Text("LookAtDirection");
        ImGui::SameLine();
        if (ImGui::SliderFloat3("##CameraLookAt", &Scene::camLookAtFromSettings.x, -10, 10)) {
            refresh = true;
        }
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void selectedObjectSettingsUI() {
    ImGui::Begin("Selected Object Seetings");
    ImGui::PushItemWidth(-1);

    if (Scene::selectedObjectIndex != -1) {
        int i = Scene::selectedObjectIndex;
        std::string i_str = std::to_string(Scene::selectedObjectIndex);
        
        ImGui::Text(std::string("Sphere #").append(i_str).c_str());

        ImGui::Text(std::string("Center").c_str());
        ImGui::SameLine();
        if (ImGui::InputFloat3("##Center", &Scene::spheres[i].center.x)) {
            refresh = true;
        }
        ImGui::Text(std::string("Radius").c_str());
        ImGui::SameLine();
        if (ImGui::InputFloat("##Radius", &Scene::spheres[i].radius)) {
            refresh = true;
        }
        ImGui::Text(std::string("Material Type").c_str());
        ImGui::SameLine();
        if (ImGui::InputInt("##MatType", &Scene::spheres[i].mat.type)) {
            refresh = true;
        }
        switch(Scene::spheres[i].mat.type) {
            case (0): {
                ImGui::Text(std::string("Color").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat3("##color", &Scene::spheres[i].mat.color.x, 0, 1)) {
                    refresh = true;
                }
                break;
            }
            case (1): {
                ImGui::Text(std::string("Color").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat3("##color", &Scene::spheres[i].mat.color.x, 0, 1)) {
                    refresh = true;
                }
                ImGui::Text(std::string("Fuzz").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat("##fuzz", &Scene::spheres[i].mat.fuzz, 0, 1)) {
                    refresh = true;
                }
                break;
            }
            case (2): {
                ImGui::Text(std::string("Refractive Index").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat("##refIdx", &Scene::spheres[i].mat.refIndex, 1, 5)) {
                    refresh = true;
                }
                break;
            }
            case (3): {
                ImGui::Text(std::string("Color").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat3("##color", &Scene::spheres[i].mat.color.x, 0, 1)) {
                    refresh = true;
                }
                ImGui::Text(std::string("Emission Strength").c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat("##emission", &Scene::spheres[i].mat.emissionStrength, 0, 20)) {
                    refresh = true;
                }
                break;
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::End();
}

void sceneSettingsUI() {

}


