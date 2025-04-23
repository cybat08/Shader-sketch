#!/bin/bash
# Build script for ShaderSketch GUI

set -e  # Exit on error

echo "=== Building ShaderSketch GUI ==="

# Create build directory for GUI
mkdir -p gui_build
cd gui_build

# Use the GUI CMakeLists.txt
cp ../gui_CMakeLists.txt CMakeLists.txt

# Configure and build
echo "Configuring and building ShaderSketch GUI..."
cmake -DCMAKE_BUILD_TYPE=Release .
make -j$(nproc)

# Copy the executable to the main directory
cp ShaderSketch_GUI ..

cd ..
echo
echo "=== Build completed successfully! ==="
echo "You can run the application by executing ./ShaderSketch_GUI"

# Make executable
chmod +x ShaderSketch_GUI