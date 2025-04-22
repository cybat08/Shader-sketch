#!/bin/bash
# 3D Model Painter Installation Script

echo "=== 3D Model Painter Installer ==="
echo "This script will install the 3D Model Painter application."
echo

# Create necessary directories
mkdir -p build
mkdir -p simple_build

# Step 1: Compile the EnhancedFunModelPainter version (no dependencies)
echo "Step 1: Compiling EnhancedFunModelPainter..."
g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter
if [ $? -eq 0 ]; then
    echo "✓ EnhancedFunModelPainter compiled successfully!"
else
    echo "✗ Failed to compile EnhancedFunModelPainter."
    exit 1
fi

# Step 2: Compile simple versions
echo "Step 2: Compiling FunModelPainter..."
g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter
if [ $? -eq 0 ]; then
    echo "✓ FunModelPainter compiled successfully!"
else
    echo "✗ Failed to compile FunModelPainter."
fi

# Step 3: Compile ColorModelPainter if it exists
echo "Step 3: Compiling ColorModelPainter..."
if [ -f "ColorModelPainter.cpp" ]; then
    g++ -std=c++17 ColorModelPainter.cpp -o ColorModelPainter
    if [ $? -eq 0 ]; then
        echo "✓ ColorModelPainter compiled successfully!"
    else
        echo "✗ Failed to compile ColorModelPainter."
    fi
else
    echo "- ColorModelPainter.cpp not found, skipping."
fi

# Step 4: Create a desktop entry
echo "Step 4: Creating desktop shortcut..."
mkdir -p ~/.local/share/applications/
cat > ~/.local/share/applications/3d-model-painter.desktop << EOF
[Desktop Entry]
Type=Application
Name=3D Model Painter
Comment=Professional-grade 3D model painting application
Exec=$(pwd)/EnhancedFunModelPainter
Icon=$(pwd)/generated-icon.png
Terminal=true
Categories=Graphics;3DGraphics;
EOF

echo "✓ Desktop shortcut created!"

# Print completion message
echo
echo "=== Installation Complete! ==="
echo "You can now run 3D Model Painter in the following ways:"

echo "1. Run the enhanced version with all features:"
echo "   ./EnhancedFunModelPainter"

echo "2. Run the basic fun version:"
echo "   ./FunModelPainter"

if [ -f "ColorModelPainter" ]; then
    echo "3. Run the color-focused version:"
    echo "   ./ColorModelPainter"
fi

echo
echo "To get started, run: ./EnhancedFunModelPainter"
echo "Enjoy your 3D modeling experience!"