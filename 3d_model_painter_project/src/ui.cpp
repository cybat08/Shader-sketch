#include "ui.h"
#include <iostream>
#include <algorithm>

UI::UI(GLFWwindow* window) 
    : selectedTool(nullptr),
      loadModelFlag(false),
      saveProjectFlag(false),
      exportModelFlag(false) {
    
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup ImGui style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

UI::~UI() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(context);
}

void UI::update(float deltaTime, Project& project, 
               const std::vector<std::unique_ptr<PaintTool>>& tools,
               PaintTool* currentTool) {
    
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Save current tool
    selectedTool = currentTool;
    
    // Show UI elements
    showMainMenuBar(project);
    showToolbar(tools);
    showLayersPanel(project);
    showPropertiesPanel(currentTool);
    
    // File dialogs
    if (loadModelFlag) {
        showOpenModelDialog();
    }
    
    if (saveProjectFlag) {
        showSaveProjectDialog();
    }
    
    if (exportModelFlag) {
        showExportModelDialog();
    }
    
    // Demo window (for development)
    // ImGui::ShowDemoWindow();
}

void UI::render() {
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::wantCaptureMouse() const {
    return ImGui::GetIO().WantCaptureMouse;
}

bool UI::wantCaptureKeyboard() const {
    return ImGui::GetIO().WantCaptureKeyboard;
}

void UI::showMainMenuBar(Project& project) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project")) {
                project.clear();
                project.addLayer(); // Add a default layer
            }
            
            if (ImGui::MenuItem("Open Model", "Ctrl+O")) {
                loadModelFlag = true;
            }
            
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {
                saveProjectFlag = true;
            }
            
            if (ImGui::MenuItem("Export Model", "Ctrl+E")) {
                exportModelFlag = true;
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {
                // TODO: Implement undo
            }
            
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {
                // TODO: Implement redo
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Clear All Layers")) {
                for (size_t i = 0; i < project.getLayers().size(); i++) {
                    project.getLayer(i)->clear();
                }
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Layer")) {
            if (ImGui::MenuItem("Add Layer", "Ctrl+N")) {
                project.addLayer();
            }
            
            if (ImGui::MenuItem("Remove Current Layer", nullptr, false, project.getCurrentLayerIndex() >= 0)) {
                project.removeLayer(project.getCurrentLayerIndex());
            }
            
            if (ImGui::MenuItem("Clear Current Layer", nullptr, false, project.getCurrentLayerIndex() >= 0)) {
                project.getCurrentLayer()->clear();
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Reset Camera")) {
                // TODO: Reset camera
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                // TODO: Show about dialog
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void UI::showToolbar(const std::vector<std::unique_ptr<PaintTool>>& tools) {
    // Create toolbar window
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(50, ImGui::GetIO().DisplaySize.y - 20));
    ImGui::Begin("##Toolbar", nullptr, 
                ImGuiWindowFlags_NoTitleBar | 
                ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar);
    
    // Tool buttons
    for (const auto& tool : tools) {
        bool selected = (selectedTool == tool.get());
        
        if (ImGui::RadioButton(tool->getName().c_str(), selected)) {
            selectedTool = tool.get();
        }
        
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%s", tool->getName().c_str());
        }
    }
    
    ImGui::End();
}

void UI::showLayersPanel(Project& project) {
    // Create layers panel window
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 250, ImGui::GetIO().DisplaySize.y / 2));
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y / 2 - 20));
    ImGui::Begin("Layers", nullptr);
    
    // Add layer button
    if (ImGui::Button("Add Layer")) {
        project.addLayer();
    }
    
    ImGui::SameLine();
    
    // Remove layer button
    if (ImGui::Button("Remove Layer") && project.getCurrentLayerIndex() >= 0) {
        project.removeLayer(project.getCurrentLayerIndex());
    }
    
    ImGui::Separator();
    
    // Layers list
    const auto& layers = project.getLayers();
    for (int i = static_cast<int>(layers.size()) - 1; i >= 0; i--) {
        ImGui::PushID(i);
        
        // Layer visibility toggle
        bool visible = layers[i]->isVisible();
        if (ImGui::Checkbox("##visible", &visible)) {
            layers[i]->setVisible(visible);
        }
        
        ImGui::SameLine();
        
        // Layer selection
        bool selected = (project.getCurrentLayerIndex() == i);
        if (ImGui::Selectable(layers[i]->getName().c_str(), selected)) {
            project.setCurrentLayerIndex(i);
        }
        
        // Edit layer name on double-click
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup("Edit Layer Name");
        }
        
        // Layer name edit popup
        if (ImGui::BeginPopup("Edit Layer Name")) {
            char buffer[256];
            strcpy(buffer, layers[i]->getName().c_str());
            
            if (ImGui::InputText("##name", buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
                layers[i]->setName(buffer);
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        // Layer opacity
        float opacity = layers[i]->getOpacity();
        if (ImGui::SliderFloat("##opacity", &opacity, 0.0f, 1.0f, "%.2f")) {
            layers[i]->setOpacity(opacity);
        }
        
        ImGui::PopID();
    }
    
    ImGui::End();
}

void UI::showPropertiesPanel(PaintTool* currentTool) {
    // Create properties panel window
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 250, 20));
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y / 2 - 20));
    ImGui::Begin("Properties", nullptr);
    
    if (currentTool) {
        ImGui::Text("Tool: %s", currentTool->getName().c_str());
        ImGui::Separator();
        
        // Tool size
        float size = currentTool->getSize();
        if (ImGui::SliderFloat("Size", &size, 1.0f, 50.0f, "%.1f")) {
            currentTool->setSize(size);
        }
        
        // Tool hardness
        float hardness = currentTool->getHardness();
        if (ImGui::SliderFloat("Hardness", &hardness, 0.0f, 1.0f, "%.2f")) {
            currentTool->setHardness(hardness);
        }
        
        // Tool color
        ImVec4 color = glmToImVec4(currentTool->getColor());
        if (ImGui::ColorEdit4("Color", (float*)&color)) {
            currentTool->setColor(imVec4ToGlm(color));
        }
        
        // Tool-specific properties
        if (currentTool->getName() == "Fill") {
            FillTool* fillTool = static_cast<FillTool*>(currentTool);
            float tolerance = fillTool->getTolerance();
            if (ImGui::SliderFloat("Tolerance", &tolerance, 0.01f, 1.0f, "%.2f")) {
                fillTool->setTolerance(tolerance);
            }
        }
    }
    
    ImGui::End();
}

