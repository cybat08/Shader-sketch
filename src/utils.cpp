#include "utils.h"
#include <filesystem>

namespace Utils {
    glm::vec2 worldToTextureCoord(const glm::vec3& worldPos, int textureWidth, int textureHeight) {
        // This is a simplistic mapping, in a real application you would use the model's UV coordinates
        // For now, we'll just use the X,Y coordinates and normalize them
        float u = (worldPos.x + 1.0f) * 0.5f;
        float v = (worldPos.y + 1.0f) * 0.5f;
        
        // Convert to pixel coordinates
        int x = static_cast<int>(u * textureWidth);
        int y = static_cast<int>(v * textureHeight);
        
        // Clamp to texture dimensions
        x = std::max(0, std::min(x, textureWidth - 1));
        y = std::max(0, std::min(y, textureHeight - 1));
        
        return glm::vec2(x, y);
    }
    
    glm::vec3 textureCoordToWorld(const glm::vec2& texCoord, const glm::vec3& worldPos, const glm::vec3& normal) {
        // This is the inverse of worldToTextureCoord, but we'll need the original position and normal to reconstruct the 3D position
        // Again, this is a simplistic approach, in a real application you would use the model's UV mapping
        return worldPos; // Return the original position for now
    }
    
    bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    
    std::string getFileExtension(const std::string& path) {
        std::filesystem::path p(path);
        return p.extension().string();
    }
}
