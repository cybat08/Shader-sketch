/**
 * 3D Model Painter
 * A C++ application for painting 3D models (OBJ and FBX) with layer support
 */
#include "application.h"
#include <iostream>

int main(int argc, char** argv) {
    try {
        Application app;
        app.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
