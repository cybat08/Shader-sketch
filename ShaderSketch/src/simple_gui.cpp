#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <array>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include "../include/color_wheel.h"

/**
 * ShaderSketch Simple GUI Implementation
 * A lightweight GUI implementation that doesn't rely on external libraries
 * for cross-platform compatibility.
 * 
 * Features:
 * - Text-based GUI with color support
 * - 3D model preview and interaction
 * - Layer management
 * - Multiple painting tools
 */

// Constants for display
const int CANVAS_WIDTH = 40;
const int CANVAS_HEIGHT = 20;
const int COLOR_COUNT = 8;
const int MODEL_VIEW_WIDTH = 20;
const int MODEL_VIEW_HEIGHT = 15;

// Color definitions (ANSI colors)
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";
const std::string BG_BLACK = "\033[40m";

// Define a basic Color class
struct Color {
    int r, g, b;
    std::string name;
    std::string ansiCode;
    
    Color(int red, int green, int blue, const std::string& colorName, const std::string& code) :
        r(red), g(green), b(blue), name(colorName), ansiCode(code) {}
        
    std::string getColoredChar(char c) const {
        return ansiCode + c + RESET;
    }
};

// 3D Model classes
struct Vertex {
    float x, y, z;
    Vertex(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
};

struct Triangle {
    int v1, v2, v3;  // Vertex indices
    int textureID = 0;
    int colorIndex = -1;  // -1 means no color assigned
    bool isPainted = false;
    Triangle(int v1_ = 0, int v2_ = 0, int v3_ = 0) : v1(v1_), v2(v2_), v3(v3_) {}
};

struct Model {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    
    // UV coordinates for texture mapping
    std::vector<std::pair<float, float>> uvCoords;
    
    // Model transformation
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float scaleZ = 1.0f;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    
    bool loadFromOBJ(const std::string& filename) {
        // Very simple OBJ loader
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open OBJ file: " << filename << std::endl;
            createDefaultCube();
            return false;
        }
        
        vertices.clear();
        triangles.clear();
        uvCoords.clear();
        
        std::string line;
        std::vector<std::pair<float, float>> tempUVs;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            
            if (type == "v") {
                // Vertex
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(Vertex(x, y, z));
            }
            else if (type == "vt") {
                // Texture coordinate
                float u, v;
                iss >> u >> v;
                tempUVs.push_back(std::make_pair(u, v));
            }
            else if (type == "f") {
                // Face (assuming triangles)
                std::string v1, v2, v3;
                iss >> v1 >> v2 >> v3;
                
                // Parse face indices (format: vertex/texture/normal)
                auto parseVertex = [](const std::string& str) -> std::pair<int, int> {
                    std::istringstream viss(str);
                    std::string indexStr;
                    std::vector<int> indices;
                    
                    while (std::getline(viss, indexStr, '/')) {
                        if (indexStr.empty()) {
                            indices.push_back(0);
                        } else {
                            indices.push_back(std::stoi(indexStr));
                        }
                    }
                    
                    // Return vertex and texture indices (1-based in OBJ format)
                    return std::make_pair(
                        indices.size() > 0 ? indices[0] - 1 : 0,
                        indices.size() > 1 ? indices[1] - 1 : 0
                    );
                };
                
                auto [v1idx, vt1] = parseVertex(v1);
                auto [v2idx, vt2] = parseVertex(v2);
                auto [v3idx, vt3] = parseVertex(v3);
                
                Triangle t(v1idx, v2idx, v3idx);
                triangles.push_back(t);
                
                // Store UV coordinates for this triangle
                if (!tempUVs.empty()) {
                    if (vt1 >= 0 && vt1 < static_cast<int>(tempUVs.size())) uvCoords.push_back(tempUVs[vt1]);
                    if (vt2 >= 0 && vt2 < static_cast<int>(tempUVs.size())) uvCoords.push_back(tempUVs[vt2]);
                    if (vt3 >= 0 && vt3 < static_cast<int>(tempUVs.size())) uvCoords.push_back(tempUVs[vt3]);
                }
            }
        }
        
        file.close();
        
        if (vertices.empty() || triangles.empty()) {
            std::cerr << "Empty or invalid OBJ file: " << filename << std::endl;
            createDefaultCube();
            return false;
        }
        
        // Normalize model size
        normalizeModel();
        
