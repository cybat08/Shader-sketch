#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <SFML/Graphics.hpp>

// Simple 3D Model Painter application with SFML GUI
// This is a minimal version to demonstrate the core architecture

class Texture {
public:
    Texture(int width, int height) : width(width), height(height) {
        // Create an SFML texture
        sfTexture.create(width, height);
        // Fill with white initially
        sf::Image img;
        img.create(width, height, sf::Color::White);
        sfTexture.update(img);
        std::cout << "Created texture: " << width << "x" << height << std::endl;
    }
    
    void clear() {
        sf::Image img;
        img.create(width, height, sf::Color::White);
        sfTexture.update(img);
        std::cout << "Cleared texture" << std::endl;
    }
    
    void paint(int x, int y, float r, float g, float b, float a, float radius) {
        // Get the image from the texture
        sf::Image img = sfTexture.copyToImage();
        
        // Apply a circular brush
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                float dist = std::sqrt(i*i + j*j);
                if (dist <= radius) {
                    float alpha = a * (1.0f - dist/radius);
                    int px = x + i;
                    int py = y + j;
                    
                    if (px >= 0 && px < width && py >= 0 && py < height) {
                        sf::Color currentColor = img.getPixel(px, py);
                        sf::Color newColor(
                            r * 255,
                            g * 255,
                            b * 255,
                            alpha * 255
                        );
                        
                        // Blend colors
                        float blendFactor = alpha;
                        sf::Color blendedColor(
                            currentColor.r * (1-blendFactor) + newColor.r * blendFactor,
                            currentColor.g * (1-blendFactor) + newColor.g * blendFactor,
                            currentColor.b * (1-blendFactor) + newColor.b * blendFactor,
                            255
                        );
                        
                        img.setPixel(px, py, blendedColor);
                    }
                }
            }
        }
        
        // Update the texture with the modified image
        sfTexture.update(img);
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const sf::Texture& getSFMLTexture() const { return sfTexture; }
    
private:
    int width;
    int height;
    sf::Texture sfTexture;
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
    
    void paint(int x, int y, float r, float g, float b, float a, float radius) {
        texture.paint(x, y, r, g, b, a, radius);
        std::cout << "Painting at " << x << "," << y << " with radius " << radius << std::endl;
    }
    
    const std::string& getName() const { return name; }
    bool isVisible() const { return visible; }
    float getOpacity() const { return opacity; }
    void setVisible(bool visible) { this->visible = visible; }
    void setOpacity(float opacity) { this->opacity = opacity; }
    const Texture& getTexture() const { return texture; }
    
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
        
        // Create a simple quad model
        vertices.setPrimitiveType(sf::Quads);
        vertices.resize(4);
        
        // Set position
        vertices[0].position = sf::Vector2f(100, 100);
        vertices[1].position = sf::Vector2f(300, 100);
        vertices[2].position = sf::Vector2f(300, 300);
        vertices[3].position = sf::Vector2f(100, 300);
        
        // Set texture coordinates
        vertices[0].texCoords = sf::Vector2f(0, 0);
        vertices[1].texCoords = sf::Vector2f(1, 0);
        vertices[2].texCoords = sf::Vector2f(1, 1);
        vertices[3].texCoords = sf::Vector2f(0, 1);
    }
    
    bool loadModel(const std::string& path) {
        std::cout << "Loading model from: " << path << std::endl;
        loaded = true;
        return true;
    }
    
    bool isLoaded() const { return loaded; }
    
    void draw(sf::RenderWindow& window, const sf::Texture& texture) {
        sf::Sprite sprite(texture);
        sprite.setPosition(100, 100);
        window.draw(sprite);
    }
    
private:
    bool loaded;
    sf::VertexArray vertices;
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
        
        layers.push_back(new Layer(512, 512, layerName));
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
    Model& getModel() { return model; }
    const std::vector<Layer*>& getLayers() const { return layers; }
    
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
    PaintTool(const std::string& name) : name(name), size(20.0f), 
        r(1.0f), g(0.0f), b(0.0f), a(1.0f) {
        std::cout << "Created paint tool: " << name << std::endl;
    }
    
    virtual void use(Layer* layer, int x, int y) {
        if (layer) {
            layer->paint(x, y, r, g, b, a, size);
        }
    }
    
    const std::string& getName() const { return name; }
    float getSize() const { return size; }
    void setSize(float size) { this->size = size; }
    
    void setColor(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    
protected:
    std::string name;
    float size;
    float r, g, b, a;
};

class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush") {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser") {
        // Set to white for eraser
        r = g = b = 1.0f;
    }
};

class Application {
public:
    Application() : currentTool(nullptr), window(sf::VideoMode(800, 600), "3D Model Painter") {
        std::cout << "Application initialized" << std::endl;
        
        // Initialize tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        
        // Set default tool
        currentTool = tools[0];
        
        // Initialize UI
        initUI();
        
        // Add a default layer
        project.loadModel("cube.obj");
        project.addLayer("Base Layer");
    }
    
