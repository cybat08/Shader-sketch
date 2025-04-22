
// Minimal Assimp scene stub
#ifndef ASSIMP_SCENE_H
#define ASSIMP_SCENE_H

struct aiVector3D {
    float x, y, z;
};

struct aiColor4D {
    float r, g, b, a;
};

struct aiString {
    char data[1024];
};

struct aiMesh {
    unsigned int mNumVertices;
    aiVector3D* mVertices;
    aiVector3D* mNormals;
    aiVector3D* mTextureCoords[8];
    unsigned int mNumFaces;
    struct aiFace* mFaces;
    unsigned int mMaterialIndex;
};

struct aiFace {
    unsigned int mNumIndices;
    unsigned int* mIndices;
};

struct aiMaterial {
    // Empty
};

struct aiScene {
    unsigned int mNumMeshes;
    aiMesh** mMeshes;
    unsigned int mNumMaterials;
    aiMaterial** mMaterials;
};

#endif // ASSIMP_SCENE_H
