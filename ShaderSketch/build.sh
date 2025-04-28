#!/bin/bash

# Build script for ShaderSketch

# Make script executable
chmod +x "$0"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake to generate build files
cmake ..

# Build the project
make

# Return to the previous directory
cd ..

echo "Build complete. Executable is in build directory."
echo "Run with: ./build/ShaderSketch_GUI_Simple"