    void initUI() {
        // Load font
        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        
        // Tool selection text
        toolText.setFont(font);
        toolText.setCharacterSize(18);
        toolText.setFillColor(sf::Color::Black);
        toolText.setPosition(10, 520);
        updateToolText();
        
        // Brush size text
        brushSizeText.setFont(font);
        brushSizeText.setCharacterSize(18);
        brushSizeText.setFillColor(sf::Color::Black);
        brushSizeText.setPosition(10, 550);
        updateBrushSizeText();
        
        // Color swatches
        colorSwatches.resize(6);
        float swatchSize = 30.0f;
        float startX = 350.0f;
        
        // Red
        colorSwatches[0] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[0].setPosition(startX, 520);
        colorSwatches[0].setFillColor(sf::Color::Red);
        
        // Green
        colorSwatches[1] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[1].setPosition(startX + swatchSize + 5, 520);
        colorSwatches[1].setFillColor(sf::Color::Green);
        
        // Blue
        colorSwatches[2] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[2].setPosition(startX + 2 * (swatchSize + 5), 520);
        colorSwatches[2].setFillColor(sf::Color::Blue);
        
        // Yellow
        colorSwatches[3] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[3].setPosition(startX, 520 + swatchSize + 5);
        colorSwatches[3].setFillColor(sf::Color::Yellow);
        
        // Cyan
        colorSwatches[4] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[4].setPosition(startX + swatchSize + 5, 520 + swatchSize + 5);
        colorSwatches[4].setFillColor(sf::Color::Cyan);
        
        // Magenta
        colorSwatches[5] = sf::RectangleShape(sf::Vector2f(swatchSize, swatchSize));
        colorSwatches[5].setPosition(startX + 2 * (swatchSize + 5), 520 + swatchSize + 5);
        colorSwatches[5].setFillColor(sf::Color::Magenta);
    }
    
    void updateToolText() {
        toolText.setString("Tool: " + currentTool->getName());
    }
    
    void updateBrushSizeText() {
        brushSizeText.setString("Size: " + std::to_string(static_cast<int>(currentTool->getSize())));
    }
    
    void run() {
        std::cout << "Running 3D Model Painter" << std::endl;
        
        // Main application loop
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    handleMouseDrag(event.mouseMove.x, event.mouseMove.y);
                }
            }
        }
    }
    
    void handleKeyPress(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Num1 || key == sf::Keyboard::B) {
            // Switch to brush
            currentTool = tools[0];
            updateToolText();
        }
        else if (key == sf::Keyboard::Num2 || key == sf::Keyboard::E) {
            // Switch to eraser
            currentTool = tools[1];
            updateToolText();
        }
        else if (key == sf::Keyboard::Up) {
            // Increase brush size
            currentTool->setSize(std::min(50.0f, currentTool->getSize() + 5.0f));
            updateBrushSizeText();
        }
        else if (key == sf::Keyboard::Down) {
            // Decrease brush size
            currentTool->setSize(std::max(5.0f, currentTool->getSize() - 5.0f));
            updateBrushSizeText();
        }
        else if (key == sf::Keyboard::C) {
            // Clear current layer
            if (auto* layer = project.getCurrentLayer()) {
                layer->clear();
            }
        }
    }
    
    void handleMouseClick(int x, int y) {
        // Check if click was on the canvas area (model)
        if (x >= 100 && x <= 400 && y >= 100 && y <= 400) {
            // Apply tool
            if (currentTool && project.getCurrentLayer()) {
                currentTool->use(project.getCurrentLayer(), x - 100, y - 100);
            }
        }
        // Check if click was on a color swatch
        else {
            for (size_t i = 0; i < colorSwatches.size(); i++) {
                if (colorSwatches[i].getGlobalBounds().contains(x, y)) {
                    // Set color based on swatch
                    sf::Color swatchColor = colorSwatches[i].getFillColor();
                    if (currentTool && currentTool->getName() != "Eraser") {
                        currentTool->setColor(
                            swatchColor.r / 255.0f,
                            swatchColor.g / 255.0f,
                            swatchColor.b / 255.0f,
                            1.0f
                        );
                    }
                    break;
                }
            }
        }
    }
    
    void handleMouseDrag(int x, int y) {
        // Check if drag was on the canvas area (model)
        if (x >= 100 && x <= 400 && y >= 100 && y <= 400) {
            // Apply tool
            if (currentTool && project.getCurrentLayer()) {
                currentTool->use(project.getCurrentLayer(), x - 100, y - 100);
            }
        }
    }
    
    void render() {
        window.clear(sf::Color(240, 240, 240));
        
        // Draw the model with layered textures
        if (project.hasModel()) {
            // Start with a white background
            sf::RectangleShape background(sf::Vector2f(300, 300));
            background.setPosition(100, 100);
            background.setFillColor(sf::Color::White);
            window.draw(background);
            
            // Draw each visible layer
            for (const auto* layer : project.getLayers()) {
                if (layer->isVisible()) {
                    project.getModel().draw(window, layer->getTexture().getSFMLTexture());
                }
            }
            
            // Draw a frame around the canvas
            sf::RectangleShape frame(sf::Vector2f(300, 300));
            frame.setPosition(100, 100);
            frame.setFillColor(sf::Color::Transparent);
            frame.setOutlineColor(sf::Color::Black);
            frame.setOutlineThickness(2);
            window.draw(frame);
        }
        
        // Draw UI elements
        window.draw(toolText);
        window.draw(brushSizeText);
        
        // Draw color swatches
        for (const auto& swatch : colorSwatches) {
            window.draw(swatch);
        }
        
        window.display();
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
    sf::RenderWindow window;
    
    // UI elements
    sf::Font font;
    sf::Text toolText;
    sf::Text brushSizeText;
    std::vector<sf::RectangleShape> colorSwatches;
};

int main() {
    try {
        Application app;
        app.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}