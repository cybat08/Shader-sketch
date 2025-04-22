#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Vertex structure
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// Mesh class
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();
    
    // Getters
    unsigned int getVAO() const { return VAO; }
    bool hasIndices() const { return !indices.empty(); }
    size_t getIndicesCount() const { return indices.size(); }
    size_t getVerticesCount() const { return vertices.size(); }
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
    
private:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // OpenGL objects
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    // Set up mesh
    void setupMesh();
};

// Model class
class Model {
public:
    Model();
    ~Model();
    
    // Load model from file
    bool loadModel(const std::string& path);
    
    // Export model to file
    bool exportModel(const std::string& path) const;
    
    // Getters
    const std::vector<Mesh>& getMeshes() const { return meshes; }
    const std::string& getPath() const { return path; }
    bool isLoaded() const { return !meshes.empty(); }
    
private:
    // Model data
    std::vector<Mesh> meshes;
    std::string path;
    std::string directory;
    
    // Process Assimp scene
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
