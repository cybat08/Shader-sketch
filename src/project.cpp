#include "project.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

Project::Project() 
    : currentLayerIndex(0), textureWidth(1024), textureHeight(1024) {
}

Project::~Project() {
    // Layers are cleaned up by their destructors
}

bool Project::loadModel(const std::string& path) {
    // Clear existing project
    clear();
    
    // Load model
    if (!model.loadModel(path)) {
        return false;
    }
    
    // Set default texture size
    setDefaultTextureSize();
    
    // Add a default layer
    addLayer("Base Layer");
    
    return true;
}

bool Project::exportModel(const std::string& path) const {
    if (!model.isLoaded()) {
        std::cerr << "No model loaded to export." << std::endl;
        return false;
    }
    
    return model.exportModel(path);
}

Layer* Project::addLayer(const std::string& name) {
    static int layerCounter = 1;
    std::string layerName = name;
    
    if (layerName.empty()) {
        layerName = "Layer " + std::to_string(layerCounter++);
    }
    
    // Add layer
    layers.push_back(std::make_unique<Layer>(textureWidth, textureHeight, layerName));
    
    // Set current layer to the new one
    currentLayerIndex = layers.size() - 1;
    
    return layers.back().get();
}

bool Project::removeLayer(size_t index) {
    if (index >= layers.size()) {
        return false;
    }
    
    // Remove layer
    layers.erase(layers.begin() + index);
    
    // Update current layer index
    if (currentLayerIndex >= layers.size()) {
        currentLayerIndex = layers.empty() ? 0 : layers.size() - 1;
    }
    
    return true;
}

Layer* Project::getLayer(size_t index) {
    if (index >= layers.size()) {
        return nullptr;
    }
    
    return layers[index].get();
}

Layer* Project::getCurrentLayer() {
    if (layers.empty() || currentLayerIndex >= layers.size()) {
        return nullptr;
    }
    
    return layers[currentLayerIndex].get();
}

void Project::setCurrentLayerIndex(size_t index) {
    if (index < layers.size()) {
        currentLayerIndex = index;
    }
}

bool Project::saveProject(const std::string& path) const {
    if (!model.isLoaded()) {
        std::cerr << "No model loaded to save project." << std::endl;
        return false;
    }
    
    try {
        // Create project directory
        std::filesystem::path projectPath(path);
        std::filesystem::path projectDir = projectPath.parent_path();
        std::string projectName = projectPath.stem().string();
        
        if (!projectDir.empty() && !std::filesystem::exists(projectDir)) {
            std::filesystem::create_directories(projectDir);
        }
        
        // Create textures directory inside project directory
        std::filesystem::path texturesDir = projectDir / (projectName + "_textures");
        if (!std::filesystem::exists(texturesDir)) {
            std::filesystem::create_directories(texturesDir);
        }
        
        // Save model
        std::string modelPath = (projectDir / (projectName + "_model.obj")).string();
        if (!model.exportModel(modelPath)) {
            std::cerr << "Failed to save model." << std::endl;
            return false;
        }
        
        // Create JSON data
        nlohmann::json projectData;
        projectData["model"] = modelPath;
        projectData["textureWidth"] = textureWidth;
        projectData["textureHeight"] = textureHeight;
        projectData["currentLayerIndex"] = currentLayerIndex;
        
        // Save layers
        nlohmann::json layersData = nlohmann::json::array();
        for (size_t i = 0; i < layers.size(); i++) {
            const Layer* layer = layers[i].get();
            
            // Save layer texture
            std::string texturePath = (texturesDir / (projectName + "_layer_" + std::to_string(i) + ".png")).string();
            if (!layer->saveToFile(texturePath)) {
                std::cerr << "Failed to save layer texture: " << i << std::endl;
                continue;
            }
            
            // Add layer data
            nlohmann::json layerData;
            layerData["name"] = layer->getName();
            layerData["visible"] = layer->isVisible();
            layerData["opacity"] = layer->getOpacity();
            layerData["texture"] = texturePath;
            
            layersData.push_back(layerData);
        }
        projectData["layers"] = layersData;
        
        // Write JSON to file
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open project file for writing: " << path << std::endl;
            return false;
        }
        
        file << projectData.dump(4);
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving project: " << e.what() << std::endl;
        return false;
    }
}

bool Project::loadProject(const std::string& path) {
    try {
        // Clear existing project
        clear();
        
        // Open project file
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open project file: " << path << std::endl;
            return false;
        }
        
        // Parse JSON
        nlohmann::json projectData;
        file >> projectData;
        file.close();
        
        // Load model
        std::string modelPath = projectData["model"];
        if (!model.loadModel(modelPath)) {
            std::cerr << "Failed to load model: " << modelPath << std::endl;
            return false;
        }
        
        // Set texture size
        textureWidth = projectData["textureWidth"];
        textureHeight = projectData["textureHeight"];
        
        // Load layers
        for (const auto& layerData : projectData["layers"]) {
            std::string name = layerData["name"];
            std::string texturePath = layerData["texture"];
            
            // Create layer from texture
            layers.push_back(std::make_unique<Layer>(texturePath, name));
            
            // Set layer properties
            Layer* layer = layers.back().get();
            layer->setVisible(layerData["visible"]);
            layer->setOpacity(layerData["opacity"]);
        }
        
        // Set current layer
        currentLayerIndex = projectData["currentLayerIndex"];
        if (currentLayerIndex >= layers.size()) {
            currentLayerIndex = layers.empty() ? 0 : layers.size() - 1;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading project: " << e.what() << std::endl;
        return false;
    }
}

void Project::clear() {
    // Clear model
    model = Model();
    
    // Clear layers
    layers.clear();
    currentLayerIndex = 0;
    
    // Reset texture size
    textureWidth = 1024;
    textureHeight = 1024;
}

void Project::setDefaultTextureSize() {
    // Calculate appropriate texture size based on model complexity
    const auto& meshes = model.getMeshes();
    int vertexCount = 0;
    
    for (const auto& mesh : meshes) {
        vertexCount += mesh.getVerticesCount();
    }
    
    // Scale texture size based on vertex count
    if (vertexCount > 100000) {
        textureWidth = textureHeight = 4096;
    } else if (vertexCount > 50000) {
        textureWidth = textureHeight = 2048;
    } else {
        textureWidth = textureHeight = 1024;
    }
}
