#include "model.h"
#include <glad/glad.h>
#include <iostream>
#include <assimp/Exporter.hpp>

// Mesh implementation
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0) {
    setupMesh();
}

Mesh::~Mesh() {
    // Clean up OpenGL resources
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
    }
    
    // Bind VAO
    glBindVertexArray(VAO);
    
    // Load data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // Load indices
    if (!indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }
    
    // Set vertex attribute pointers
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    // Texture coordinates attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    // Unbind VAO
    glBindVertexArray(0);
}

// Model implementation
Model::Model() {}

Model::~Model() {
    // Meshes are cleaned up by their destructors
}

bool Model::loadModel(const std::string& path) {
    // Clear existing data
    meshes.clear();
    
    // Save path
    this->path = path;
    
    // Extract directory
    directory = path.substr(0, path.find_last_of('/'));
    
    // Create importer
    Assimp::Importer importer;
    
    // Import scene
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | 
        aiProcess_CalcTangentSpace);
    
    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }
    
    // Process the scene
    processNode(scene->mRootNode, scene);
    
    return true;
}

bool Model::exportModel(const std::string& path) const {
    if (meshes.empty()) {
        std::cerr << "Cannot export empty model." << std::endl;
        return false;
    }
    
    // Determine export format from path extension
    std::string extension = path.substr(path.find_last_of(".") + 1);
    std::string formatId;
    
    if (extension == "obj") {
        formatId = "obj";
    } else if (extension == "fbx") {
        formatId = "fbx";
    } else {
        std::cerr << "Unsupported export format: " << extension << std::endl;
        return false;
    }
    
    // Create exporter
    Assimp::Exporter exporter;
    
    // Create a new scene
    aiScene scene;
    scene.mRootNode = new aiNode();
    
    // Initialize scene with meshes
    scene.mNumMeshes = static_cast<unsigned int>(meshes.size());
    scene.mMeshes = new aiMesh*[scene.mNumMeshes];
    
    // Create meshes
    for (unsigned int i = 0; i < scene.mNumMeshes; i++) {
        const Mesh& mesh = meshes[i];
        
        scene.mMeshes[i] = new aiMesh();
        aiMesh* aiMesh = scene.mMeshes[i];
        
        // Set vertices
        aiMesh->mNumVertices = static_cast<unsigned int>(mesh.getVertices().size());
        aiMesh->mVertices = new aiVector3D[aiMesh->mNumVertices];
        aiMesh->mNormals = new aiVector3D[aiMesh->mNumVertices];
        aiMesh->mTextureCoords[0] = new aiVector3D[aiMesh->mNumVertices];
        aiMesh->mNumUVComponents[0] = 2;
        
        for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
            const Vertex& vertex = mesh.getVertices()[j];
            
            aiMesh->mVertices[j] = aiVector3D(vertex.Position.x, vertex.Position.y, vertex.Position.z);
            aiMesh->mNormals[j] = aiVector3D(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
            aiMesh->mTextureCoords[0][j] = aiVector3D(vertex.TexCoords.x, vertex.TexCoords.y, 0.0f);
        }
        
        // Set faces
        if (mesh.hasIndices()) {
            aiMesh->mNumFaces = static_cast<unsigned int>(mesh.getIndices().size() / 3);
            aiMesh->mFaces = new aiFace[aiMesh->mNumFaces];
            
            for (unsigned int j = 0; j < aiMesh->mNumFaces; j++) {
                aiFace& face = aiMesh->mFaces[j];
                face.mNumIndices = 3;
                face.mIndices = new unsigned int[3];
                
                face.mIndices[0] = mesh.getIndices()[j * 3 + 0];
                face.mIndices[1] = mesh.getIndices()[j * 3 + 1];
                face.mIndices[2] = mesh.getIndices()[j * 3 + 2];
            }
        }
        
        // Add to scene
        scene.mRootNode->mNumMeshes++;
        scene.mRootNode->mMeshes = new unsigned int[scene.mRootNode->mNumMeshes];
        scene.mRootNode->mMeshes[scene.mRootNode->mNumMeshes - 1] = i;
    }
    
    // Export scene
    aiReturn result = exporter.Export(&scene, formatId, path);
    
    // Clean up scene
    for (unsigned int i = 0; i < scene.mNumMeshes; i++) {
        delete[] scene.mMeshes[i]->mVertices;
        delete[] scene.mMeshes[i]->mNormals;
        delete[] scene.mMeshes[i]->mTextureCoords[0];
        
        for (unsigned int j = 0; j < scene.mMeshes[i]->mNumFaces; j++) {
            delete[] scene.mMeshes[i]->mFaces[j].mIndices;
        }
        
        delete[] scene.mMeshes[i]->mFaces;
        delete scene.mMeshes[i];
    }
    
    delete[] scene.mMeshes;
    delete[] scene.mRootNode->mMeshes;
    delete scene.mRootNode;
    
    if (result != aiReturn_SUCCESS) {
        std::cerr << "Failed to export model: " << exporter.GetErrorString() << std::endl;
        return false;
    }
    
    return true;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    
    // Process child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        
        // Position
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        
        // Normal
        if (mesh->HasNormals()) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }
        
        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        
        vertices.push_back(vertex);
    }
    
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // Create mesh
    return Mesh(vertices, indices);
}
