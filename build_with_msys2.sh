#!/bin/bash
# Advanced build script for MSYS2 environment
# This script handles building the 3D Model Painter with MSYS2 tools

# Check if running in MSYS2
if [[ -z "$MSYSTEM" ]]; then
    echo "⚠️ Warning: This script is designed to run in an MSYS2 environment."
    echo "   Please run this script from an MSYS2 MINGW64, MINGW32, or UCRT64 terminal."
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
else
    echo "✓ MSYS2 environment detected: $MSYSTEM"
fi

# Make sure we have the necessary build tools
if ! command -v g++ &> /dev/null; then
    echo "❌ Error: g++ compiler not found."
    echo "Please install it with: pacman -S mingw-w64-x86_64-gcc"
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo "❌ Error: cmake not found."
    echo "Please install it with: pacman -S mingw-w64-x86_64-cmake"
    exit 1
fi

# Create build directory if it doesn't exist
BUILD_DIR="build_msys2"
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Choose build configuration
echo "=== 3D Model Painter MSYS2 Build ==="
echo "Select build configuration:"
echo "1. Simple build (terminal-based, no dependencies)"
echo "2. Enhanced build (with full 3D support, requires GLFW)"
echo "3. Both"
read -p "Selection [1-3]: " BUILD_CONFIG
echo

# Run CMake to configure the project
echo "Running CMake configuration..."

# Copy the appropriate CMakeLists file
if [[ $BUILD_CONFIG -eq 1 || $BUILD_CONFIG -eq 3 ]]; then
    echo "Configuring simple build..."
    # For the simple build, we can just use g++ directly
    g++ -std=c++17 ../EnhancedFunModelPainter.cpp -o ../EnhancedFunModelPainter_MSYS2.exe
    echo "✓ Simple build completed"
fi

if [[ $BUILD_CONFIG -eq 2 || $BUILD_CONFIG -eq 3 ]]; then
    echo "Configuring enhanced build with CMake..."
    # For the enhanced build, we use CMake
    cp ../CMakeLists_msys2.txt ../CMakeLists.txt.bak  # Backup the original file
    cp ../CMakeLists_msys2.txt ../CMakeLists.txt      # Use our MSYS2-optimized file
    
    # Run CMake
    cmake .. -G "MSYS Makefiles"
    
    # Build the enhanced version
    echo "Building enhanced version..."
    cmake --build . --target EnhancedModelPainter_MSYS2
    
    # Restore the original CMakeLists.txt
    cp ../CMakeLists.txt.bak ../CMakeLists.txt
    rm ../CMakeLists.txt.bak
    
    echo "✓ Enhanced build completed"
fi

# Return to the original directory
cd ..

echo ""
echo "=== BUILD COMPLETE! ==="
if [[ $BUILD_CONFIG -eq 1 || $BUILD_CONFIG -eq 3 ]]; then
    echo "Simple version executable: ./EnhancedFunModelPainter_MSYS2.exe"
fi
if [[ $BUILD_CONFIG -eq 2 || $BUILD_CONFIG -eq 3 ]]; then
    echo "Enhanced version executable: ./EnhancedModelPainter_MSYS2.exe"
fi
echo ""
echo "To run the application, use the appropriate executable from the command line"
echo "or double-click it in the file explorer."
echo ""
echo "Enjoy painting your 3D models!"