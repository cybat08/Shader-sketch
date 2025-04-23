#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <algorithm>

// ImGui and OpenGL headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// Include our color wheel implementation
#include "color_wheel.h"

// Make sure we define M_PI for Windows platforms
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Forward declarations
static void glfw_error_callback(int error, const char* description);
void RenderColorWheel(ImDrawList* draw_list, ImVec2 center, float radius, float hue, float saturation, ImU32 colorMarker);
ImU32 HSVtoRGB(float h, float s, float v);
ImVec4 HSVtoImVec4(float h, float s, float v);

// Global state for the application
struct AppState {
    // Color selection state
    float hue = 0.0f;
    float saturation = 1.0f;
    float value = 1.0f;
    
    // Canvas state
    std::vector<ImVec4> canvasPixels;
    int canvasWidth = 30;
    int canvasHeight = 15;
    int selectedTool = 0;
    float brushSize = 1.0f;
    
    // Current selected color in ImGui format
    ImVec4 currentColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // Layer management
    struct Layer {
        std::string name;
        std::vector<ImVec4> pixels;
        bool visible = true;
    };
    std::vector<Layer> layers;
    int currentLayer = 0;
    
    // Color harmony
    int currentHarmony = 0;
    const char* harmonyTypes[7] = {
        "Complementary", "Analogous", "Triadic", 
        "Split Complementary", "Tetradic", "Square", "Monochromatic"
    };
    std::vector<ImVec4> harmonyColors;
    
    // Color history
    std::vector<ImVec4> colorHistory;
    int maxHistorySize = 10;
    
    AppState() {
        // Initialize canvas with transparent pixels
        canvasPixels.resize(canvasWidth * canvasHeight, ImVec4(0, 0, 0, 0));
        
        // Add a default background layer
        Layer backgroundLayer;
        backgroundLayer.name = "Background";
        backgroundLayer.pixels.resize(canvasWidth * canvasHeight, ImVec4(0, 0, 0, 0));
        layers.push_back(backgroundLayer);
        
        // Initialize harmony colors
        updateHarmonyColors();
    }
    
    void addColorToHistory(ImVec4 color) {
        // Check for duplicates
        for (auto it = colorHistory.begin(); it != colorHistory.end(); ++it) {
            if (it->x == color.x && it->y == color.y && it->z == color.z) {
                colorHistory.erase(it);
                break;
            }
        }
        
        // Add to the beginning
        colorHistory.insert(colorHistory.begin(), color);
        
        // Keep history within size limit
        if (colorHistory.size() > maxHistorySize) {
            colorHistory.resize(maxHistorySize);
        }
    }
    
    void updateHarmonyColors() {
        harmonyColors.clear();
        harmonyColors.push_back(currentColor); // Base color
        
        // Generate harmony colors based on harmony type
        switch (currentHarmony) {
            case 0: // Complementary
                {
                    float compHue = fmodf(hue + 180.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(compHue, saturation, value));
                }
                break;
            case 1: // Analogous
                {
                    float h1 = fmodf(hue - 30.0f + 360.0f, 360.0f);
                    float h2 = fmodf(hue + 30.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(h1, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h2, saturation, value));
                }
                break;
            case 2: // Triadic
                {
                    float h1 = fmodf(hue + 120.0f, 360.0f);
                    float h2 = fmodf(hue + 240.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(h1, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h2, saturation, value));
                }
                break;
            case 3: // Split Complementary
                {
                    float h1 = fmodf(hue + 150.0f, 360.0f);
                    float h2 = fmodf(hue + 210.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(h1, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h2, saturation, value));
                }
                break;
            case 4: // Tetradic
                {
                    float h1 = fmodf(hue + 90.0f, 360.0f);
                    float h2 = fmodf(hue + 180.0f, 360.0f);
                    float h3 = fmodf(hue + 270.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(h1, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h2, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h3, saturation, value));
                }
                break;
            case 5: // Square
                {
                    float h1 = fmodf(hue + 90.0f, 360.0f);
                    float h2 = fmodf(hue + 180.0f, 360.0f);
                    float h3 = fmodf(hue + 270.0f, 360.0f);
                    harmonyColors.push_back(HSVtoImVec4(h1, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h2, saturation, value));
                    harmonyColors.push_back(HSVtoImVec4(h3, saturation, value));
                }
                break;
            case 6: // Monochromatic
                {
                    harmonyColors.push_back(HSVtoImVec4(hue, saturation * 0.7f, value));
                    harmonyColors.push_back(HSVtoImVec4(hue, saturation, value * 0.7f));
                    harmonyColors.push_back(HSVtoImVec4(hue, saturation * 0.7f, value * 0.7f));
                }
                break;
        }
    }
    
