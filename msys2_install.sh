#!/bin/bash
# MSYS2 Installation for 3D Model Painter
# This script provides full support for building and running the application on Windows using MSYS2

echo "=== 3D Model Painter: MSYS2 Installation ==="
echo "Setting up your 3D Model Painter application in MSYS2 environment..."

# Check if running in MSYS2
if [[ "$MSYSTEM" != "MINGW64" && "$MSYSTEM" != "MINGW32" && "$MSYSTEM" != "UCRT64" ]]; then
    echo "⚠️ Warning: This script is designed to run in an MSYS2 environment."
    echo "   Please run this script from an MSYS2 MINGW64, MINGW32, or UCRT64 terminal."
    echo "   If you haven't installed MSYS2 yet, please visit: https://www.msys2.org/"
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Install required packages if on MSYS2
if [[ -n "$MSYSTEM" ]]; then
    echo "✓ Installing required packages via pacman..."
    if ! command -v pacman &> /dev/null; then
        echo "❌ Error: pacman not found. Are you running in a proper MSYS2 environment?"
        exit 1
    fi

    # Check if packages are already installed
    if pacman -Q mingw-w64-x86_64-gcc mingw-w64-x86_64-make &> /dev/null; then
        echo "✓ Core build tools already installed"
    else
        echo "Installing core build tools..."
        pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-make
    fi

    # Check for optional dependencies
    if pacman -Q mingw-w64-x86_64-cmake &> /dev/null; then
        echo "✓ CMake already installed"
    else
        echo "Installing CMake..."
        pacman -S --noconfirm mingw-w64-x86_64-cmake
    fi

    if pacman -Q mingw-w64-x86_64-glfw &> /dev/null; then
        echo "✓ GLFW already installed"
    else
        echo "Installing GLFW (for advanced 3D builds)..."
        pacman -S --noconfirm mingw-w64-x86_64-glfw
    fi
fi

# Make all scripts executable
echo "✓ Setting permissions..."
chmod +x *.sh 2>/dev/null || true

# Compile Enhanced Fun Model Painter (works on all platforms)
echo "✓ Compiling Enhanced Fun Model Painter..."
g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter.exe

# Compile Fun Model Painter as backup
echo "✓ Compiling Fun Model Painter..."
g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter.exe

# Create Windows shortcut
echo "✓ Creating Windows shortcut..."
echo '@echo off
cd "%~dp0"
EnhancedFunModelPainter.exe
' > Run_3D_Model_Painter.bat

echo "✓ Desktop integration created (Run_3D_Model_Painter.bat)"

# Create a CMake build script for more advanced builds in MSYS2
echo "✓ Creating advanced build script for MSYS2..."
cat > build_with_msys2.sh << 'EOF'
#!/bin/bash
# Advanced build script for MSYS2

# Create build directory if it doesn't exist
mkdir -p build_msys2
cd build_msys2

# Run CMake to configure the project
echo "Running CMake configuration..."
cmake .. -G "MSYS Makefiles"

# Build the project
echo "Building project..."
make -j$(nproc)

echo "Build complete!"
echo "Executable can be found in the build_msys2 directory"

# Return to the original directory
cd ..
EOF

chmod +x build_with_msys2.sh

echo ""
echo "=== MSYS2 INSTALLATION COMPLETE! ==="
echo "To start the application, double-click Run_3D_Model_Painter.bat"
echo "Or run: ./EnhancedFunModelPainter.exe"
echo ""
echo "For advanced 3D features, use the build_with_msys2.sh script:"
echo "  ./build_with_msys2.sh"
echo ""
echo "Enjoy painting your 3D models!"