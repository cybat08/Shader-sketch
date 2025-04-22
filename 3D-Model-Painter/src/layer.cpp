#include "layer.h"

Layer::Layer(int width, int height, const std::string& name)
    : name(name), visible(true), opacity(1.0f) {
    texture = std::make_unique<Texture>(width, height);
    clear();
}

Layer::Layer(const std::string& path, const std::string& name)
    : name(name), visible(true), opacity(1.0f) {
    texture = std::make_unique<Texture>(path);
}

Layer::~Layer() {
    // Texture is cleaned up by its destructor
}

void Layer::clear(const glm::vec4& color) {
    texture->clear(color);
}

void Layer::paint(int x, int y, const glm::vec4& color, float radius, float hardness) {
    texture->applyBrush(x, y, color, radius, hardness);
}

void Layer::fill(int x, int y, const glm::vec4& color, float tolerance) {
    texture->fill(x, y, color, tolerance);
}

void Layer::erase(int x, int y, float radius, float hardness) {
    // Erasing is just painting with transparent color
    glm::vec4 transparent(0.0f, 0.0f, 0.0f, 0.0f);
    texture->applyBrush(x, y, transparent, radius, hardness);
}

void Layer::resize(int width, int height) {
    // Create new texture with desired dimensions
    std::unique_ptr<Texture> newTexture = std::make_unique<Texture>(width, height);
    
    // Clear with transparent
    newTexture->clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    
    // Copy old texture content
    int copyWidth = std::min(width, texture->getWidth());
    int copyHeight = std::min(height, texture->getHeight());
    
    for (int y = 0; y < copyHeight; y++) {
        for (int x = 0; x < copyWidth; x++) {
            glm::vec4 pixelColor = texture->getPixel(x, y);
            newTexture->setPixel(x, y, pixelColor);
        }
    }
    
    // Update texture
    newTexture->updateTexture();
    
    // Replace old texture
    texture = std::move(newTexture);
}

bool Layer::saveToFile(const std::string& path) const {
    return texture->saveToFile(path);
}
