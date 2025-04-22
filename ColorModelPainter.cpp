#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <map>
#include <iomanip>
#include <cmath>

// Enhanced 3D Model Painter with improved colors and visual effects
// ANSI color codes for terminal coloring
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

struct Color {
    char symbol;
    std::string code;
    std::string name;
    
    Color(char s = ' ', const std::string& c = RESET, const std::string& n = "None") 
        : symbol(s), code(c), name(n) {}
};

// Enhanced pixel with color information
struct Pixel {
    Color color;
    float opacity;
    
    Pixel() : opacity(0.0f) {}
    Pixel(const Color& c, float o = 1.0f) : color(c), opacity(o) {}
};

class Texture {
public:
    Texture(int width, int height) : width(width), height(height), data(width * height) {
        std::cout << CYAN << "Created texture: " << width << "x" << height << RESET << std::endl;
    }
    
    void clear() {
        std::fill(data.begin(), data.end(), Pixel());
        std::cout << YELLOW << "Cleared texture" << RESET << std::endl;
    }
    
    void paint(int x, int y, const Color& color, float brushSize, float opacity = 1.0f) {
        int r = static_cast<int>(brushSize);
        
        for (int i = -r; i <= r; i++) {
            for (int j = -r; j <= r; j++) {
                int px = x + i;
                int py = y + j;
                
                // Simple distance check for circular brush
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    float distance = std::sqrt(i*i + j*j);
                    if (distance <= r) {
                        // Apply falloff for smoother brush
                        float falloff = 1.0f - (distance / r);
                        float pixelOpacity = opacity * falloff * falloff;
                        
                        // Apply the pixel if it's more opaque than what's there
                        // or if we're using the eraser (space symbol)
                        Pixel& currentPixel = data[py * width + px];
                        if (color.symbol == ' ' || currentPixel.opacity < pixelOpacity) {
                            currentPixel = Pixel(color, pixelOpacity);
                        }
                    }
                }
            }
        }
    }
    
    void render() const {
        // Top border with column numbers (tens)
        std::cout << "   ";
        for (int x = 0; x < width; x += 10) {
            std::cout << std::setw(10) << x;
        }
        std::cout << std::endl;
        
        // Column numbers (ones)
        std::cout << "   ";
        for (int x = 0; x < width; x++) {
            std::cout << x % 10;
        }
        std::cout << std::endl;
        
        // Draw top border
        std::cout << "  +" << std::string(width, '-') << "+" << std::endl;
        
        // Draw texture content
        for (int y = 0; y < height; y++) {
            // Row numbers
            std::cout << std::setw(2) << y << "|";
            
            for (int x = 0; x < width; x++) {
                const Pixel& pixel = data[y * width + x];
                if (pixel.opacity > 0.1f) {
                    std::cout << pixel.color.code << pixel.color.symbol << RESET;
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "|" << std::endl;
        }
        
        // Draw bottom border
        std::cout << "  +" << std::string(width, '-') << "+" << std::endl;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int width;
    int height;
    std::vector<Pixel> data;
};

class Layer {
public:
    Layer(int width, int height, const std::string& name = "Layer") 
        : name(name), visible(true), opacity(1.0f), texture(width, height) {
        std::cout << GREEN << "Created layer: " << BOLD << name << RESET << std::endl;
    }
    
    void clear() {
        texture.clear();
        std::cout << YELLOW << "Cleared layer: " << name << RESET << std::endl;
    }
    
    void paint(int x, int y, const Color& color, float brushSize, float opacity = 1.0f) {
        texture.paint(x, y, color, brushSize, opacity * this->opacity);
        std::cout << BLUE << "Painting at " 
                  << x << "," << y 
                  << " with " << color.name 
                  << " (size: " << brushSize << ")" 
                  << RESET << std::endl;
    }
    
    void render() const {
        if (visible) {
            std::cout << CYAN << "● " << BOLD << name << RESET << std::endl;
            texture.render();
        } else {
            std::cout << RED << "○ " << name << " (hidden)" << RESET << std::endl;
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
        std::cout << MAGENTA << "Model created" << RESET << std::endl;
    }
    
    bool loadModel(const std::string& path) {
        std::cout << MAGENTA << "Loading model from: " << path << RESET << std::endl;
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
        std::cout << YELLOW << "Project created" << RESET << std::endl;
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
        
        layers.push_back(new Layer(30, 15, layerName));
        currentLayerIndex = layers.size() - 1;
        
        return layers.back();
    }
    
    void renderLayers() const {
        std::cout << BOLD << CYAN << "=== CANVAS ===" << RESET << std::endl;
        for (size_t i = 0; i < layers.size(); i++) {
            if (i == currentLayerIndex) {
                std::cout << WHITE << BG_BLUE << "[SELECTED] " << RESET << " ";
            }
            if (layers[i]->isVisible()) {
                layers[i]->render();
                std::cout << std::endl;
            } else {
                std::cout << RED << "Layer " << i << ": " << layers[i]->getName() 
                          << " (hidden)" << RESET << std::endl;
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
    
    void toggleLayerVisibility(size_t index) {
        if (index < layers.size()) {
            layers[index]->setVisible(!layers[index]->isVisible());
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
    PaintTool(const std::string& name, const Color& color) 
        : name(name), size(2.0f), color(color) {
        std::cout << BLUE << "Created paint tool: " << BOLD << name << RESET << std::endl;
    }
    
    virtual void use(Layer* layer, int x, int y) {
        if (layer) {
            layer->paint(x, y, color, size);
        }
    }
    
    const std::string& getName() const { return name; }
    float getSize() const { return size; }
    void setSize(float size) { this->size = size; }
    Color getColor() const { return color; }
    void setColor(const Color& color) { this->color = color; }
    
protected:
    std::string name;
    float size;
    Color color;
};

// Tool implementations
class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", Color('*', RED, "Red Brush")) {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", Color(' ', RESET, "Eraser")) {}
};

class LineArtTool : public PaintTool {
public:
    LineArtTool() : PaintTool("LineArt", Color('#', BLUE, "Blue LineArt")) {}
};

class DotBrushTool : public PaintTool {
public:
    DotBrushTool() : PaintTool("DotBrush", Color('.', GREEN, "Green Dots")) {}
};

class StarBrushTool : public PaintTool {
public:
    StarBrushTool() : PaintTool("StarBrush", Color('*', YELLOW, "Yellow Stars")) {}
};

class Application {
public:
    Application() : currentTool(nullptr), running(true) {
        std::cout << BOLD << GREEN << "=== 3D Model Painter Initialized ===" << RESET << std::endl;
        
        // Initialize color palette
        initColorPalette();
        
        // Initialize tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        tools.push_back(new LineArtTool());
        tools.push_back(new DotBrushTool());
        tools.push_back(new StarBrushTool());
        
        // Set default tool
        currentTool = tools[0];
        
        // Load a sample model and add a default layer
        project.loadModel("cube.obj");
        project.addLayer("Background");
    }
    
    void initColorPalette() {
        colorPalette['1'] = Color('*', RED, "Red");
        colorPalette['2'] = Color('*', GREEN, "Green");
        colorPalette['3'] = Color('*', BLUE, "Blue");
        colorPalette['4'] = Color('*', YELLOW, "Yellow");
        colorPalette['5'] = Color('*', MAGENTA, "Magenta");
        colorPalette['6'] = Color('*', CYAN, "Cyan");
        colorPalette['7'] = Color('#', WHITE, "White");
        colorPalette['8'] = Color('.', GREEN, "Dots");
        colorPalette['9'] = Color('=', YELLOW, "Lines");
        colorPalette['0'] = Color(' ', RESET, "Eraser");
    }
    
    void showHelp() {
        std::cout << BOLD << CYAN << "\n===== 3D Model Painter Help =====" << RESET << std::endl;
        
        std::cout << BOLD << "\nTool Selection:" << RESET << std::endl;
        std::cout << "  " << YELLOW << "1-5" << RESET << "      - Select tools (Brush, Eraser, LineArt, DotBrush, StarBrush)" << std::endl;
        
        std::cout << BOLD << "\nBrush Settings:" << RESET << std::endl;
        std::cout << "  " << YELLOW << "+, -" << RESET << "     - Increase/decrease brush size" << std::endl;
        std::cout << "  " << YELLOW << "1-9, 0" << RESET << "   - Select color (0 = eraser)" << std::endl;
        
        std::cout << BOLD << "\nPainting Commands:" << RESET << std::endl;
        std::cout << "  " << YELLOW << "p x y" << RESET << "    - Paint at position (x,y)" << std::endl;
        std::cout << "  " << YELLOW << "c" << RESET << "        - Clear current layer" << std::endl;
        
        std::cout << BOLD << "\nLayer Management:" << RESET << std::endl;
        std::cout << "  " << YELLOW << "n [name]" << RESET << " - New layer with optional name" << std::endl;
        std::cout << "  " << YELLOW << "l" << RESET << "        - List layers" << std::endl;
        std::cout << "  " << YELLOW << "s index" << RESET << "  - Select layer by index" << std::endl;
        std::cout << "  " << YELLOW << "t index" << RESET << "  - Toggle layer visibility" << std::endl;
        
        std::cout << BOLD << "\nApplication Controls:" << RESET << std::endl;
        std::cout << "  " << YELLOW << "v" << RESET << "        - View canvas" << std::endl;
        std::cout << "  " << YELLOW << "h" << RESET << "        - Show this help" << std::endl;
        std::cout << "  " << YELLOW << "q" << RESET << "        - Quit" << std::endl;
        
        std::cout << BOLD << CYAN << "===============================" << RESET << std::endl;
    }
    
    void showStatus() {
        std::cout << BOLD << "Current Tool: " << BLUE << currentTool->getName() << RESET << std::endl;
        std::cout << "Brush Size: " << YELLOW << currentTool->getSize() << RESET << std::endl;
        std::cout << "Color: " << currentTool->getColor().code << currentTool->getColor().name 
                  << " " << currentTool->getColor().symbol << RESET << std::endl;
        
        if (project.getCurrentLayer()) {
            std::cout << "Layer: " << GREEN << project.getCurrentLayer()->getName() << RESET 
                      << " (Index: " << project.getCurrentLayerIndex() << ")" << std::endl;
        } else {
            std::cout << RED << "No active layer!" << RESET << std::endl;
        }
    }
    
    void run() {
        std::cout << BOLD << MAGENTA << "\n=== 3D Model Painter Started ===" << RESET << std::endl;
        showHelp();
        
        // Draw a sample
        auto* layer = project.getCurrentLayer();
        if (layer && currentTool) {
            currentTool->use(layer, 15, 7);
        }
        
        // Main application loop
        std::string command;
        while (running) {
            std::cout << std::endl;
            showStatus();
            std::cout << BOLD << CYAN << "\nCommand" << RESET << " (h for help): ";
            std::getline(std::cin, command);
            
            if (command.empty()) {
                continue;
            }
            
            char cmd = command[0];
            switch (cmd) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5': {
                    int toolIndex = cmd - '1';
                    if (toolIndex >= 0 && toolIndex < tools.size()) {
                        currentTool = tools[toolIndex];
                        std::cout << GREEN << "Selected tool: " << BOLD << currentTool->getName() << RESET << std::endl;
                    }
                    break;
                }
                // Colors are selectable with the number keys when combined with Alt or Ctrl
                case 'C':
                case 'c': {
                    if (command.length() > 1) {
                        char colorKey = command[1];
                        if (colorPalette.find(colorKey) != colorPalette.end()) {
                            currentTool->setColor(colorPalette[colorKey]);
                            std::cout << GREEN << "Selected color: " << currentTool->getColor().name << RESET << std::endl;
                        }
                    } else {
                        if (project.getCurrentLayer()) {
                            project.getCurrentLayer()->clear();
                            std::cout << YELLOW << "Layer cleared" << RESET << std::endl;
                        }
                    }
                    break;
                }
                case '+': {
                    if (currentTool) {
                        float newSize = currentTool->getSize() + 0.5f;
                        currentTool->setSize(newSize);
                        std::cout << GREEN << "Increased brush size to " << newSize << RESET << std::endl;
                    }
                    break;
                }
                case '-': {
                    if (currentTool) {
                        float newSize = std::max(0.5f, currentTool->getSize() - 0.5f);
                        currentTool->setSize(newSize);
                        std::cout << GREEN << "Decreased brush size to " << newSize << RESET << std::endl;
                    }
                    break;
                }
                case 'p':
                case 'P': {
                    int x, y;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d %d", &x, &y) == 2) {
                        if (currentTool && project.getCurrentLayer()) {
                            currentTool->use(project.getCurrentLayer(), x, y);
                            std::cout << CYAN << "Canvas after painting:" << RESET << std::endl;
                            project.getCurrentLayer()->render();
                        }
                    } else {
                        std::cout << RED << "Invalid paint command. Format: p x y" << RESET << std::endl;
                    }
                    break;
                }
                case 'n':
                case 'N': {
                    std::string name;
                    if (command.length() > 2) {
                        name = command.substr(2);
                    }
                    Layer* newLayer = project.addLayer(name);
                    std::cout << GREEN << "Created new layer: " << BOLD << newLayer->getName() << RESET << std::endl;
                    break;
                }
                case 'l':
                case 'L': {
                    std::cout << BOLD << CYAN << "\n=== Layers ===" << RESET << std::endl;
                    project.renderLayers();
                    break;
                }
                case 's':
                case 'S': {
                    int index;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d", &index) == 1) {
                        project.setCurrentLayerIndex(index);
                        std::cout << GREEN << "Selected layer " << index << RESET << std::endl;
                    } else {
                        std::cout << RED << "Invalid select command. Format: s index" << RESET << std::endl;
                    }
                    break;
                }
                case 't':
                case 'T': {
                    int index;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d", &index) == 1) {
                        project.toggleLayerVisibility(index);
                        std::cout << YELLOW << "Toggled visibility of layer " << index << RESET << std::endl;
                    } else {
                        std::cout << RED << "Invalid toggle command. Format: t index" << RESET << std::endl;
                    }
                    break;
                }
                case 'v':
                case 'V': {
                    std::cout << BOLD << CYAN << "\n=== Current Canvas ===" << RESET << std::endl;
                    project.renderLayers();
                    break;
                }
                case 'h':
                case 'H': {
                    showHelp();
                    break;
                }
                case 'q':
                case 'Q': {
                    running = false;
                    std::cout << BOLD << RED << "Quitting application..." << RESET << std::endl;
                    break;
                }
                default: {
                    std::cout << RED << "Unknown command: " << cmd << ". Type 'h' for help." << RESET << std::endl;
                    break;
                }
            }
        }
    }
    
    // Function to run a demo without user input (for automated testing/workflow)
    void runDemo() {
        std::cout << BOLD << MAGENTA << "\n=== 3D Model Painter Demo ===" << RESET << std::endl;
        
        // Create some layers
        project.addLayer("Details Layer");
        
        // Draw on first layer
        auto* baseLayer = project.getCurrentLayer();
        project.setCurrentLayerIndex(0);  // Select base layer
        
        // Use different colors and tools for a more interesting demo
        
        // Draw background with large cyan brush
        currentTool = tools[0]; // Brush
        currentTool->setColor(Color('*', CYAN, "Cyan"));
        currentTool->setSize(5.0f);
        for (int i = 5; i < 25; i += 3) {
            for (int j = 3; j < 12; j += 2) {
                currentTool->use(baseLayer, i, j);
            }
        }
        
        // Draw a red heart
        currentTool->setColor(Color('<', RED, "Red Heart"));
        currentTool->setSize(1.0f);
        currentTool->use(baseLayer, 15, 7);
        
        // Draw on second layer
        project.setCurrentLayerIndex(1);  // Select details layer
        auto* detailsLayer = project.getCurrentLayer();
        
        // Use line art tool for frame
        currentTool = tools[2];  // LineArt
        currentTool->setSize(1.0f);
        
        // Draw a border
        for (int i = 5; i < 25; i++) {
            currentTool->use(detailsLayer, i, 2);
            currentTool->use(detailsLayer, i, 12);
        }
        for (int j = 2; j <= 12; j++) {
            currentTool->use(detailsLayer, 5, j);
            currentTool->use(detailsLayer, 24, j);
        }
        
        // Use star brush for accents
        currentTool = tools[4];  // StarBrush
        currentTool->setSize(1.0f);
        
        // Add some stars
        currentTool->use(detailsLayer, 7, 4);
        currentTool->use(detailsLayer, 22, 4);
        currentTool->use(detailsLayer, 7, 10);
        currentTool->use(detailsLayer, 22, 10);
        
        // Show the results
        std::cout << BOLD << CYAN << "\n=== Final Canvas Result ===" << RESET << std::endl;
        project.renderLayers();
        
        std::cout << BOLD << GREEN << "\n3D Model Painter Demo completed successfully!" << RESET << std::endl;
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
    std::map<char, Color> colorPalette;
};

int main(int argc, char* argv[]) {
    try {
        Application app;
        
        if (argc > 1 && std::string(argv[1]) == "--demo") {
            app.runDemo();
        } else {
            app.run();
        }
        
        std::cout << BOLD << GREEN << "Application ran successfully!" << RESET << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << BOLD << RED << "Error: " << e.what() << RESET << std::endl;
        return 1;
    }
}