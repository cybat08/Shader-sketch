#!/bin/bash
# Build script for ShaderSketch GUI using MSYS2

set -e  # Exit on error

echo "=== Building ShaderSketch GUI with MSYS2 ==="

# Check if running in MSYS2
if [[ "$(uname -o)" != "Msys" ]]; then
    echo "Error: This script should be run from MSYS2 MINGW64 shell."
    echo "Please open MSYS2 MINGW64 shell and try again."
    exit 1
fi

# Check for required packages
echo "Checking for required packages..."
required_packages=(
    "mingw-w64-x86_64-gcc"
    "mingw-w64-x86_64-cmake"
    "mingw-w64-x86_64-make"
    "git"
)

missing_packages=()
for pkg in "${required_packages[@]}"; do
    if ! pacman -Q $pkg &>/dev/null; then
        missing_packages+=($pkg)
    fi
done

if [ ${#missing_packages[@]} -gt 0 ]; then
    echo "The following required packages are missing:"
    for pkg in "${missing_packages[@]}"; do
        echo "  - $pkg"
    done
    
    read -p "Do you want to install them now? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        pacman -S --noconfirm "${missing_packages[@]}"
    else
        echo "Cannot continue without required packages. Exiting."
        exit 1
    fi
fi

# Create build directory for GUI
mkdir -p gui_build
cd gui_build

# Use the GUI CMakeLists.txt
cp ../gui_CMakeLists.txt CMakeLists.txt

# Configure and build
echo "Configuring and building ShaderSketch GUI..."
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release .
make -j$(nproc)

# Copy the executable to the main directory
cp ShaderSketch_GUI.exe ..

echo
echo "=== Build completed successfully! ==="
echo "You can run the application by executing ShaderSketch_GUI.exe"