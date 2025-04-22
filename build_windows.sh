#!/bin/bash
# Script to build Windows executable for 3D Model Painter

# Text formatting
BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
CYAN="\033[36m"
RESET="\033[0m"

echo -e "${BOLD}${CYAN}===== Building 3D Model Painter for Windows =====${RESET}"

# Create build directory
mkdir -p windows_build
cd windows_build

# Install MinGW if not already installed
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo -e "${YELLOW}MinGW not found. Installing cross-compiler...${RESET}"
    
    # If apt is available (Debian/Ubuntu)
    if command -v apt-get &> /dev/null; then
        sudo apt-get update
        sudo apt-get install -y mingw-w64
    # If dnf is available (Fedora)
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y mingw64-gcc-c++
    # If pacman is available (Arch)
    elif command -v pacman &> /dev/null; then
        sudo pacman -S mingw-w64-gcc
    else
        echo -e "${RED}Error: Unable to install MinGW. Please install it manually.${RESET}"
        exit 1
    fi
fi

echo -e "${GREEN}Using MinGW to cross-compile for Windows...${RESET}"

# Compile for Windows
x86_64-w64-mingw32-g++ -std=c++17 ../EnhancedModelPainter.cpp -o 3DModelPainter.exe -static

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Successfully built Windows executable!${RESET}"
    
    # Create a simple batch file to make it easier to run
    echo "@echo off" > run_3d_model_painter.bat
    echo "echo Starting 3D Model Painter..." >> run_3d_model_painter.bat
    echo "3DModelPainter.exe" >> run_3d_model_painter.bat
    echo "pause" >> run_3d_model_painter.bat
    
    # Create a batch file for demo mode
    echo "@echo off" > run_3d_model_painter_demo.bat
    echo "echo Starting 3D Model Painter Demo..." >> run_3d_model_painter_demo.bat
    echo "3DModelPainter.exe --demo" >> run_3d_model_painter_demo.bat
    echo "pause" >> run_3d_model_painter_demo.bat
    
    # Create a readme.txt file
    echo "3D Model Painter - Windows Version" > readme.txt
    echo "====================================" >> readme.txt
    echo "" >> readme.txt
    echo "Running the application:" >> readme.txt
    echo "1. Double-click 'run_3d_model_painter.bat' to run the full application" >> readme.txt
    echo "2. Double-click 'run_3d_model_painter_demo.bat' to run the demo mode" >> readme.txt
    echo "" >> readme.txt
    echo "Commands:" >> readme.txt
    echo "- 1-5 : Select tools (Brush, Eraser, LineArt, Rainbow, Stamp)" >> readme.txt
    echo "- +/- : Adjust tool size" >> readme.txt
    echo "- p x y : Paint at coordinates x,y" >> readme.txt
    echo "- w : Open color wheel" >> readme.txt
    echo "- u : Open UV editor" >> readme.txt
    echo "- x : Export project" >> readme.txt
    echo "- h : Show help" >> readme.txt
    echo "- q : Quit" >> readme.txt
    
    # Create zip package
    echo -e "${BLUE}Creating Windows package...${RESET}"
    zip -r 3DModelPainter_Windows.zip 3DModelPainter.exe run_3d_model_painter.bat run_3d_model_painter_demo.bat readme.txt
    
    echo -e "${BOLD}${GREEN}Windows executable built successfully!${RESET}"
    echo -e "${CYAN}Windows package created at: windows_build/3DModelPainter_Windows.zip${RESET}"
else
    echo -e "${RED}Failed to build Windows executable.${RESET}"
    exit 1
fi