        std::cout << "Loaded model with " << vertices.size() << " vertices and " 
                  << triangles.size() << " triangles." << std::endl;
        return true;
    }
    
    void createDefaultCube() {
        // Create a simple cube as default model
        vertices = {
            Vertex(-1.0f, -1.0f, -1.0f),  // 0
            Vertex(1.0f, -1.0f, -1.0f),   // 1
            Vertex(1.0f, 1.0f, -1.0f),    // 2
            Vertex(-1.0f, 1.0f, -1.0f),   // 3
            Vertex(-1.0f, -1.0f, 1.0f),   // 4
            Vertex(1.0f, -1.0f, 1.0f),    // 5
            Vertex(1.0f, 1.0f, 1.0f),     // 6
            Vertex(-1.0f, 1.0f, 1.0f)     // 7
        };
        
        triangles = {
            // Front
            Triangle(0, 1, 2), Triangle(0, 2, 3),
            // Right
            Triangle(1, 5, 6), Triangle(1, 6, 2),
            // Back
            Triangle(5, 4, 7), Triangle(5, 7, 6),
            // Left
            Triangle(4, 0, 3), Triangle(4, 3, 7),
            // Top
            Triangle(3, 2, 6), Triangle(3, 6, 7),
            // Bottom
            Triangle(4, 5, 1), Triangle(4, 1, 0)
        };
        
        // Add basic UV coordinates
        for (size_t i = 0; i < 12; ++i) {
            uvCoords.push_back(std::make_pair(0.0f, 0.0f));
            uvCoords.push_back(std::make_pair(1.0f, 0.0f));
            uvCoords.push_back(std::make_pair(1.0f, 1.0f));
        }
        
        name = "Default Cube";
        std::cout << "Created default cube model." << std::endl;
    }
    
    void normalizeModel() {
        // Find min and max values
        float minX = vertices[0].x, maxX = vertices[0].x;
        float minY = vertices[0].y, maxY = vertices[0].y;
        float minZ = vertices[0].z, maxZ = vertices[0].z;
        
        for (const auto& v : vertices) {
            minX = std::min(minX, v.x);
            maxX = std::max(maxX, v.x);
            minY = std::min(minY, v.y);
            maxY = std::max(maxY, v.y);
            minZ = std::min(minZ, v.z);
            maxZ = std::max(maxZ, v.z);
        }
        
        // Calculate center
        float centerX = (minX + maxX) / 2.0f;
        float centerY = (minY + maxY) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;
        
        // Calculate scale factor
        float maxDim = std::max({maxX - minX, maxY - minY, maxZ - minZ});
        float scale = 2.0f / maxDim;
        
        // Normalize vertices
        for (auto& v : vertices) {
            v.x = (v.x - centerX) * scale;
            v.y = (v.y - centerY) * scale;
            v.z = (v.z - centerZ) * scale;
        }
    }
    
    // Rotate model by given angles (in radians)
    void rotate(float x, float y, float z) {
        rotationX += x;
        rotationY += y;
        rotationZ += z;
    }
    
    // Scale model
    void scale(float x, float y, float z) {
        scaleX *= x;
        scaleY *= y;
        scaleZ *= z;
    }
    
    // Translate model
    void translate(float x, float y, float z) {
        positionX += x;
        positionY += y;
        positionZ += z;
    }
    
    // Paint a triangle with the specified color
    bool paintTriangle(int triangleIndex, int colorIndex) {
        if (triangleIndex >= 0 && triangleIndex < static_cast<int>(triangles.size())) {
            triangles[triangleIndex].colorIndex = colorIndex;
            triangles[triangleIndex].isPainted = true;
            return true;
        }
        return false;
    }
    
    // Get the triangle at a specific screen position from the rendered view
    int getTriangleAtPosition(int x, int y, const std::vector<std::vector<int>>& triangleBuffer) {
        if (x >= 0 && x < MODEL_VIEW_WIDTH && y >= 0 && y < MODEL_VIEW_HEIGHT) {
            return triangleBuffer[y][x];
        }
        return -1;
    }
};

// Define basic app state
struct AppState {
    // Canvas storage
    std::vector<std::vector<int>> canvas;
    
    // Available colors
    std::vector<Color> colors;
    
    // Current settings
    int currentColor = 0;
    int currentTool = 0;
    float brushSize = 1.0f;
    
    // 3D Model
    std::shared_ptr<Model> model;
    bool showModel = true;
    bool modelLoaded = false;
    std::vector<std::vector<int>> triangleBuffer; // Stores triangle indices for each pixel in the model view
    
    // Layer system
    struct Layer {
        std::string name;
        std::vector<std::vector<int>> pixels;
        bool visible = true;
    };
    std::vector<Layer> layers;
    int currentLayer = 0;
    
    // Tools
    const std::vector<std::string> tools = {
        "Brush", "Eraser", "Line", "Fill", "Rectangle"
    };
    
    // Color wheel for interactive color selection and harmony
    color_wheel::ColorWheel colorWheel;
    bool showColorWheel = false;
    
    // Initialize the application
    AppState() {
        // Initialize colors
        colors = {
            Color(0, 0, 0, "Black", "\033[30m"),
            Color(255, 0, 0, "Red", "\033[31m"),
            Color(0, 255, 0, "Green", "\033[32m"),
            Color(255, 255, 0, "Yellow", "\033[33m"),
            Color(0, 0, 255, "Blue", "\033[34m"),
            Color(255, 0, 255, "Magenta", "\033[35m"),
            Color(0, 255, 255, "Cyan", "\033[36m"),
            Color(255, 255, 255, "White", "\033[37m")
        };
        
        // Initialize canvas
        canvas.resize(CANVAS_HEIGHT, std::vector<int>(CANVAS_WIDTH, -1));
        
        // Create a default layer
        Layer defaultLayer;
        defaultLayer.name = "Background";
        defaultLayer.pixels.resize(CANVAS_HEIGHT, std::vector<int>(CANVAS_WIDTH, -1));
        layers.push_back(defaultLayer);
        
        // Initialize 3D model
        model = std::make_shared<Model>();
        
        // Try to load a model from "cube.obj" if it exists
        if (!model->loadFromOBJ("cube.obj")) {
            // If loading failed, create a default cube
            model->createDefaultCube();
        }
        
        modelLoaded = true;
    }
    
