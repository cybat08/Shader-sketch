#include "renderer.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() {
    // Initialize OpenGL
    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    
    initOpenGL();
    
    // Load shaders
    basicShader = std::make_unique<Shader>("src/shaders/basic.vert", "src/shaders/basic.frag");
    paintShader = std::make_unique<Shader>("src/shaders/paint.vert", "src/shaders/paint.frag");
}

Renderer::~Renderer() {
    // Cleanup handled by shader destructors
}

void Renderer::initOpenGL() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::render(const Model& model, const Camera& camera, const Project& project) {
    // Set viewport dimensions
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render each mesh
    for (const auto& mesh : model.getMeshes()) {
        renderMesh(mesh, camera);
    }
    
    // Apply paint layers
    applyPaintLayers(model, camera, project);
}

void Renderer::renderMesh(const Mesh& mesh, const Camera& camera) {
    // Use basic shader
    basicShader->use();
    
    // Set view and projection matrices
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    basicShader->setMat4("projection", projection);
    basicShader->setMat4("view", view);
    
    // Set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    basicShader->setMat4("model", model);
    
    // Bind the VAO and draw
    glBindVertexArray(mesh.getVAO());
    
    if (mesh.hasIndices()) {
        glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh.getVerticesCount());
    }
    
    glBindVertexArray(0);
}

void Renderer::applyPaintLayers(const Model& model, const Camera& camera, const Project& project) {
    // Use paint shader
    paintShader->use();
    
    // Set view and projection matrices
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    paintShader->setMat4("projection", projection);
    paintShader->setMat4("view", view);
    
    // Set model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    paintShader->setMat4("model", modelMatrix);
    
    // Iterate through layers from bottom to top
    const auto& layers = project.getLayers();
    for (size_t i = 0; i < layers.size(); ++i) {
        if (!layers[i]->isVisible()) {
            continue;
        }
        
        // Set layer parameters
        paintShader->setInt("layerIndex", static_cast<int>(i));
        paintShader->setFloat("layerOpacity", layers[i]->getOpacity());
        
        // Bind layer texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, layers[i]->getTextureID());
        paintShader->setInt("layerTexture", 0);
        
        // Draw model with painted layer
        for (const auto& mesh : model.getMeshes()) {
            glBindVertexArray(mesh.getVAO());
            
            if (mesh.hasIndices()) {
                glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, mesh.getVerticesCount());
            }
            
            glBindVertexArray(0);
        }
    }
}

bool Renderer::pickPosition(const Model& model, const Camera& camera, 
                           double mouseX, double mouseY,
                           int windowWidth, int windowHeight,
                           glm::vec3& outWorldPos) {
    // Convert mouse position to normalized device coordinates
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
    
    // Create NDC position
    glm::vec4 rayClip(x, y, -1.0, 1.0);
    
    // Convert to eye space
    glm::mat4 projInverse = glm::inverse(camera.getProjectionMatrix());
    glm::vec4 rayEye = projInverse * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
    
    // Convert to world space
    glm::mat4 viewInverse = glm::inverse(camera.getViewMatrix());
    glm::vec4 rayWorldH = viewInverse * rayEye;
    glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorldH));
    
    // Ray origin (camera position)
    glm::vec3 rayOrigin = camera.getPosition();
    
    // Find closest intersection with the model
    float closestDist = std::numeric_limits<float>::max();
    bool hasIntersection = false;
    
    for (const auto& mesh : model.getMeshes()) {
        // Get mesh vertices and indices
        const auto& vertices = mesh.getVertices();
        const auto& indices = mesh.getIndices();
        
        // Iterate through triangles
        for (size_t i = 0; i < indices.size(); i += 3) {
            glm::vec3 v0 = vertices[indices[i]].Position;
            glm::vec3 v1 = vertices[indices[i+1]].Position;
            glm::vec3 v2 = vertices[indices[i+2]].Position;
            
            // Ray-triangle intersection
            glm::vec3 e1 = v1 - v0;
            glm::vec3 e2 = v2 - v0;
            glm::vec3 p = glm::cross(rayWorld, e2);
            float det = glm::dot(e1, p);
            
            // If ray is parallel to triangle
            if (det < 1e-6 && det > -1e-6) {
                continue;
            }
            
            float invDet = 1.0f / det;
            
            // Calculate barycentric u coordinate
            glm::vec3 t = rayOrigin - v0;
            float u = glm::dot(t, p) * invDet;
            
            if (u < 0.0f || u > 1.0f) {
                continue;
            }
            
            // Calculate barycentric v coordinate
            glm::vec3 q = glm::cross(t, e1);
            float v = glm::dot(rayWorld, q) * invDet;
            
            if (v < 0.0f || u + v > 1.0f) {
                continue;
            }
            
            // Calculate distance
            float dist = glm::dot(e2, q) * invDet;
            
            if (dist > 1e-6 && dist < closestDist) {
                closestDist = dist;
                outWorldPos = rayOrigin + rayWorld * dist;
                hasIntersection = true;
            }
        }
    }
    
    return hasIntersection;
}
