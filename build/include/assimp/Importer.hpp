
// Minimal Assimp stub
#ifndef ASSIMP_IMPORTER_HPP
#define ASSIMP_IMPORTER_HPP

#include <string>

namespace Assimp {
    class Importer {
    public:
        Importer() {}
        ~Importer() {}
        
        const struct aiScene* ReadFile(const std::string& file, unsigned int flags) { return nullptr; }
    };
}

#endif // ASSIMP_IMPORTER_HPP