    // Clear the screen
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
    
    // Display the interactive color wheel
    void displayColorWheel() {
        std::cout << BOLD << "Interactive Color Wheel with Harmony Suggestions" << RESET << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        // Display the current color wheel and harmonies
        colorWheel.display();
        
        // Get harmony colors
        auto harmonyColors = colorWheel.getCurrentHarmonyColors();
        
        // Display color harmony controls
        std::cout << std::endl;
        std::cout << BOLD << "Harmony Controls:" << RESET << std::endl;
        std::cout << "  h [type] - Change harmony type (0-6)" << std::endl;
        std::cout << "  s [color] - Select harmony color (1-" << harmonyColors.size() << ")" << std::endl;
        std::cout << "  u - Use selected color" << std::endl;
        std::cout << "  a - Adjust hue/saturation/value" << std::endl;
        std::cout << "  x - Exit color wheel" << std::endl;
        
        std::cout << std::endl;
        std::cout << BOLD << "Current Harmony: " << RESET 
                  << colorWheel.getCurrentHarmonyName() 
                  << " (" << (colorWheel.getSelectedHarmony() + 1) << "/" 
                  << colorWheel.getHarmoniesCount() << ")" << std::endl;
        
        // Display the harmony colors as a palette
        std::cout << BOLD << "Harmony Palette:" << RESET << std::endl;
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            std::cout << "  " << (i + 1) << ": ";
            for (int j = 0; j < 5; j++) { // Show a larger color sample
                std::cout << harmonyColors[i].getColoredChar('#');
            }
            std::cout << " " << harmonyColors[i].getHexCode();
            
            if (i == 0) {
                std::cout << " (Base)";
            }
            std::cout << std::endl;
        }
        
