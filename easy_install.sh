#!/bin/bash
# One-Click Installation for 3D Model Painter
# Works on Linux, macOS, and Windows (with Git Bash or WSL)

echo "=== 3D Model Painter: Easy Installation ==="
echo "Setting up your 3D Model Painter application..."

# Make all scripts executable
echo "✓ Setting permissions..."
chmod +x *.sh 2>/dev/null || true

# Detect OS
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    echo "✓ Windows system detected"
    IS_WINDOWS=1
else
    echo "✓ Unix-based system detected (Linux/macOS)"
    IS_WINDOWS=0
fi

# Compile Enhanced Fun Model Painter (works on all platforms)
echo "✓ Compiling Enhanced Fun Model Painter..."
if [[ $IS_WINDOWS -eq 1 ]]; then
    g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter.exe
else
    g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter
fi

# Compile Fun Model Painter as backup
echo "✓ Compiling Fun Model Painter..."
if [[ $IS_WINDOWS -eq 1 ]]; then
    g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter.exe
else
    g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter
fi

# Create desktop shortcut if possible
if [[ $IS_WINDOWS -eq 1 ]]; then
    echo "✓ Creating Windows shortcut..."
    echo '@echo off
cd "%~dp0"
EnhancedFunModelPainter.exe
' > Run_3D_Model_Painter.bat
    
    echo "✓ Desktop integration created (Run_3D_Model_Painter.bat)"
else
    if [[ -d "$HOME/.local/share/applications" ]]; then
        echo "✓ Creating desktop shortcut..."
        echo "[Desktop Entry]
Name=3D Model Painter
Comment=Interactive 3D Model Painting Application
Exec=$(pwd)/EnhancedFunModelPainter
Terminal=true
Type=Application
Categories=Graphics;3DGraphics;
Icon=$(pwd)/generated-icon.png
" > "$HOME/.local/share/applications/3d-model-painter.desktop"
        echo "✓ Desktop shortcut created"
    fi
fi

echo ""
echo "=== INSTALLATION COMPLETE! ==="
if [[ $IS_WINDOWS -eq 1 ]]; then
    echo "To start the application, double-click Run_3D_Model_Painter.bat"
    echo "Or run: ./EnhancedFunModelPainter.exe"
else
    echo "To start the application, run: ./EnhancedFunModelPainter"
    echo "Or find '3D Model Painter' in your applications menu"
fi
echo ""
echo "Enjoy painting your 3D models!"