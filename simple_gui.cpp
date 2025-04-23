#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <array>
#include <iomanip>

/**
 * ShaderSketch Simple GUI Implementation
 * A lightweight GUI implementation that doesn't rely on external libraries
 * for cross-platform compatibility.
 */

// Constants for display
const int CANVAS_WIDTH = 40;
const int CANVAS_HEIGHT = 20;
const int COLOR_COUNT = 8;

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
    }
    
    // Clear the screen
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
    
    // Draw the main interface
    void drawInterface() {
        clearScreen();
        
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
        
        // Draw canvas
        drawCanvas();
        
        // Draw command prompt
        std::cout << std::endl;
        std::cout << BOLD << "Commands:" << RESET << std::endl;
        std::cout << "  t [0-" << (tools.size()-1) << "] - Select tool" << std::endl;
        std::cout << "  c [0-" << (colors.size()-1) << "] - Select color" << std::endl;
        std::cout << "  b [size] - Change brush size" << std::endl;
        std::cout << "  p [x] [y] - Paint at position" << std::endl;
        std::cout << "  l [index] - Select layer" << std::endl;
        std::cout << "  n [name] - New layer" << std::endl;
        std::cout << "  v [index] - Toggle layer visibility" << std::endl;
        std::cout << "  cl - Clear current layer" << std::endl;
        std::cout << "  q - Quit" << std::endl;
        
        std::cout << std::endl << "> ";
    }
    
    // Draw the canvas
    void drawCanvas() {
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