    void paint(int x, int y) {
        if (x < 0 || y < 0 || x >= canvasWidth || y >= canvasHeight)
            return;
        
        // Get the current layer to paint on
        Layer& layer = layers[currentLayer];
        
        // Apply the brush
        for (int dy = -static_cast<int>(brushSize); dy <= static_cast<int>(brushSize); dy++) {
            for (int dx = -static_cast<int>(brushSize); dx <= static_cast<int>(brushSize); dx++) {
                int px = x + dx;
                int py = y + dy;
                
                // Check if point is within canvas and within brush radius
                if (px >= 0 && py >= 0 && px < canvasWidth && py < canvasHeight) {
                    float distance = std::sqrt(dx*dx + dy*dy);
                    if (distance <= brushSize) {
                        // Paint the pixel
                        layer.pixels[py * canvasWidth + px] = currentColor;
                    }
                }
            }
        }
        
        // Composite all visible layers to the canvas
        renderCanvas();
    }
    
    void addLayer(const std::string& name) {
        Layer newLayer;
        newLayer.name = name;
        newLayer.pixels.resize(canvasWidth * canvasHeight, ImVec4(0, 0, 0, 0));
        layers.push_back(newLayer);
        currentLayer = layers.size() - 1;
    }
    
    void renderCanvas() {
        // Reset canvas
        std::fill(canvasPixels.begin(), canvasPixels.end(), ImVec4(0, 0, 0, 0));
        
        // Composite all visible layers from bottom to top
        for (const Layer& layer : layers) {
            if (layer.visible) {
                for (int i = 0; i < canvasWidth * canvasHeight; i++) {
                    const ImVec4& layerPixel = layer.pixels[i];
                    if (layerPixel.w > 0) { // If pixel is not transparent
                        canvasPixels[i] = layerPixel;
                    }
                }
            }
        }
    }
    
    void clearCurrentLayer() {
        Layer& layer = layers[currentLayer];
        std::fill(layer.pixels.begin(), layer.pixels.end(), ImVec4(0, 0, 0, 0));
        renderCanvas();
    }
};

// Main application state
AppState appState;

// Convert HSV to RGB and return ImGui color
ImU32 HSVtoRGB(float h, float s, float v) {
    // H: 0-360, S: 0-1, V: 0-1
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    
    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c, g = x, b = 0;
    } else if (h >= 60 && h < 120) {
        r = x, g = c, b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0, g = c, b = x;
    } else if (h >= 180 && h < 240) {
        r = 0, g = x, b = c;
    } else if (h >= 240 && h < 300) {
        r = x, g = 0, b = c;
    } else {
        r = c, g = 0, b = x;
    }
    
    int ri = static_cast<int>((r + m) * 255);
    int gi = static_cast<int>((g + m) * 255);
    int bi = static_cast<int>((b + m) * 255);
    
    return IM_COL32(ri, gi, bi, 255);
}

// Convert HSV to ImVec4
ImVec4 HSVtoImVec4(float h, float s, float v) {
    // H: 0-360, S: 0-1, V: 0-1
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    
    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c, g = x, b = 0;
    } else if (h >= 60 && h < 120) {
        r = x, g = c, b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0, g = c, b = x;
    } else if (h >= 180 && h < 240) {
        r = 0, g = x, b = c;
    } else if (h >= 240 && h < 300) {
        r = x, g = 0, b = c;
    } else {
        r = c, g = 0, b = x;
    }
    
    return ImVec4(r + m, g + m, b + m, 1.0f);
}

