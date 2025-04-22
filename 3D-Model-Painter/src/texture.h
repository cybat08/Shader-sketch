#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Texture {
public:
    // Create empty texture with specified dimensions
    Texture(int width, int height);
    
    // Load texture from file
    Texture(const std::string& path);
    
    // Destructor
    ~Texture();
    
    // Bind texture to specified texture unit
    void bind(unsigned int unit = 0) const;
    
    // Clear texture with color
    void clear(const glm::vec4& color);
    
    // Set pixel at coordinates
    void setPixel(int x, int y, const glm::vec4& color);
    
    // Apply a brush stamp at the specified position with a given color and radius
    void applyBrush(int x, int y, const glm::vec4& color, float radius, float hardness);
    
    // Fill area starting from specified pixel with a color
    void fill(int x, int y, const glm::vec4& color, float tolerance = 0.1f);
    
    // Getters
    unsigned int getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::vector<unsigned char>& getData() const { return data; }
    
    // Save texture to file
    bool saveToFile(const std::string& path) const;
    
private:
    unsigned int textureID;
    int width;
    int height;
    int channels;
    std::vector<unsigned char> data;
    
    // Update texture data in GPU memory
    void updateTexture();
    
    // Get pixel color at coordinates
    glm::vec4 getPixel(int x, int y) const;
    
    // Check if coordinates are within texture boundaries
    bool isValidCoordinate(int x, int y) const;
};
