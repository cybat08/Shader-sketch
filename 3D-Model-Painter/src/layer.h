#pragma once

#include "texture.h"
#include <string>
#include <memory>

class Layer {
public:
    // Create a new layer with the given dimensions
    Layer(int width, int height, const std::string& name = "Layer");
    
    // Load layer from texture file
    Layer(const std::string& path, const std::string& name = "Layer");
    
    // Destructor
    ~Layer();
    
    // Getters
    const std::string& getName() const { return name; }
    bool isVisible() const { return visible; }
    float getOpacity() const { return opacity; }
    unsigned int getTextureID() const { return texture->getID(); }
    int getWidth() const { return texture->getWidth(); }
    int getHeight() const { return texture->getHeight(); }
    
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setVisible(bool visible) { this->visible = visible; }
    void setOpacity(float opacity) { this->opacity = std::max(0.0f, std::min(1.0f, opacity)); }
    
    // Clear layer with color
    void clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    
    // Paint on layer
    void paint(int x, int y, const glm::vec4& color, float radius, float hardness);
    
    // Fill area on layer
    void fill(int x, int y, const glm::vec4& color, float tolerance = 0.1f);
    
    // Erase on layer
    void erase(int x, int y, float radius, float hardness);
    
    // Resize layer
    void resize(int width, int height);
    
    // Save layer to file
    bool saveToFile(const std::string& path) const;
    
    // Get texture
    Texture* getTexture() { return texture.get(); }
    const Texture* getTexture() const { return texture.get(); }
    
private:
    std::string name;
    bool visible;
    float opacity;
    std::unique_ptr<Texture> texture;
};
