
// Minimal Assimp material stub
#ifndef ASSIMP_MATERIAL_H
#define ASSIMP_MATERIAL_H

#include <assimp/scene.h>

#define AI_MATKEY_NAME ?mat.name, 0, 0
#define AI_MATKEY_TEXTURE_DIFFUSE(n) ?mat.texture.diffuse, n, 0

enum aiTextureType {
    aiTextureType_DIFFUSE = 1,
    aiTextureType_SPECULAR,
    aiTextureType_AMBIENT,
    aiTextureType_NORMALS
};

enum aiReturn {
    aiReturn_SUCCESS = 0,
    aiReturn_FAILURE = -1,
    aiReturn_OUTOFMEMORY = -3
};

#endif // ASSIMP_MATERIAL_H
