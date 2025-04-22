#pragma once

#include "camera.h"
#include "model.h"
#include "renderer.h"
#include "ui.h"
#include "paint_tool.h"
#include "project.h"

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>

class Application {
public:
    Application();
    ~Application();
    
    void run();
    
private:
    // GLFW window
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    
    // Application components
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<UI> ui;
    std::unique_ptr<Project> project;
    std::vector<std::unique_ptr<PaintTool>> paintTools;
    
    // Current state
    PaintTool* currentTool;
    
    // Mouse state
    double lastMouseX;
    double lastMouseY;
    bool mousePressed;
    
    // Initialize application
    void initGLFW();
    void initUI();
    void initTools();
    
    // GLFW callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    // Set up callback context
    void setupCallbacks();
    
    // Handle input
    void processInput();
    
    // Update and render
    void update(float deltaTime);
    void render();
};
