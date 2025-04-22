#include "application.h"
#include <iostream>
#include <stdexcept>

// Static member to store the current instance for callbacks
static Application* currentInstance = nullptr;

// Callback forwarders
static void mouseButtonCallbackForwarder(GLFWwindow* window, int button, int action, int mods) {
    if (currentInstance) currentInstance->mouseButtonCallback(window, button, action, mods);
}

static void cursorPosCallbackForwarder(GLFWwindow* window, double xpos, double ypos) {
    if (currentInstance) currentInstance->cursorPosCallback(window, xpos, ypos);
}

static void scrollCallbackForwarder(GLFWwindow* window, double xoffset, double yoffset) {
    if (currentInstance) currentInstance->scrollCallback(window, xoffset, yoffset);
}

static void framebufferSizeCallbackForwarder(GLFWwindow* window, int width, int height) {
    if (currentInstance) currentInstance->framebufferSizeCallback(window, width, height);
}

static void keyCallbackForwarder(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (currentInstance) currentInstance->keyCallback(window, key, scancode, action, mods);
}

Application::Application() 
    : window(nullptr), 
      windowWidth(1280), 
      windowHeight(720),
      lastMouseX(0.0),
      lastMouseY(0.0),
      mousePressed(false),
      currentTool(nullptr) {
    
    // Store instance for callbacks
    currentInstance = this;
    
    // Initialize GLFW and create window
    initGLFW();
    
    // Create camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
    
    // Create renderer
    renderer = std::make_unique<Renderer>();
    
    // Create project
    project = std::make_unique<Project>();
    
    // Initialize UI
    initUI();
    
    // Initialize painting tools
    initTools();
    
    // Set up callbacks
    setupCallbacks();
}

Application::~Application() {
    // ImGui cleanup is handled by UI destructor
    
    // Destroy window
    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Reset static instance
    currentInstance = nullptr;
}

void Application::initGLFW() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    window = glfwCreateWindow(windowWidth, windowHeight, "3D Model Painter", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(window);
    
    // Enable vsync
    glfwSwapInterval(1);
}

void Application::setupCallbacks() {
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackForwarder);
    glfwSetMouseButtonCallback(window, mouseButtonCallbackForwarder);
    glfwSetCursorPosCallback(window, cursorPosCallbackForwarder);
    glfwSetScrollCallback(window, scrollCallbackForwarder);
    glfwSetKeyCallback(window, keyCallbackForwarder);
}

void Application::initUI() {
    ui = std::make_unique<UI>(window);
}

void Application::initTools() {
    // Add brush tool
    paintTools.push_back(std::make_unique<BrushTool>());
    
    // Add eraser tool
    paintTools.push_back(std::make_unique<EraserTool>());
    
    // Add fill tool
    paintTools.push_back(std::make_unique<FillTool>());
    
    // Set default tool
    currentTool = paintTools[0].get();
}

void Application::run() {
    float lastFrame = 0.0f;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process input
        processInput();
        
        // Update
        update(deltaTime);
        
        // Render
        render();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::processInput() {
    // Close window on Escape key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::FORWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::BACKWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::LEFT, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::RIGHT, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::UP, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera->processKeyboard(CameraMovement::DOWN, 0.05f);
    }
}

void Application::update(float deltaTime) {
    // Update UI
    ui->update(deltaTime, *project, paintTools, currentTool);
    
    // Process UI commands
    if (ui->shouldLoadModel()) {
        std::string path = ui->getModelPath();
        project->loadModel(path);
        ui->clearModelLoadFlag();
    }
    
    if (ui->shouldSaveProject()) {
        std::string path = ui->getProjectPath();
        project->saveProject(path);
        ui->clearSaveProjectFlag();
    }
    
    if (ui->shouldExportModel()) {
        std::string path = ui->getExportPath();
        project->exportModel(path);
        ui->clearExportModelFlag();
    }
    
    // Update current tool
    currentTool = ui->getSelectedTool();
}

void Application::render() {
    // Clear the screen
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the model if available
    if (project->hasModel()) {
        renderer->render(project->getModel(), *camera, *project);
    }
    
    // Render UI
    ui->render();
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    
    // Update camera aspect ratio
    camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (ui->wantCaptureMouse()) {
        return;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            
            // Start painting if we have a model and a tool
            if (project->hasModel() && currentTool) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                
                // Perform ray casting to determine the 3D position on the model
                glm::vec3 worldPos;
                if (renderer->pickPosition(project->getModel(), *camera, xpos, ypos, windowWidth, windowHeight, worldPos)) {
                    currentTool->begin(project->getCurrentLayer(), worldPos);
                }
            }
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
            
            // End painting
            if (currentTool) {
                currentTool->end();
            }
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            // Store mouse position for camera rotation
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
    }
}

void Application::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (ui->wantCaptureMouse()) {
        return;
    }
    
    if (mousePressed && currentTool && project->hasModel()) {
        // Continue painting
        glm::vec3 worldPos;
        if (renderer->pickPosition(project->getModel(), *camera, xpos, ypos, windowWidth, windowHeight, worldPos)) {
            currentTool->update(worldPos);
        }
    }
    
    // Camera rotation with right mouse button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        float xoffset = static_cast<float>(xpos - lastMouseX);
        float yoffset = static_cast<float>(lastMouseY - ypos); // Reversed since y-coordinates go from bottom to top
        
        lastMouseX = xpos;
        lastMouseY = ypos;
        
        camera->processMouseMovement(xoffset, yoffset);
    }
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (ui->wantCaptureMouse()) {
        return;
    }
    
    // Camera zoom
    camera->processMouseScroll(static_cast<float>(yoffset));
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (ui->wantCaptureKeyboard()) {
        return;
    }
    
    // Handle keyboard shortcuts
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1:
                // Select brush tool
                if (paintTools.size() > 0) {
                    currentTool = paintTools[0].get();
                }
                break;
            case GLFW_KEY_2:
                // Select eraser tool
                if (paintTools.size() > 1) {
                    currentTool = paintTools[1].get();
                }
                break;
            case GLFW_KEY_3:
                // Select fill tool
                if (paintTools.size() > 2) {
                    currentTool = paintTools[2].get();
                }
                break;
            case GLFW_KEY_N:
                // New layer
                if (mods & GLFW_MOD_CONTROL) {
                    project->addLayer();
                }
                break;
            case GLFW_KEY_S:
                // Save project
                if (mods & GLFW_MOD_CONTROL) {
                    ui->setSaveProjectFlag();
                }
                break;
            case GLFW_KEY_O:
                // Open model
                if (mods & GLFW_MOD_CONTROL) {
                    ui->setModelLoadFlag();
                }
                break;
            case GLFW_KEY_E:
                // Export model
                if (mods & GLFW_MOD_CONTROL) {
                    ui->setExportModelFlag();
                }
                break;
        }
    }
}
