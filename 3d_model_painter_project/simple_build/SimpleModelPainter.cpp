#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>

// Simple 3D Model Painter application with text-based UI
// This is a minimal version to demonstrate the core architecture

class Texture {
public:
    Texture(int width, int height) : width(width), height(height), data(width * height, ' ') {
        std::cout << "Created texture: " << width << "x" << height << std::endl;
    }
    
    void clear() {
        std::fill(data.begin(), data.end(), ' ');
        std::cout << "Cleared texture" << std::endl;
    }
    
    void paint(int x, int y, char brush, float radius) {
        int r = static_cast<int>(radius);
        
        for (int i = -r; i <= r; i++) {
            for (int j = -r; j <= r; j++) {
                int px = x + i;
                int py = y + j;
                
                // Simple distance check for circular brush
                if (px >= 0 && px < width && py >= 0 && py < height &&
                    (i*i + j*j) <= r*r) {
                    data[py * width + px] = brush;
                }
            }
        }
    }
    
    void render() const {
        // Draw border
        std::cout << "+" << std::string(width, '-') << "+" << std::endl;
        
        // Draw texture content
        for (int y = 0; y < height; y++) {
            std::cout << "|";
            for (int x = 0; x < width; x++) {
                std::cout << data[y * width + x];
            }
            std::cout << "|" << std::endl;
        }
        
        // Draw border
        std::cout << "+" << std::string(width, '-') << "+" << std::endl;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int width;
    int height;
    std::vector<char> data;
};

class Layer {
public:
    Layer(int width, int height, const std::string& name = "Layer") 
        : name(name), visible(true), opacity(1.0f), texture(width, height) {
        std::cout << "Created layer: " << name << std::endl;
    }
    
    void clear() {
        texture.clear();
        std::cout << "Cleared layer: " << name << std::endl;
    }
    
    void paint(int x, int y, char brush, float radius) {
        texture.paint(x, y, brush, radius);
        std::cout << "Painting at " << x << "," << y << " with brush " << brush << std::endl;
    }
    
    void render() const {
        if (visible) {
            texture.render();
        }
    }
    
    const std::string& getName() const { return name; }
    bool isVisible() const { return visible; }
    float getOpacity() const { return opacity; }
    void setVisible(bool visible) { this->visible = visible; }
    void setOpacity(float opacity) { this->opacity = opacity; }
    
private:
    std::string name;
    bool visible;
    float opacity;
    Texture texture;
};

class Model {
public:
    Model() : loaded(false) {
        std::cout << "Model created" << std::endl;
    }
    
    bool loadModel(const std::string& path) {
        std::cout << "Loading model from: " << path << std::endl;
        loaded = true;
        return true;
    }
    
    bool isLoaded() const { return loaded; }
    
private:
    bool loaded;
};

class Project {
public:
    Project() : currentLayerIndex(0) {
        std::cout << "Project created" << std::endl;
    }
    
    bool loadModel(const std::string& path) {
        return model.loadModel(path);
    }
    
    Layer* addLayer(const std::string& name = "") {
        static int layerCounter = 1;
        std::string layerName = name;
        
        if (layerName.empty()) {
            layerName = "Layer " + std::to_string(layerCounter++);
        }
        
        layers.push_back(new Layer(20, 10, layerName));
        currentLayerIndex = layers.size() - 1;
        
        return layers.back();
    }
    
    void renderLayers() const {
        for (const auto* layer : layers) {
            if (layer->isVisible()) {
                std::cout << "Layer: " << layer->getName() << std::endl;
                layer->render();
                std::cout << std::endl;
            }
        }
    }
    
    Layer* getCurrentLayer() {
        if (layers.empty() || currentLayerIndex >= layers.size()) {
            return nullptr;
        }
        
        return layers[currentLayerIndex];
    }
    
    size_t getCurrentLayerIndex() const { return currentLayerIndex; }
    void setCurrentLayerIndex(size_t index) { 
        if (index < layers.size()) {
            currentLayerIndex = index;
        }
    }
    
    bool hasModel() const { return model.isLoaded(); }
    const Model& getModel() const { return model; }
    
    ~Project() {
        for (auto* layer : layers) {
            delete layer;
        }
    }
    
private:
    Model model;
    std::vector<Layer*> layers;
    size_t currentLayerIndex;
};

class PaintTool {
public:
    PaintTool(const std::string& name, char brush = '*') 
        : name(name), size(2.0f), brush(brush) {
        std::cout << "Created paint tool: " << name << std::endl;
    }
    
    virtual void use(Layer* layer, int x, int y) {
        if (layer) {
            layer->paint(x, y, brush, size);
        }
    }
    
    const std::string& getName() const { return name; }
    float getSize() const { return size; }
    void setSize(float size) { this->size = size; }
    char getBrush() const { return brush; }
    void setBrush(char brush) { this->brush = brush; }
    
protected:
    std::string name;
    float size;
    char brush;
};

class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", '*') {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", ' ') {}
};

class LineArtTool : public PaintTool {
public:
    LineArtTool() : PaintTool("LineArt", '#') {}
};

class Application {
public:
    Application() : currentTool(nullptr), running(true) {
        std::cout << "Application initialized" << std::endl;
        
        // Initialize tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        tools.push_back(new LineArtTool());
        
        // Set default tool
        currentTool = tools[0];
        
        // Load a sample model and add a default layer
        project.loadModel("cube.obj");
        project.addLayer("Base Layer");
    }
    