void UI::showOpenModelDialog() {
    ImGui::OpenPopup("Open Model");
    
    if (ImGui::BeginPopupModal("Open Model", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char path[256] = "";
        ImGui::InputText("Path", path, 256);
        
        ImGui::TextUnformatted("Supported formats: .obj, .fbx");
        
        ImGui::Separator();
        
        if (ImGui::Button("Open", ImVec2(120, 0))) {
            modelPath = path;
            loadModelFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            loadModelFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}

void UI::showSaveProjectDialog() {
    ImGui::OpenPopup("Save Project");
    
    if (ImGui::BeginPopupModal("Save Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char path[256] = "";
        ImGui::InputText("Path", path, 256);
        
        ImGui::TextUnformatted("Save as .3dp file");
        
        ImGui::Separator();
        
        if (ImGui::Button("Save", ImVec2(120, 0))) {
            projectPath = path;
            saveProjectFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            saveProjectFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}

void UI::showExportModelDialog() {
    ImGui::OpenPopup("Export Model");
    
    if (ImGui::BeginPopupModal("Export Model", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char path[256] = "";
        ImGui::InputText("Path", path, 256);
        
        ImGui::TextUnformatted("Supported formats: .obj, .fbx");
        
        ImGui::Separator();
        
        if (ImGui::Button("Export", ImVec2(120, 0))) {
            exportPath = path;
            exportModelFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            exportModelFlag = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}

ImVec4 UI::glmToImVec4(const glm::vec4& color) {
    return ImVec4(color.r, color.g, color.b, color.a);
}

glm::vec4 UI::imVec4ToGlm(const ImVec4& color) {
    return glm::vec4(color.x, color.y, color.z, color.w);
}