        std::cout << std::endl << "> ";
    }
    
    // Convert from color_wheel::Color to application Color
    void addColorFromColorWheel(const color_wheel::Color& wheelColor) {
        // Only add if we're under the color limit
        if (colors.size() < COLOR_COUNT) {
            // Create ANSI color code
            std::string ansiCode = "\033[38;2;" + 
                                std::to_string(wheelColor.r) + ";" + 
                                std::to_string(wheelColor.g) + ";" + 
                                std::to_string(wheelColor.b) + "m";
            
            // Add the color to the palette
            colors.push_back(Color(wheelColor.r, wheelColor.g, wheelColor.b, 
                                  wheelColor.getHexCode(), ansiCode));
            
            // Select the new color
            currentColor = colors.size() - 1;
        } else {
            // Replace the current color
            colors[currentColor] = Color(wheelColor.r, wheelColor.g, wheelColor.b, 
                                        wheelColor.getHexCode(), 
                                        wheelColor.getANSIColor());
        }
        
        std::cout << "Color updated to " << wheelColor.getHexCode() << std::endl;
    }
    
    // Handle color wheel interaction
    bool handleColorWheelCommands(const std::string& command) {
        if (command.empty()) {
            return true;
        }
        
        // Split command into parts
        std::vector<std::string> parts;
        std::string part;
        for (char c : command) {
            if (c == ' ') {
                if (!part.empty()) {
                    parts.push_back(part);
                    part.clear();
                }
            } else {
                part += c;
            }
        }
        if (!part.empty()) {
            parts.push_back(part);
        }
        
        if (parts.empty()) {
            return true;
        }
        
        std::string cmd = parts[0];
        
        if (cmd == "x") {
            showColorWheel = false;
            std::cout << "Exiting color wheel..." << std::endl;
            return true;
        } else if (cmd == "h" && parts.size() > 1) {
            // Change harmony type
            try {
                int harmonyType = std::stoi(parts[1]);
                if (harmonyType >= 0 && harmonyType < colorWheel.getHarmoniesCount()) {
                    for (int i = 0; i < colorWheel.getHarmoniesCount(); i++) {
                        if (i != harmonyType) colorWheel.nextHarmony();
                    }
                    std::cout << "Switched to " << colorWheel.getCurrentHarmonyName() << " harmony." << std::endl;
                } else {
                    std::cout << "Invalid harmony type. Choose 0-" << (colorWheel.getHarmoniesCount() - 1) << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid harmony type" << std::endl;
            }
        } else if (cmd == "s" && parts.size() > 1) {
            // Select harmony color
            try {
                int colorIndex = std::stoi(parts[1]) - 1; // 1-based indexing for user
                auto harmonyColors = colorWheel.getCurrentHarmonyColors();
                if (colorIndex >= 0 && colorIndex < static_cast<int>(harmonyColors.size())) {
                    colorWheel.setColor(harmonyColors[colorIndex]);
                    std::cout << "Selected harmony color " << (colorIndex + 1) << std::endl;
                } else {
                    std::cout << "Invalid color index. Choose 1-" << harmonyColors.size() << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid color index" << std::endl;
            }
        } else if (cmd == "u") {
            // Use the selected color
            addColorFromColorWheel(colorWheel.getSelectedColor());
            showColorWheel = false;
            return true;
        } else if (cmd == "a") {
            // Simple HSV adjustment mode
            std::cout << "Enter new HSV values (hue 0-360, saturation 0-100, value 0-100): ";
            std::string input;
            std::getline(std::cin, input);
            
            std::istringstream iss(input);
            float h, s, v;
            if (iss >> h >> s >> v) {
                // Normalize saturation and value to 0-1 range
                s /= 100.0f;
                v /= 100.0f;
                
                // Clamp values to valid ranges
                h = std::min(360.0f, std::max(0.0f, h));
                s = std::min(1.0f, std::max(0.0f, s));
                v = std::min(1.0f, std::max(0.0f, v));
                
                colorWheel.setColorHSV(h, s, v);
                std::cout << "Color updated to HSV(" << h << ", " << (s * 100) << "%, " << (v * 100) << "%)" << std::endl;
            } else {
                std::cout << "Invalid HSV values" << std::endl;
            }
        } else {
            std::cout << "Invalid color wheel command" << std::endl;
        }
        
        return true;
    }

    // Draw the main interface
    void drawInterface() {
        clearScreen();
        
        // If we're in color wheel mode, show that instead of the main interface
        if (showColorWheel) {
            displayColorWheel();
            return;
        }
        
        // Draw header
        std::cout << BOLD << "ShaderSketch GUI Mode" << RESET << std::endl;
        std::cout << "A lightweight 3D model painting application" << std::endl;
        std::cout << std::string(50, '-') << std::endl << std::endl;
        
        // Draw tools section
        std::cout << BOLD << "Tools:" << RESET << std::endl;
        for (size_t i = 0; i < tools.size(); ++i) {
            if (i == currentTool) {
                std::cout << "[" << BOLD << "*" << RESET << "] ";
            } else {
                std::cout << "[ ] ";
            }
            std::cout << tools[i] << " ";
        }
        std::cout << std::endl << std::endl;
        
        // Draw colors section
        std::cout << BOLD << "Colors:" << RESET << std::endl;
        for (size_t i = 0; i < colors.size(); ++i) {
            if (i == currentColor) {
                std::cout << "[" << BOLD << "*" << RESET << "] ";
            } else {
                std::cout << "[ ] ";
            }
            std::cout << colors[i].getColoredChar('#') << " ";
        }
        std::cout << std::endl << std::endl;
        
        // Draw brush size
        std::cout << BOLD << "Brush Size: " << RESET << brushSize << std::endl << std::endl;
        
        // Draw layers
        std::cout << BOLD << "Layers:" << RESET << std::endl;
        for (size_t i = 0; i < layers.size(); ++i) {
            if (i == currentLayer) {
                std::cout << "[" << BOLD << "*" << RESET << "] ";
            } else {
                std::cout << "[ ] ";
            }
            std::cout << layers[i].name;
            if (!layers[i].visible) {
                std::cout << " (hidden)";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        // Draw canvas with 3D model
        drawCanvas();
        
        // Draw command prompt
        std::cout << std::endl;
        std::cout << BOLD << "Commands:" << RESET << std::endl;
        std::cout << "  t [0-" << (tools.size()-1) << "] - Select tool" << std::endl;
        std::cout << "  c [0-" << (colors.size()-1) << "] - Select color" << std::endl;
        std::cout << "  wheel - " << MAGENTA << "Open interactive color wheel with harmony suggestions" << RESET << std::endl;
        std::cout << "  b [size] - Change brush size" << std::endl;
        std::cout << "  p [x] [y] - Paint at position" << std::endl;
        std::cout << "  l [index] - Select layer" << std::endl;
        std::cout << "  n [name] - New layer" << std::endl;
        std::cout << "  v [index] - Toggle layer visibility" << std::endl;
        std::cout << "  cl - Clear current layer" << std::endl;
        
        // 3D Model commands
        std::cout << std::endl << BOLD << "3D Model Commands:" << RESET << std::endl;
        std::cout << "  paint3d [x] [y] - " << CYAN << "Paint directly on 3D model at position" << RESET << std::endl;
        std::cout << "  rx [angle] - Rotate around X axis" << std::endl;
        std::cout << "  ry [angle] - Rotate around Y axis" << std::endl;
        std::cout << "  rz [angle] - Rotate around Z axis" << std::endl;
        std::cout << "  load [file] - Load model from OBJ file" << std::endl;
        std::cout << "  reset - Reset model transforms" << std::endl;
        std::cout << "  toggle - Toggle model visibility" << std::endl;
        
        std::cout << std::endl << "  q - Quit" << std::endl;
        
        std::cout << std::endl << "> ";
    }
    
    // Render 3D model to ASCII
    void renderModel() {
        if (!modelLoaded || !model) return;
        
        std::cout << BOLD << "3D Model Preview:" << RESET << " " << model->name << std::endl;
        
        // Create a Z-buffer to handle occlusion
        std::vector<std::vector<float>> zBuffer(MODEL_VIEW_HEIGHT, 
            std::vector<float>(MODEL_VIEW_WIDTH, std::numeric_limits<float>::infinity()));
        
        // Create a frame buffer for the model
        std::vector<std::vector<char>> frameBuffer(MODEL_VIEW_HEIGHT, 
            std::vector<char>(MODEL_VIEW_WIDTH, ' '));
        
        // Create a color buffer for model colors
        std::vector<std::vector<int>> colorBuffer(MODEL_VIEW_HEIGHT, 
            std::vector<int>(MODEL_VIEW_WIDTH, -1));
            
        // Initialize or reset the triangle buffer to track which triangle is at each position
        triangleBuffer.resize(MODEL_VIEW_HEIGHT, 
            std::vector<int>(MODEL_VIEW_WIDTH, -1));
        
        // Simple perspective projection parameters
        const float fNear = 0.1f;
        const float fFar = 100.0f;
        const float fFov = 90.0f;
        const float fAspectRatio = (float)MODEL_VIEW_WIDTH / (float)MODEL_VIEW_HEIGHT;
        const float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
        
        // Define rotation matrices - very simplified
        auto rotateX = [](const Vertex& v, float angle) -> Vertex {
            Vertex result = v;
            result.y = v.y * cosf(angle) - v.z * sinf(angle);
            result.z = v.y * sinf(angle) + v.z * cosf(angle);
            return result;
        };
        
        auto rotateY = [](const Vertex& v, float angle) -> Vertex {
            Vertex result = v;
            result.x = v.x * cosf(angle) + v.z * sinf(angle);
            result.z = -v.x * sinf(angle) + v.z * cosf(angle);
            return result;
        };
        
        auto rotateZ = [](const Vertex& v, float angle) -> Vertex {
            Vertex result = v;
            result.x = v.x * cosf(angle) - v.y * sinf(angle);
            result.y = v.x * sinf(angle) + v.y * cosf(angle);
            return result;
        };
        
        // Draw each triangle
        for (const Triangle& tri : model->triangles) {
            if (tri.v1 >= model->vertices.size() || tri.v2 >= model->vertices.size() || 
                tri.v3 >= model->vertices.size()) {
                continue;  // Skip invalid triangles
            }
            
            // Get vertices
            Vertex v1 = model->vertices[tri.v1];
            Vertex v2 = model->vertices[tri.v2];
            Vertex v3 = model->vertices[tri.v3];
            
            // Apply model transformations
            // Scaling
            v1.x *= model->scaleX; v1.y *= model->scaleY; v1.z *= model->scaleZ;
            v2.x *= model->scaleX; v2.y *= model->scaleY; v2.z *= model->scaleZ;
            v3.x *= model->scaleX; v3.y *= model->scaleY; v3.z *= model->scaleZ;
            
            // Rotation
            v1 = rotateX(v1, model->rotationX);
            v1 = rotateY(v1, model->rotationY);
            v1 = rotateZ(v1, model->rotationZ);
            
            v2 = rotateX(v2, model->rotationX);
            v2 = rotateY(v2, model->rotationY);
            v2 = rotateZ(v2, model->rotationZ);
            
            v3 = rotateX(v3, model->rotationX);
            v3 = rotateY(v3, model->rotationY);
            v3 = rotateZ(v3, model->rotationZ);
            
            // Translation
            v1.x += model->positionX; v1.y += model->positionY; v1.z += model->positionZ;
            v2.x += model->positionX; v2.y += model->positionY; v2.z += model->positionZ;
            v3.x += model->positionX; v3.y += model->positionY; v3.z += model->positionZ;
            
            // Offset for camera position
            v1.z += 3.0f;
            v2.z += 3.0f;
            v3.z += 3.0f;
            
            // Project 3D to 2D
            auto project = [&](const Vertex& v) -> std::pair<int, int> {
                // Basic perspective projection
                float x = v.x * fFovRad / v.z;
                float y = v.y * fFovRad * fAspectRatio / v.z;
                
                // Scale and convert to screen coordinates
                int screenX = (int)((x + 1.0f) * 0.5f * MODEL_VIEW_WIDTH);
                int screenY = (int)((y + 1.0f) * 0.5f * MODEL_VIEW_HEIGHT);
                
                return {screenX, screenY};
            };
            
            // Get 2D coordinates
            auto [x1, y1] = project(v1);
            auto [x2, y2] = project(v2);
            auto [x3, y3] = project(v3);
            
            // Very simple triangle rasterization - just draw outline
            auto drawLine = [&](int x0, int y0, int x1, int y1, float z, int triangleIndex) {
                int dx = abs(x1 - x0);
                int dy = abs(y1 - y0);
                int sx = (x0 < x1) ? 1 : -1;
                int sy = (y0 < y1) ? 1 : -1;
                int err = dx - dy;
                
                while (true) {
                    if (x0 >= 0 && x0 < MODEL_VIEW_WIDTH && y0 >= 0 && y0 < MODEL_VIEW_HEIGHT) {
                        if (z < zBuffer[y0][x0]) {
                            // Use @ symbol for painted triangles
                            char displayChar = '#';
                            int colorIdx = currentColor;
                            
                            // Check if this triangle is painted and use its color
                            if (triangleIndex >= 0 && triangleIndex < static_cast<int>(model->triangles.size())) {
                                const Triangle& tri = model->triangles[triangleIndex];
                                if (tri.isPainted) {
                                    displayChar = '@';
                                    if (tri.colorIndex >= 0 && tri.colorIndex < static_cast<int>(colors.size())) {
                                        colorIdx = tri.colorIndex;
                                    }
                                }
                            }
                            
                            frameBuffer[y0][x0] = displayChar;
                            colorBuffer[y0][x0] = colorIdx;
                            zBuffer[y0][x0] = z;
                            
                            // Store triangle index in the triangle buffer
                            triangleBuffer[y0][x0] = triangleIndex;
                        }
                    }
                    
                    if (x0 == x1 && y0 == y1) break;
                    int e2 = 2 * err;
                    if (e2 > -dy) {
                        err -= dy;
                        x0 += sx;
                    }
                    if (e2 < dx) {
                        err += dx;
                        y0 += sy;
                    }
                }
            };
            
            // Calculate average Z for depth testing
            float avgZ = (v1.z + v2.z + v3.z) / 3.0f;
            
            // Get triangle index in the triangle list
            int triangleIndex = &tri - &model->triangles[0];
            
            // Draw triangle outline
            drawLine(x1, y1, x2, y2, avgZ, triangleIndex);
            drawLine(x2, y2, x3, y3, avgZ, triangleIndex);
            drawLine(x3, y3, x1, y1, avgZ, triangleIndex);
        }
        
        // Draw the 3D model frame
        std::cout << "  +" << std::string(MODEL_VIEW_WIDTH, '-') << "+" << std::endl;
        
        for (int y = 0; y < MODEL_VIEW_HEIGHT; ++y) {
            std::cout << std::setw(2) << y << "|";
            for (int x = 0; x < MODEL_VIEW_WIDTH; ++x) {
                if (frameBuffer[y][x] == ' ') {
                    std::cout << ' ';
                } else {
                    int colorIdx = colorBuffer[y][x];
                    if (colorIdx == -1) colorIdx = 7; // White by default
                    std::cout << colors[colorIdx].getColoredChar(frameBuffer[y][x]);
                }
            }
            std::cout << "|" << std::endl;
        }
        
        std::cout << "  +" << std::string(MODEL_VIEW_WIDTH, '-') << "+" << std::endl;
        
        // Model controls
        std::cout << std::endl;
        std::cout << BOLD << "Model Controls:" << RESET << std::endl;
        std::cout << "  rx [angle] - Rotate around X axis" << std::endl;
        std::cout << "  ry [angle] - Rotate around Y axis" << std::endl;
        std::cout << "  rz [angle] - Rotate around Z axis" << std::endl;
        std::cout << "  load [file] - Load model from OBJ file" << std::endl;
        std::cout << "  reset - Reset model transforms" << std::endl;
    }
    
    // Draw the canvas
    void drawCanvas() {
        // First render the 3D model if visible
        if (showModel && modelLoaded) {
            renderModel();
            std::cout << std::endl;
        }
        
        std::cout << BOLD << "Canvas:" << RESET << std::endl;
        
        // Composite all visible layers
        std::vector<std::vector<int>> compositeCanvas = 
            std::vector<std::vector<int>>(CANVAS_HEIGHT, std::vector<int>(CANVAS_WIDTH, -1));
        
        for (const Layer& layer : layers) {
            if (layer.visible) {
                for (int y = 0; y < CANVAS_HEIGHT; ++y) {
                    for (int x = 0; x < CANVAS_WIDTH; ++x) {
                        if (layer.pixels[y][x] != -1) {
                            compositeCanvas[y][x] = layer.pixels[y][x];
                        }
                    }
                }
            }
        }
        
        // Draw the top border
        std::cout << "  +" << std::string(CANVAS_WIDTH, '-') << "+" << std::endl;
        
        // Draw each row
        for (int y = 0; y < CANVAS_HEIGHT; ++y) {
            std::cout << std::setw(2) << y << "|";
            for (int x = 0; x < CANVAS_WIDTH; ++x) {
                int colorIndex = compositeCanvas[y][x];
                if (colorIndex == -1) {
                    std::cout << " ";
                } else {
                    std::cout << colors[colorIndex].getColoredChar('#');
                }
            }
            std::cout << "|" << std::endl;
        }
        
        // Draw the bottom border
        std::cout << "  +" << std::string(CANVAS_WIDTH, '-') << "+" << std::endl;
        
        // Draw the column numbers (every 5 positions)
        std::cout << "   ";
        for (int x = 0; x < CANVAS_WIDTH; x += 5) {
            std::cout << std::setw(5) << x;
        }
        std::cout << std::endl;
    }
    
    // Handle user input
    bool handleCommand(const std::string& command) {
        if (command.empty()) {
            return true;
        }
        
        // If we're in color wheel mode, use that handler
        if (showColorWheel) {
            return handleColorWheelCommands(command);
        }
        
        // Split command into parts
        std::vector<std::string> parts;
        std::string part;
        for (char c : command) {
            if (c == ' ') {
                if (!part.empty()) {
                    parts.push_back(part);
                    part.clear();
                }
            } else {
                part += c;
            }
        }
        if (!part.empty()) {
            parts.push_back(part);
        }
        
        if (parts.empty()) {
            return true;
        }
        
        // Handle different commands
        std::string cmd = parts[0];
        
        if (cmd == "q") {
            std::cout << "Exiting ShaderSketch GUI..." << std::endl;
            return false;
        }
        else if (cmd == "t" && parts.size() > 1) {
            // Select tool
            try {
                int toolIndex = std::stoi(parts[1]);
                if (toolIndex >= 0 && toolIndex < static_cast<int>(tools.size())) {
                    currentTool = toolIndex;
                    std::cout << "Selected tool: " << tools[currentTool] << std::endl;
                } else {
                    std::cout << "Invalid tool index. Choose 0-" << (tools.size()-1) << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid tool index" << std::endl;
            }
        }
        else if (cmd == "c" && parts.size() > 1) {
            // Select color
            try {
                int colorIndex = std::stoi(parts[1]);
                if (colorIndex >= 0 && colorIndex < static_cast<int>(colors.size())) {
                    currentColor = colorIndex;
                    std::cout << "Selected color: " << colors[currentColor].name << std::endl;
                } else {
                    std::cout << "Invalid color index. Choose 0-" << (colors.size()-1) << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid color index" << std::endl;
            }
        }
        else if (cmd == "b" && parts.size() > 1) {
            // Change brush size
            try {
                float size = std::stof(parts[1]);
                if (size > 0 && size <= 10) {
                    brushSize = size;
                    std::cout << "Brush size set to: " << brushSize << std::endl;
                } else {
                    std::cout << "Invalid brush size. Choose 0.1-10" << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid brush size" << std::endl;
            }
        }
        else if (cmd == "p" && parts.size() > 2) {
            // Paint at position
            try {
                int x = std::stoi(parts[1]);
                int y = std::stoi(parts[2]);
                if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
                    paint(x, y);
                    std::cout << "Painted at position (" << x << ", " << y << ")" << std::endl;
                } else {
                    std::cout << "Position out of canvas bounds" << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid position coordinates" << std::endl;
            }
        }
        else if (cmd == "l" && parts.size() > 1) {
            // Select layer
            try {
                int layerIndex = std::stoi(parts[1]);
                if (layerIndex >= 0 && layerIndex < static_cast<int>(layers.size())) {
                    currentLayer = layerIndex;
                    std::cout << "Selected layer: " << layers[currentLayer].name << std::endl;
                } else {
                    std::cout << "Invalid layer index" << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid layer index" << std::endl;
            }
        }
        else if (cmd == "n" && parts.size() > 1) {
            // Create new layer
            std::string layerName = parts[1];
            for (size_t i = 2; i < parts.size(); ++i) {
                layerName += " " + parts[i];
            }
            createLayer(layerName);
            std::cout << "Created new layer: " << layerName << std::endl;
        }
        else if (cmd == "v" && parts.size() > 1) {
            // Toggle layer visibility
            try {
                int layerIndex = std::stoi(parts[1]);
                if (layerIndex >= 0 && layerIndex < static_cast<int>(layers.size())) {
                    layers[layerIndex].visible = !layers[layerIndex].visible;
                    std::cout << "Toggled visibility of layer: " << layers[layerIndex].name << std::endl;
                } else {
                    std::cout << "Invalid layer index" << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid layer index" << std::endl;
            }
        }
        else if (cmd == "cl") {
            // Clear current layer
            clearLayer();
            std::cout << "Cleared layer: " << layers[currentLayer].name << std::endl;
        }
        // 3D Model commands
        else if (cmd == "rx" && parts.size() > 1) {
            // Rotate around X axis
            try {
                float angle = std::stof(parts[1]) * (3.14159f / 180.0f); // Convert to radians
                model->rotate(angle, 0, 0);
                std::cout << "Rotated model around X axis by " << parts[1] << " degrees" << std::endl;
            } catch (...) {
                std::cout << "Invalid angle" << std::endl;
            }
        }
        else if (cmd == "ry" && parts.size() > 1) {
            // Rotate around Y axis
            try {
                float angle = std::stof(parts[1]) * (3.14159f / 180.0f); // Convert to radians
                model->rotate(0, angle, 0);
                std::cout << "Rotated model around Y axis by " << parts[1] << " degrees" << std::endl;
            } catch (...) {
                std::cout << "Invalid angle" << std::endl;
            }
        }
        else if (cmd == "rz" && parts.size() > 1) {
            // Rotate around Z axis
            try {
                float angle = std::stof(parts[1]) * (3.14159f / 180.0f); // Convert to radians
                model->rotate(0, 0, angle);
                std::cout << "Rotated model around Z axis by " << parts[1] << " degrees" << std::endl;
            } catch (...) {
                std::cout << "Invalid angle" << std::endl;
            }
        }
        else if (cmd == "load" && parts.size() > 1) {
            // Load model from file
            std::string filename = parts[1];
            if (model->loadFromOBJ(filename)) {
                std::cout << "Successfully loaded model from " << filename << std::endl;
            } else {
                std::cout << "Failed to load model from " << filename << ". Using default cube." << std::endl;
            }
        }
        else if (cmd == "reset") {
            // Reset model transforms
            model->rotationX = 0.0f;
            model->rotationY = 0.0f;
            model->rotationZ = 0.0f;
            model->scaleX = 1.0f;
            model->scaleY = 1.0f;
            model->scaleZ = 1.0f;
            model->positionX = 0.0f;
            model->positionY = 0.0f;
            model->positionZ = 0.0f;
            std::cout << "Reset model transforms" << std::endl;
        }
        else if (cmd == "toggle") {
            // Toggle model visibility
            showModel = !showModel;
            std::cout << "Model visibility " << (showModel ? "enabled" : "disabled") << std::endl;
        }
        else if (cmd == "paint3d" && parts.size() > 2) {
            // Paint directly on the 3D model
            try {
                int x = std::stoi(parts[1]);
                int y = std::stoi(parts[2]);
                
                if (x >= 0 && x < MODEL_VIEW_WIDTH && y >= 0 && y < MODEL_VIEW_HEIGHT) {
                    // Get the triangle at this position
                    int triangleIndex = model->getTriangleAtPosition(x, y, triangleBuffer);
                    
                    if (triangleIndex >= 0 && triangleIndex < static_cast<int>(model->triangles.size())) {
                        // Paint the triangle
                        model->paintTriangle(triangleIndex, currentColor);
                        std::cout << "Painted triangle " << triangleIndex << " with " 
                                  << colors[currentColor].name << " color" << std::endl;
                    } else {
                        std::cout << "No triangle found at position (" << x << ", " << y << ")" << std::endl;
                    }
                } else {
                    std::cout << "Position out of model view bounds" << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid position coordinates" << std::endl;
            }
        }
        else if (cmd == "wheel") {
            // Open the interactive color wheel
            showColorWheel = true;
            std::cout << "Opening interactive color wheel..." << std::endl;
        }
        else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
        
        return true;
    }
    
    // Paint at specific coordinates with the current tool and color
    void paint(int x, int y) {
        Layer& layer = layers[currentLayer];
        
        switch (currentTool) {
            case 0: // Brush
                {
                    int brushInt = static_cast<int>(brushSize);
                    for (int dy = -brushInt; dy <= brushInt; ++dy) {
                        for (int dx = -brushInt; dx <= brushInt; ++dx) {
                            int nx = x + dx;
                            int ny = y + dy;
                            if (nx >= 0 && nx < CANVAS_WIDTH && ny >= 0 && ny < CANVAS_HEIGHT) {
                                float distance = std::sqrt(dx*dx + dy*dy);
                                if (distance <= brushSize) {
                                    layer.pixels[ny][nx] = currentColor;
                                }
                            }
                        }
                    }
                }
                break;
                
            case 1: // Eraser
                {
                    int brushInt = static_cast<int>(brushSize);
                    for (int dy = -brushInt; dy <= brushInt; ++dy) {
                        for (int dx = -brushInt; dx <= brushInt; ++dx) {
                            int nx = x + dx;
                            int ny = y + dy;
                            if (nx >= 0 && nx < CANVAS_WIDTH && ny >= 0 && ny < CANVAS_HEIGHT) {
                                float distance = std::sqrt(dx*dx + dy*dy);
                                if (distance <= brushSize) {
                                    layer.pixels[ny][nx] = -1;
                                }
                            }
                        }
                    }
                }
                break;
                
            case 2: // Line (simple implementation - just paints current position)
                layer.pixels[y][x] = currentColor;
                break;
                
            case 3: // Fill (simple implementation - just paints current position)
                layer.pixels[y][x] = currentColor;
                break;
                
            case 4: // Rectangle (simple implementation - just paints current position)
                layer.pixels[y][x] = currentColor;
                break;
                
            default:
                layer.pixels[y][x] = currentColor;
                break;
        }
    }
    
    // Create a new layer
    void createLayer(const std::string& name) {
        Layer newLayer;
        newLayer.name = name;
        newLayer.pixels.resize(CANVAS_HEIGHT, std::vector<int>(CANVAS_WIDTH, -1));
        layers.push_back(newLayer);
        currentLayer = layers.size() - 1;
    }
    
    // Clear the current layer
    void clearLayer() {
        Layer& layer = layers[currentLayer];
        layer.pixels = std::vector<std::vector<int>>(CANVAS_HEIGHT, std::vector<int>(CANVAS_WIDTH, -1));
    }
};

int main() {
    AppState app;
    
    bool running = true;
    while (running) {
        app.drawInterface();
        
        std::string command;
        std::getline(std::cin, command);
        
        running = app.handleCommand(command);
    }
    
    return 0;
}