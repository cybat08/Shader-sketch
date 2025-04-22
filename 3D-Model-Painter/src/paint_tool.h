#pragma once

#include "layer.h"
#include <string>
#include <glm/glm.hpp>

class PaintTool {
public:
    PaintTool(const std::string& name, const std::string& iconName);
    virtual ~PaintTool();
    
    // Begin painting
    virtual void begin(Layer* layer, const glm::vec3& position) = 0;
    
    // Update painting
    virtual void update(const glm::vec3& position) = 0;
    
    // End painting
    virtual void end() = 0;
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getIconName() const { return iconName; }
    
    // Get/set parameters
    virtual float getSize() const { return size; }
    virtual void setSize(float size) { this->size = size; }
    
    virtual float getHardness() const { return hardness; }
    virtual void setHardness(float hardness) { this->hardness = hardness; }
    
    virtual glm::vec4 getColor() const { return color; }
    virtual void setColor(const glm::vec4& color) { this->color = color; }
    
protected:
    std::string name;
    std::string iconName;
    Layer* currentLayer;
    glm::vec3 lastPosition;
    bool painting;
    
    // Tool parameters
    float size;
    float hardness;
    glm::vec4 color;
    
    // Helper for interpolating brush strokes
    void interpolateStroke(const glm::vec3& p1, const glm::vec3& p2);
};

// Brush tool
class BrushTool : public PaintTool {
public:
    BrushTool();
    
    void begin(Layer* layer, const glm::vec3& position) override;
    void update(const glm::vec3& position) override;
    void end() override;
};

// Eraser tool
class EraserTool : public PaintTool {
public:
    EraserTool();
    
    void begin(Layer* layer, const glm::vec3& position) override;
    void update(const glm::vec3& position) override;
    void end() override;
};

// Fill tool
class FillTool : public PaintTool {
public:
    FillTool();
    
    void begin(Layer* layer, const glm::vec3& position) override;
    void update(const glm::vec3& position) override;
    void end() override;
    
    float getTolerance() const { return tolerance; }
    void setTolerance(float tolerance) { this->tolerance = tolerance; }
    
private:
    float tolerance;
};
