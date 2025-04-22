#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "paint_tool.h"
#include "project.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();
    
    // Update UI
    void update(float deltaTime, Project& project, 
               const std::vector<std::unique_ptr<PaintTool>>& tools,
               PaintTool* currentTool);
    
    // Render UI
    void render();
    
    // Check if UI wants to capture mouse/keyboard input
    bool wantCaptureMouse() const;
    bool wantCaptureKeyboard() const;
    
    // Get selected tool
    PaintTool* getSelectedTool() const { return selectedTool; }
    
    // File dialog flags
    bool shouldLoadModel() const { return loadModelFlag; }
    bool shouldSaveProject() const { return saveProjectFlag; }
    bool shouldExportModel() const { return exportModelFlag; }
    
    // Clear flags
    void clearModelLoadFlag() { loadModelFlag = false; }
    void clearSaveProjectFlag() { saveProjectFlag = false; }
    void clearExportModelFlag() { exportModelFlag = false; }
    
    // Set flags
    void setModelLoadFlag() { loadModelFlag = true; }
    void setSaveProjectFlag() { saveProjectFlag = true; }
    void setExportModelFlag() { exportModelFlag = true; }
    
    // Get file paths
    const std::string& getModelPath() const { return modelPath; }
    const std::string& getProjectPath() const { return projectPath; }
    const std::string& getExportPath() const { return exportPath; }
    
private:
    // ImGui context
    ImGuiContext* context;
    
    // Selected tool
    PaintTool* selectedTool;
    
    // File dialog flags
    bool loadModelFlag;
    bool saveProjectFlag;
    bool exportModelFlag;
    
    // File paths
    std::string modelPath;
    std::string projectPath;
    std::string exportPath;
    
    // UI sections
    void showMainMenuBar(Project& project);
    void showToolbar(const std::vector<std::unique_ptr<PaintTool>>& tools);
    void showLayersPanel(Project& project);
    void showPropertiesPanel(PaintTool* currentTool);
    
    // File dialogs
    void showOpenModelDialog();
    void showSaveProjectDialog();
    void showExportModelDialog();
    
    // Helper functions
    ImVec4 glmToImVec4(const glm::vec4& color);
    glm::vec4 imVec4ToGlm(const ImVec4& color);
};
