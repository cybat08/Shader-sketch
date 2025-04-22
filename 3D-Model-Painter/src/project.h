#pragma once

#include "model.h"
#include "layer.h"
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

class Project {
public:
    Project();
    ~Project();
    
    // Model operations
    bool loadModel(const std::string& path);
    bool exportModel(const std::string& path) const;
    
    // Layer operations
    Layer* addLayer(const std::string& name = "");
    bool removeLayer(size_t index);
    Layer* getLayer(size_t index);
    Layer* getCurrentLayer();
    void setCurrentLayerIndex(size_t index);
    
    // Project operations
    bool saveProject(const std::string& path) const;
    bool loadProject(const std::string& path);
    void clear();
    
    // Getters
    const Model& getModel() const { return model; }
    const std::vector<std::unique_ptr<Layer>>& getLayers() const { return layers; }
    size_t getCurrentLayerIndex() const { return currentLayerIndex; }
    bool hasModel() const { return model.isLoaded(); }
    
private:
    Model model;
    std::vector<std::unique_ptr<Layer>> layers;
    size_t currentLayerIndex;
    int textureWidth;
    int textureHeight;
    
    // Helper to create a default texture size based on model
    void setDefaultTextureSize();
};
