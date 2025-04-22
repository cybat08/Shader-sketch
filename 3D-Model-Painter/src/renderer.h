#pragma once

#include "model.h"
#include "camera.h"
#include "project.h"
#include "shader.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // Render model with camera
    void render(const Model& model, const Camera& camera, const Project& project);
    
    // Ray casting for picking a position on the model
    bool pickPosition(const Model& model, const Camera& camera, 
                     double mouseX, double mouseY, 
                     int windowWidth, int windowHeight,
                     glm::vec3& outWorldPos);
    
private:
    // Shaders
    std::unique_ptr<Shader> basicShader;
    std::unique_ptr<Shader> paintShader;
    
    // Render meshes
    void renderMesh(const Mesh& mesh, const Camera& camera);
    
    // Apply paint layers
    void applyPaintLayers(const Model& model, const Camera& camera, const Project& project);
    
    // Initialize OpenGL
    void initOpenGL();
};