// Render a color wheel in ImGui
void RenderColorWheel(ImDrawList* draw_list, ImVec2 center, float radius, float hue, float saturation, ImU32 colorMarker) {
    int segments = 32;
    draw_list->AddCircle(center, radius + 1, IM_COL32(40, 40, 40, 255), segments, 2.0f);
    
    // Draw color wheel segments
    for (int i = 0; i < segments; i++) {
        float a1 = i * 2.0f * M_PI / segments;
        float a2 = (i + 1) * 2.0f * M_PI / segments;
        float h1 = i * 360.0f / segments;
        float h2 = (i + 1) * 360.0f / segments;
        
        ImVec2 p1(center.x + std::cos(a1) * radius, center.y + std::sin(a1) * radius);
        ImVec2 p2(center.x + std::cos(a2) * radius, center.y + std::sin(a2) * radius);
        
        draw_list->AddTriangleFilled(center, p1, p2, HSVtoRGB(h1, 1.0f, 1.0f));
    }
    
    // Draw saturation as a gradient from center (white) to edge (full color)
    for (float r = 0; r < radius; r += radius / 12) {
        float sat = r / radius;
        draw_list->AddCircle(center, r, HSVtoRGB(hue, sat, 1.0f), segments);
    }
    
    // Draw current color marker
    float a = hue * 2.0f * M_PI / 360.0f;
    ImVec2 pos(center.x + std::cos(a) * radius * saturation, 
               center.y + std::sin(a) * radius * saturation);
    draw_list->AddCircleFilled(pos, 5.0f, colorMarker);
    draw_list->AddCircle(pos, 5.0f, IM_COL32(255, 255, 255, 255), 12, 1.0f);
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "3D Model Painter GUI", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontDefault();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a dockspace to organize the UI
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // Begin main window
        {
            ImGui::Begin("3D Model Painter");
            
            // Tool selection
            const char* tools[] = { "Brush", "Eraser", "Line Art", "Fill", "Color Picker" };
            ImGui::Text("Tools:");
            for (int i = 0; i < IM_ARRAYSIZE(tools); i++) {
                ImGui::SameLine();
                if (ImGui::RadioButton(tools[i], appState.selectedTool == i))
                    appState.selectedTool = i;
            }
            
            // Brush size
            ImGui::SliderFloat("Brush Size", &appState.brushSize, 0.5f, 5.0f, "%.1f");
            
            // Color selection with HSV sliders
            ImGui::Text("Color Selection:");
            bool colorChanged = false;
            colorChanged |= ImGui::SliderFloat("Hue", &appState.hue, 0.0f, 360.0f);
            colorChanged |= ImGui::SliderFloat("Saturation", &appState.saturation, 0.0f, 1.0f);
            colorChanged |= ImGui::SliderFloat("Value", &appState.value, 0.0f, 1.0f);
            
            if (colorChanged) {
                appState.currentColor = HSVtoImVec4(appState.hue, appState.saturation, appState.value);
                appState.updateHarmonyColors();
                appState.addColorToHistory(appState.currentColor);
            }
            
            // Harmony type selection
            if (ImGui::Combo("Harmony", &appState.currentHarmony, appState.harmonyTypes, IM_ARRAYSIZE(appState.harmonyTypes))) {
                appState.updateHarmonyColors();
            }
            
            // Current layer selection
            ImGui::Text("Current Layer: %s", appState.layers[appState.currentLayer].name.c_str());
            
            // Basic controls
            if (ImGui::Button("Clear Layer")) {
                appState.clearCurrentLayer();
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Add Layer")) {
                ImGui::OpenPopup("NewLayerPopup");
            }
            
            // New layer popup
            if (ImGui::BeginPopup("NewLayerPopup")) {
                static char layerName[64] = "New Layer";
                ImGui::Text("Layer Name:");
                ImGui::InputText("##layername", layerName, IM_ARRAYSIZE(layerName));
                
                if (ImGui::Button("Create")) {
                    appState.addLayer(layerName);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            
            ImGui::End();
        }
        
        // Color wheel window
        {
            ImGui::Begin("Color Wheel");
            
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            float wheel_size = std::min(canvas_size.x, canvas_size.y) * 0.8f;
            ImVec2 wheel_center(canvas_pos.x + wheel_size / 2, canvas_pos.y + wheel_size / 2);
            
            // Draw color wheel
            RenderColorWheel(draw_list, wheel_center, wheel_size / 2, appState.hue, appState.saturation, 
                             IM_COL32(255, 255, 255, 255));
            
            // Make wheel interactive
            ImVec2 mouse = ImGui::GetIO().MousePos;
            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                float dx = mouse.x - wheel_center.x;
                float dy = mouse.y - wheel_center.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance <= wheel_size / 2) {
                    // Set color based on wheel position
                    float angle = std::atan2(dy, dx) * 180.0f / M_PI;
                    if (angle < 0)
                        angle += 360.0f;
                    
                    appState.hue = angle;
                    appState.saturation = std::min(1.0f, distance / (wheel_size / 2));
                    appState.currentColor = HSVtoImVec4(appState.hue, appState.saturation, appState.value);
                    appState.updateHarmonyColors();
                    appState.addColorToHistory(appState.currentColor);
                }
            }
            
            // Display harmony colors
            ImGui::Dummy(ImVec2(0, wheel_size));
            ImGui::Text("Harmony Colors (%s):", appState.harmonyTypes[appState.currentHarmony]);
            
            for (size_t i = 0; i < appState.harmonyColors.size(); i++) {
                const ImVec4& color = appState.harmonyColors[i];
                ImGui::ColorButton(("##harmony" + std::to_string(i)).c_str(), color, ImGuiColorEditFlags_NoTooltip, ImVec2(30, 30));
                
                if (ImGui::IsItemClicked()) {
                    appState.currentColor = color;
                    
                    // Convert back to HSV for sliders
                    float max = std::max(std::max(color.x, color.y), color.z);
                    float min = std::min(std::min(color.x, color.y), color.z);
                    float delta = max - min;
                    
                    appState.value = max;
                    
                    if (max > 0) 
                        appState.saturation = delta / max;
                    else
                        appState.saturation = 0;
                    
                    if (delta > 0) {
                        if (max == color.x)
                            appState.hue = 60.0f * std::fmod((color.y - color.z) / delta, 6.0f);
                        else if (max == color.y)
                            appState.hue = 60.0f * ((color.z - color.x) / delta + 2.0f);
                        else
                            appState.hue = 60.0f * ((color.x - color.y) / delta + 4.0f);
                    } else {
                        appState.hue = 0;
                    }
                    
                    if (appState.hue < 0)
                        appState.hue += 360.0f;
                }
                
                if ((i + 1) % 6 != 0 && i < appState.harmonyColors.size() - 1)
                    ImGui::SameLine();
            }
            
            // Color history
            if (!appState.colorHistory.empty()) {
                ImGui::Separator();
                ImGui::Text("Color History:");
                
                for (size_t i = 0; i < appState.colorHistory.size(); i++) {
                    ImGui::ColorButton(("##history" + std::to_string(i)).c_str(), appState.colorHistory[i], ImGuiColorEditFlags_NoTooltip, ImVec2(30, 30));
                    
                    if (ImGui::IsItemClicked()) {
                        appState.currentColor = appState.colorHistory[i];
                        
                        // Convert back to HSV for sliders
                        float max = std::max(std::max(appState.currentColor.x, appState.currentColor.y), appState.currentColor.z);
                        float min = std::min(std::min(appState.currentColor.x, appState.currentColor.y), appState.currentColor.z);
                        float delta = max - min;
                        
                        appState.value = max;
                        
                        if (max > 0) 
                            appState.saturation = delta / max;
                        else
                            appState.saturation = 0;
                        
                        if (delta > 0) {
                            if (max == appState.currentColor.x)
                                appState.hue = 60.0f * std::fmod((appState.currentColor.y - appState.currentColor.z) / delta, 6.0f);
                            else if (max == appState.currentColor.y)
                                appState.hue = 60.0f * ((appState.currentColor.z - appState.currentColor.x) / delta + 2.0f);
                            else
                                appState.hue = 60.0f * ((appState.currentColor.x - appState.currentColor.y) / delta + 4.0f);
                        } else {
                            appState.hue = 0;
                        }
                        
                        if (appState.hue < 0)
                            appState.hue += 360.0f;
                            
                        appState.updateHarmonyColors();
                    }
                    
                    if ((i + 1) % 6 != 0 && i < appState.colorHistory.size() - 1)
                        ImGui::SameLine();
                }
            }
            
            ImGui::End();
        }
        
        // Canvas window
        {
            ImGui::Begin("Canvas");
            
            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            ImVec2 cell_size(std::min(canvas_size.x / appState.canvasWidth, canvas_size.y / appState.canvasHeight));
            
            // Draw grid and pixels
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            for (int y = 0; y < appState.canvasHeight; y++) {
                for (int x = 0; x < appState.canvasWidth; x++) {
                    ImVec2 p1 = ImVec2(canvas_pos.x + x * cell_size.x, canvas_pos.y + y * cell_size.y);
                    ImVec2 p2 = ImVec2(p1.x + cell_size.x - 1, p1.y + cell_size.y - 1);
                    
                    // Draw cell
                    ImVec4 cellColor = appState.canvasPixels[y * appState.canvasWidth + x];
                    if (cellColor.w > 0) {
                        // Draw filled pixel
                        draw_list->AddRectFilled(
                            p1, p2, 
                            IM_COL32(cellColor.x * 255, cellColor.y * 255, cellColor.z * 255, cellColor.w * 255)
                        );
                    } else {
                        // Draw empty pixel (checkerboard pattern for transparency)
                        bool isWhite = (x + y) % 2 == 0;
                        ImU32 bgColor = isWhite ? IM_COL32(210, 210, 210, 255) : IM_COL32(180, 180, 180, 255);
                        draw_list->AddRectFilled(p1, p2, bgColor);
                    }
                    
                    // Draw grid
                    draw_list->AddRect(p1, p2, IM_COL32(100, 100, 100, 50));
                }
            }
            
            // Handle mouse interaction with canvas
            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                ImVec2 mouse = ImGui::GetIO().MousePos;
                int x = static_cast<int>((mouse.x - canvas_pos.x) / cell_size.x);
                int y = static_cast<int>((mouse.y - canvas_pos.y) / cell_size.y);
                
                if (x >= 0 && x < appState.canvasWidth && y >= 0 && y < appState.canvasHeight) {
                    appState.paint(x, y);
                }
            }
            
            ImGui::End();
        }
        
        // Layers window
        {
            ImGui::Begin("Layers");
            
            // Display layers in reverse order (top layer first)
            for (int i = appState.layers.size() - 1; i >= 0; i--) {
                const auto& layer = appState.layers[i];
                
                // Layer name with selection
                bool isSelected = (i == appState.currentLayer);
                if (ImGui::Selectable(layer.name.c_str(), isSelected)) {
                    appState.currentLayer = i;
                }
                
                // Visibility toggle
                ImGui::SameLine(ImGui::GetWindowWidth() - 30);
                ImGui::PushID(i);
                bool visible = layer.visible;
                if (ImGui::Checkbox("##visible", &visible)) {
                    appState.layers[i].visible = visible;
                    appState.renderCanvas();
                }
                ImGui::PopID();
            }
            
            ImGui::End();
        }
        
        // Export window
        {
            ImGui::Begin("Export");
            
            if (ImGui::Button("Export as PNG")) {
                // This would be implemented with stb_image_write or similar
                ImGui::Text("PNG Export would happen here");
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Export as OBJ")) {
                // This would coordinate with the 3D painting functionality
                ImGui::Text("OBJ Export would happen here");
            }
            
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}