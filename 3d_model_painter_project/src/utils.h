#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Utils {
    // Convert a world-space position to texture coordinates
    glm::vec2 worldToTextureCoord(const glm::vec3& worldPos, int textureWidth, int textureHeight);
    
    // Convert texture coordinates to world-space position
    glm::vec3 textureCoordToWorld(const glm::vec2& texCoord, const glm::vec3& worldPos, const glm::vec3& normal);
    
    // Check if file exists
    bool fileExists(const std::string& path);
    
    // Get file extension
    std::string getFileExtension(const std::string& path);
}
