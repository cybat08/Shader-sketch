#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

// Simple 3D Model Painter application
// This is a minimal version to demonstrate the core architecture

class Texture {
public:
    Texture(int width, int height) : width(width), height(height) {
        std::cout << "Created texture: " << width << "x" << height << std::endl;
    }
    
    void clear() {
        std::cout << "Cleared texture" << std::endl;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int width;
    int height;
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
    
    void paint(int x, int y, float r, float g, float b, float a, float radius, float hardness) {
        std::cout << "Painting at " << x << "," << y << " with radius " << radius << std::endl;
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
        
        layers.push_back(new Layer(1024, 1024, layerName));
        currentLayerIndex = layers.size() - 1;
        
        return layers.back();
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
    PaintTool(const std::string& name) : name(name), size(10.0f), hardness(0.5f), 
        r(1.0f), g(0.0f), b(0.0f), a(1.0f) {
        std::cout << "Created paint tool: " << name << std::endl;
    }
    
    virtual void use(Layer* layer, int x, int y) {
        if (layer) {
            layer->paint(x, y, r, g, b, a, size, hardness);
        }
    }
    
    const std::string& getName() const { return name; }
    float getSize() const { return size; }
    void setSize(float size) { this->size = size; }
    float getHardness() const { return hardness; }
    void setHardness(float hardness) { this->hardness = hardness; }
    
protected:
    std::string name;
    float size;
    float hardness;
    float r, g, b, a;
};

class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush") {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser") {
        // Set colors to transparent for eraser
        r = g = b = a = 0.0f;
    }
};

class Application {
public:
    Application() : currentTool(nullptr) {
        std::cout << "Application initialized" << std::endl;
        
        // Initialize tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        
        // Set default tool
        currentTool = tools[0];
    }
    
    void run() {
        std::cout << "Running 3D Model Painter" << std::endl;
        
        // Load a sample model
        project.loadModel("cube.obj");
        
        // Add a default layer
        project.addLayer("Base Layer");
        
        // Simulate some painting
        if (currentTool && project.getCurrentLayer()) {
            std::cout << "Using " << currentTool->getName() << " tool" << std::endl;
            currentTool->use(project.getCurrentLayer(), 100, 100);
        }
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
};

int main() {
    try {
        Application app;
        app.run();
        std::cout << "Application ran successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}