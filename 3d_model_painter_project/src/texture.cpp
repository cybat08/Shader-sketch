#include "texture.h"
#include <glad/glad.h>
#include <iostream>
#include <cmath>
#include <queue>
#include <stb_image.h>
#include <stb_image_write.h>

Texture::Texture(int width, int height) 
    : width(width), height(height), channels(4) {
    
    // Create empty data array
    data.resize(width * height * channels, 0);
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Create texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& path) : textureID(0) {
    // Load image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imgData = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!imgData) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        width = 1;
        height = 1;
        channels = 4;
        data.resize(4, 255); // White pixel
    } else {
        // Copy data to our vector
        data.assign(imgData, imgData + width * height * channels);
        stbi_image_free(imgData);
    }
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clear(const glm::vec4& color) {
    // Convert color to bytes
    unsigned char r = static_cast<unsigned char>(color.r * 255.0f);
    unsigned char g = static_cast<unsigned char>(color.g * 255.0f);
    unsigned char b = static_cast<unsigned char>(color.b * 255.0f);
    unsigned char a = static_cast<unsigned char>(color.a * 255.0f);
    
    // Fill data array
    for (int i = 0; i < width * height; i++) {
        int baseIndex = i * channels;
        
        if (channels >= 1) data[baseIndex + 0] = r;
        if (channels >= 2) data[baseIndex + 1] = g;
        if (channels >= 3) data[baseIndex + 2] = b;
        if (channels >= 4) data[baseIndex + 3] = a;
    }
    
    // Update texture
    updateTexture();
}

void Texture::setPixel(int x, int y, const glm::vec4& color) {
    if (!isValidCoordinate(x, y)) {
        return;
    }
    
    // Convert color to bytes
    unsigned char r = static_cast<unsigned char>(color.r * 255.0f);
    unsigned char g = static_cast<unsigned char>(color.g * 255.0f);
    unsigned char b = static_cast<unsigned char>(color.b * 255.0f);
    unsigned char a = static_cast<unsigned char>(color.a * 255.0f);
    
    // Calculate base index
    int baseIndex = (y * width + x) * channels;
    
    // Set pixel data
    if (channels >= 1) data[baseIndex + 0] = r;
    if (channels >= 2) data[baseIndex + 1] = g;
    if (channels >= 3) data[baseIndex + 2] = b;
    if (channels >= 4) data[baseIndex + 3] = a;
}

void Texture::applyBrush(int x, int y, const glm::vec4& color, float radius, float hardness) {
    // Determine the rectangular region to update
    int minX = std::max(0, static_cast<int>(x - radius));
    int maxX = std::min(width - 1, static_cast<int>(x + radius));
    int minY = std::max(0, static_cast<int>(y - radius));
    int maxY = std::min(height - 1, static_cast<int>(y + radius));
    
    float radiusSquared = radius * radius;
    
    // Update pixels within the brush area
    for (int py = minY; py <= maxY; py++) {
        for (int px = minX; px <= maxX; px++) {
            // Calculate distance from brush center
            float distX = px - x;
            float distY = py - y;
            float distSquared = distX * distX + distY * distY;
            
            if (distSquared <= radiusSquared) {
                // Calculate brush intensity based on distance and hardness
                float t = distSquared / radiusSquared;
                float intensity;
                
                if (hardness >= 1.0f) {
                    // Hard brush (constant intensity)
                    intensity = 1.0f;
                } else {
                    // Soft brush (falloff at edges)
                    intensity = 1.0f - std::pow(t, (1.0f - hardness) * 2.0f);
                }
                
                // Get current pixel color
                glm::vec4 currentColor = getPixel(px, py);
                
                // Blend colors based on intensity and alpha
                glm::vec4 blendedColor;
                blendedColor.r = color.r * color.a * intensity + currentColor.r * currentColor.a * (1.0f - color.a * intensity);
                blendedColor.g = color.g * color.a * intensity + currentColor.g * currentColor.a * (1.0f - color.a * intensity);
                blendedColor.b = color.b * color.a * intensity + currentColor.b * currentColor.a * (1.0f - color.a * intensity);
                blendedColor.a = color.a * intensity + currentColor.a * (1.0f - color.a * intensity);
                
                // Set pixel
                setPixel(px, py, blendedColor);
            }
        }
    }
    
    // Update texture
    updateTexture();
}

void Texture::fill(int x, int y, const glm::vec4& color, float tolerance) {
    if (!isValidCoordinate(x, y)) {
        return;
    }
    
    // Get target color to replace
    glm::vec4 targetColor = getPixel(x, y);
    
    // If target color is already the fill color, return
    if (glm::length(targetColor - color) < 0.01f) {
        return;
    }
    
    // Flood fill algorithm using BFS
    std::vector<bool> visited(width * height, false);
    std::queue<std::pair<int, int>> queue;
    
    // Start with the seed pixel
    queue.push(std::make_pair(x, y));
    visited[y * width + x] = true;
    
    // Direction vectors for 4-connected flood fill
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    while (!queue.empty()) {
        // Get current pixel
        std::pair<int, int> curr = queue.front();
        queue.pop();
        
        int cx = curr.first;
        int cy = curr.second;
        
        // Set pixel to fill color
        setPixel(cx, cy, color);
        
        // Check neighboring pixels
        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            
            if (isValidCoordinate(nx, ny) && !visited[ny * width + nx]) {
                glm::vec4 neighborColor = getPixel(nx, ny);
                
                // Check if neighbor color is similar to target color
                float colorDiff = glm::length(neighborColor - targetColor);
                if (colorDiff <= tolerance) {
                    visited[ny * width + nx] = true;
                    queue.push(std::make_pair(nx, ny));
                }
            }
        }
    }
    
    // Update texture
    updateTexture();
}

bool Texture::saveToFile(const std::string& path) const {
    // Determine format from file extension
    std::string extension = path.substr(path.find_last_of('.') + 1);
    
    int result = 0;
    if (extension == "png") {
        result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
    } else if (extension == "jpg" || extension == "jpeg") {
        result = stbi_write_jpg(path.c_str(), width, height, channels, data.data(), 90); // 90% quality
    } else if (extension == "bmp") {
        result = stbi_write_bmp(path.c_str(), width, height, channels, data.data());
    } else if (extension == "tga") {
        result = stbi_write_tga(path.c_str(), width, height, channels, data.data());
    } else {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return false;
    }
    
    return result != 0;
}

void Texture::updateTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data.data());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec4 Texture::getPixel(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        return glm::vec4(0.0f);
    }
    
    int baseIndex = (y * width + x) * channels;
    
    glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Get pixel data
    if (channels >= 1) color.r = data[baseIndex + 0] / 255.0f;
    if (channels >= 2) color.g = data[baseIndex + 1] / 255.0f;
    if (channels >= 3) color.b = data[baseIndex + 2] / 255.0f;
    if (channels >= 4) color.a = data[baseIndex + 3] / 255.0f;
    
    return color;
}

bool Texture::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}