    void showHelp() {
        std::cout << "===== 3D Model Painter Help =====" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  1, 2, 3    - Select tool (Brush, Eraser, LineArt)" << std::endl;
        std::cout << "  +, -       - Increase/decrease brush size" << std::endl;
        std::cout << "  p x y      - Paint at position (x,y)" << std::endl;
        std::cout << "  c          - Clear current layer" << std::endl;
        std::cout << "  n [name]   - New layer with optional name" << std::endl;
        std::cout << "  l          - List layers" << std::endl;
        std::cout << "  s index    - Select layer by index" << std::endl;
        std::cout << "  v          - View canvas" << std::endl;
        std::cout << "  h          - Show this help" << std::endl;
        std::cout << "  q          - Quit" << std::endl;
        std::cout << "===============================" << std::endl;
    }
    
    void run() {
        std::cout << "Running 3D Model Painter" << std::endl;
        showHelp();
        
        // Draw a sample
        auto* layer = project.getCurrentLayer();
        if (layer && currentTool) {
            currentTool->use(layer, 10, 5);
        }
        
        // Main application loop
        std::string command;
        while (running) {
            std::cout << "\nCommand (h for help): ";
            std::getline(std::cin, command);
            
            if (command.empty()) {
                continue;
            }
            
            char cmd = command[0];
            switch (cmd) {
                case '1':
                case '2':
                case '3': {
                    int toolIndex = cmd - '1';
                    if (toolIndex >= 0 && toolIndex < tools.size()) {
                        currentTool = tools[toolIndex];
                        std::cout << "Selected tool: " << currentTool->getName() << std::endl;
                    }
                    break;
                }
                case '+': {
                    if (currentTool) {
                        float newSize = currentTool->getSize() + 1.0f;
                        currentTool->setSize(newSize);
                        std::cout << "Increased brush size to " << newSize << std::endl;
                    }
                    break;
                }
                case '-': {
                    if (currentTool) {
                        float newSize = std::max(1.0f, currentTool->getSize() - 1.0f);
                        currentTool->setSize(newSize);
                        std::cout << "Decreased brush size to " << newSize << std::endl;
                    }
                    break;
                }
                case 'p': {
                    int x, y;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d %d", &x, &y) == 2) {
                        if (currentTool && project.getCurrentLayer()) {
                            currentTool->use(project.getCurrentLayer(), x, y);
                            std::cout << "Canvas after painting:" << std::endl;
                            project.getCurrentLayer()->render();
                        }
                    } else {
                        std::cout << "Invalid paint command. Format: p x y" << std::endl;
                    }
                    break;
                }
                case 'c': {
                    if (project.getCurrentLayer()) {
                        project.getCurrentLayer()->clear();
                        std::cout << "Layer cleared" << std::endl;
                    }
                    break;
                }
                case 'n': {
                    std::string name;
                    if (command.length() > 2) {
                        name = command.substr(2);
                    }
                    Layer* newLayer = project.addLayer(name);
                    std::cout << "Created new layer: " << newLayer->getName() << std::endl;
                    break;
                }
                case 'l': {
                    std::cout << "Layers:" << std::endl;
                    project.renderLayers();
                    break;
                }
                case 's': {
                    int index;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d", &index) == 1) {
                        project.setCurrentLayerIndex(index);
                        std::cout << "Selected layer " << index << std::endl;
                    } else {
                        std::cout << "Invalid select command. Format: s index" << std::endl;
                    }
                    break;
                }
                case 'v': {
                    std::cout << "Current canvas:" << std::endl;
                    project.renderLayers();
                    break;
                }
                case 'h': {
                    showHelp();
                    break;
                }
                case 'q': {
                    running = false;
                    std::cout << "Quitting application..." << std::endl;
                    break;
                }
                default: {
                    std::cout << "Unknown command: " << cmd << ". Type 'h' for help." << std::endl;
                    break;
                }
            }
        }
    }
    
    // Function to run a demo without user input (for automated testing/workflow)
    void runDemo() {
        std::cout << "Running 3D Model Painter Demo" << std::endl;
        
        // Create some layers
        project.addLayer("Details Layer");
        
        // Draw on first layer
        auto* baseLayer = project.getCurrentLayer();
        project.setCurrentLayerIndex(0);  // Select base layer
        
        // Use brush to draw a shape
        currentTool = tools[0];  // Brush
        currentTool->setSize(3.0f);
        for (int i = 3; i < 17; i++) {
            currentTool->use(baseLayer, i, 4);
        }
        
        // Draw on second layer
        project.setCurrentLayerIndex(1);  // Select details layer
        auto* detailsLayer = project.getCurrentLayer();
        
        // Use line art tool
        currentTool = tools[2];  // LineArt
        currentTool->setSize(1.0f);
        for (int i = 3; i < 17; i += 2) {
            currentTool->use(detailsLayer, i, 6);
        }
        
        // Show the results
        std::cout << "\nFinal canvas result:" << std::endl;
        project.renderLayers();
        
        std::cout << "\n3D Model Painter Demo completed successfully!" << std::endl;
    }
    
    ~Application() {
        for (auto* tool : tools) {
            delete tool;
        }
    }
    
private:
    Project project;
    std::vector<PaintTool*> tools;
    PaintTool* currentTool;
    bool running;
};

int main(int argc, char* argv[]) {
    try {
        Application app;
        
        if (argc > 1 && std::string(argv[1]) == "--demo") {
            app.runDemo();
        } else {
            app.run();
        }
        
        std::cout << "Application ran successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}