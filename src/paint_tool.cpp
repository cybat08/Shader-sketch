#include "paint_tool.h"
#include "utils.h"
#include <glm/gtx/vector_angle.hpp>

// Base PaintTool implementation
PaintTool::PaintTool(const std::string& name, const std::string& iconName)
    : name(name), iconName(iconName), currentLayer(nullptr), painting(false),
      size(10.0f), hardness(0.5f), color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) {
}

PaintTool::~PaintTool() {
}

void PaintTool::interpolateStroke(const glm::vec3& p1, const glm::vec3& p2) {
    if (!currentLayer || !painting) {
        return;
    }
    
    // Calculate distance between points
    float distance = glm::length(p2 - p1);
    
    // If points are too close, just paint at the current point
    if (distance < 0.01f) {
        glm::vec2 texCoord = Utils::worldToTextureCoord(p2, currentLayer->getWidth(), currentLayer->getHeight());
        currentLayer->paint(texCoord.x, texCoord.y, color, size, hardness);
        return;
    }
    
    // Interpolate points for smooth brush stroke
    int steps = static_cast<int>(distance * 10.0f); // Adjust multiplier as needed
    steps = std::max(1, std::min(steps, 100)); // Reasonable limits
    
    for (int i = 0; i <= steps; i++) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        glm::vec3 p = glm::mix(p1, p2, t);
        glm::vec2 texCoord = Utils::worldToTextureCoord(p, currentLayer->getWidth(), currentLayer->getHeight());
        currentLayer->paint(texCoord.x, texCoord.y, color, size, hardness);
    }
}

// BrushTool implementation
BrushTool::BrushTool() 
    : PaintTool("Brush", "brush") {
}

void BrushTool::begin(Layer* layer, const glm::vec3& position) {
    if (!layer) {
        return;
    }
    
    currentLayer = layer;
    lastPosition = position;
    painting = true;
    
    // Paint at starting position
    glm::vec2 texCoord = Utils::worldToTextureCoord(position, layer->getWidth(), layer->getHeight());
    layer->paint(texCoord.x, texCoord.y, color, size, hardness);
}

void BrushTool::update(const glm::vec3& position) {
    if (!currentLayer || !painting) {
        return;
    }
    
    // Interpolate between last position and current position
    interpolateStroke(lastPosition, position);
    
    // Update last position
    lastPosition = position;
}

void BrushTool::end() {
    painting = false;
    currentLayer = nullptr;
}

// EraserTool implementation
EraserTool::EraserTool() 
    : PaintTool("Eraser", "eraser") {
}

void EraserTool::begin(Layer* layer, const glm::vec3& position) {
    if (!layer) {
        return;
    }
    
    currentLayer = layer;
    lastPosition = position;
    painting = true;
    
    // Erase at starting position
    glm::vec2 texCoord = Utils::worldToTextureCoord(position, layer->getWidth(), layer->getHeight());
    layer->erase(texCoord.x, texCoord.y, size, hardness);
}

void EraserTool::update(const glm::vec3& position) {
    if (!currentLayer || !painting) {
        return;
    }
    
    // Calculate texture coordinates
    glm::vec2 texCoord = Utils::worldToTextureCoord(position, currentLayer->getWidth(), currentLayer->getHeight());
    
    // Interpolate between last position and current position
    glm::vec2 lastTexCoord = Utils::worldToTextureCoord(lastPosition, currentLayer->getWidth(), currentLayer->getHeight());
    
    // Step points between lastPos and pos
    float dist = glm::distance(lastTexCoord, texCoord);
    int steps = static_cast<int>(dist / (size * 0.5f)) + 1;
    
    for (int i = 0; i <= steps; i++) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        glm::vec2 interpolatedCoord = glm::mix(lastTexCoord, texCoord, t);
        currentLayer->erase(interpolatedCoord.x, interpolatedCoord.y, size, hardness);
    }
    
    // Update last position
    lastPosition = position;
}

void EraserTool::end() {
    painting = false;
    currentLayer = nullptr;
}

// FillTool implementation
FillTool::FillTool() 
    : PaintTool("Fill", "fill"), tolerance(0.1f) {
}

void FillTool::begin(Layer* layer, const glm::vec3& position) {
    if (!layer) {
        return;
    }
    
    currentLayer = layer;
    
    // Fill at position
    glm::vec2 texCoord = Utils::worldToTextureCoord(position, layer->getWidth(), layer->getHeight());
    layer->fill(texCoord.x, texCoord.y, color, tolerance);
}

void FillTool::update(const glm::vec3& position) {
    // Fill tool doesn't have any update behavior
}

void FillTool::end() {
    currentLayer = nullptr;
}
