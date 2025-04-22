#!/bin/bash
# 3D Model Painter Installation Script
# This script installs and sets up the 3D Model Painter application

set -e # Exit on error

# Display welcome banner
echo -e "\e[1;36m"
echo "======================================================"
echo "   3D Model Painter - Installation Script"
echo "======================================================"
echo -e "\e[0m"

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    OS="Windows"
else
    OS="Unknown"
fi

echo -e "\e[1;32m[✓] Detected operating system: $OS\e[0m"

# Create directories
mkdir -p build
mkdir -p simple_build

# Determine available features based on dependencies
OPENGL_AVAILABLE=false
GLM_AVAILABLE=false

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

echo -e "\e[1;33m[⧗] Checking for dependencies...\e[0m"

# Check for C++ compiler
if command_exists g++; then
    echo -e "\e[1;32m[✓] Found C++ compiler: $(g++ --version | head -n 1)\e[0m"
elif command_exists clang++; then
    echo -e "\e[1;32m[✓] Found C++ compiler: $(clang++ --version | head -n 1)\e[0m"
else
    echo -e "\e[1;31m[⚠] No C++ compiler found. Please install g++ or clang++.\e[0m"
    exit 1
fi

# Check for OpenGL and GLM
if [[ "$OS" == "Linux" ]]; then
    # Check for pkg-config
    if command_exists pkg-config; then
        # Check OpenGL
        if pkg-config --exists gl; then
            OPENGL_AVAILABLE=true
            echo -e "\e[1;32m[✓] OpenGL development libraries found\e[0m"
        else
            echo -e "\e[1;33m[i] OpenGL development libraries not found (advanced features disabled)\e[0m"
        fi
        
        # Check GLM
        if pkg-config --exists glm; then
            GLM_AVAILABLE=true
            echo -e "\e[1;32m[✓] GLM library found\e[0m"
        else
            echo -e "\e[1;33m[i] GLM library not found (advanced features disabled)\e[0m"
        fi
    else
        echo -e "\e[1;33m[i] pkg-config not found, can't detect OpenGL/GLM (advanced features disabled)\e[0m"
    fi
elif [[ "$OS" == "macOS" ]]; then
    # macOS has OpenGL by default
    OPENGL_AVAILABLE=true
    echo -e "\e[1;32m[✓] OpenGL available (macOS system libraries)\e[0m"
    
    # Check GLM through brew
    if command_exists brew; then
        if brew list glm &>/dev/null; then
            GLM_AVAILABLE=true
            echo -e "\e[1;32m[✓] GLM library found\e[0m"
        else
            echo -e "\e[1;33m[i] GLM library not found (advanced features disabled)\e[0m"
        fi
    else
        echo -e "\e[1;33m[i] Homebrew not found, can't detect GLM (advanced features disabled)\e[0m"
    fi
elif [[ "$OS" == "Windows" ]]; then
    echo -e "\e[1;33m[i] On Windows, using simplified build without dependency checks\e[0m"
fi

# Build the Simple/Fun Model Painter (no dependencies required)
echo -e "\e[1;33m[⧗] Building Fun 3D Model Painter...\e[0m"
g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter
echo -e "\e[1;32m[✓] Successfully built Fun 3D Model Painter\e[0m"

# Copy to simple_build directory
cp FunModelPainter simple_build/SimpleModelPainter
echo -e "\e[1;32m[✓] Copied to SimpleModelPainter in simple_build directory\e[0m"

# Build Enhanced Model Painter
echo -e "\e[1;33m[⧗] Building Enhanced Model Painter...\e[0m"
g++ -std=c++17 EnhancedModelPainter.cpp -o EnhancedModelPainter
echo -e "\e[1;32m[✓] Successfully built Enhanced Model Painter\e[0m"

# Build ColorModelPainter
echo -e "\e[1;33m[⧗] Building Color Model Painter...\e[0m"
g++ -std=c++17 ColorModelPainter.cpp -o ColorModelPainter
echo -e "\e[1;32m[✓] Successfully built Color Model Painter\e[0m"

# Build full 3D version if dependencies are available
if [[ "$OPENGL_AVAILABLE" == "true" && "$GLM_AVAILABLE" == "true" ]]; then
    echo -e "\e[1;33m[⧗] Building full 3D Model Painter with OpenGL...\e[0m"
    cd build
    if command_exists cmake; then
        cmake ..
        make
        echo -e "\e[1;32m[✓] Successfully built full 3D Model Painter\e[0m"
    else
        echo -e "\e[1;31m[⚠] CMake not found, skipping full 3D build\e[0m"
    fi
    cd ..
else
    echo -e "\e[1;33m[i] Skipping full 3D build due to missing dependencies\e[0m"
fi

# Create convenience script for running
cat > run_3d_model_painter.sh << 'EOL'
#!/bin/bash
if [ -f "build/3DModelPainter" ]; then
    echo "Running full 3D Model Painter..."
    ./build/3DModelPainter
elif [ -f "EnhancedModelPainter" ]; then
    echo "Running Enhanced Model Painter..."
    ./EnhancedModelPainter
else
    echo "Running Simple Model Painter..."
    ./simple_build/SimpleModelPainter
fi
EOL

chmod +x run_3d_model_painter.sh

# Create demo script
cat > run_demo.sh << 'EOL'
#!/bin/bash
if [ -f "EnhancedModelPainter" ]; then
    echo "Running Enhanced Model Painter Demo..."
    ./EnhancedModelPainter --demo
elif [ -f "ColorModelPainter" ]; then
    echo "Running Color Model Painter Demo..."
    ./ColorModelPainter --demo
else
    echo "Running Simple Model Painter Demo..."
    ./simple_build/SimpleModelPainter --demo
fi
EOL

chmod +x run_demo.sh

# Success message
echo -e "\e[1;36m"
echo "======================================================"
echo "   Installation Complete!"
echo "======================================================"
echo -e "\e[0m"
echo -e "\e[1;32mTo run the application:\e[0m"
echo -e "\e[1m./run_3d_model_painter.sh\e[0m"
echo
echo -e "\e[1;32mTo run the demo:\e[0m"
echo -e "\e[1m./run_demo.sh\e[0m"
echo
echo -e "\e[1;32mOr run specific variants:\e[0m"
echo -e "\e[1m./FunModelPainter\e[0m (Simplified version)"
echo -e "\e[1m./EnhancedModelPainter\e[0m (Enhanced features)"
echo -e "\e[1m./ColorModelPainter\e[0m (Color-focused version)"

if [[ "$OS" == "Windows" ]]; then
    echo
    echo -e "\e[1;32mOn Windows, you can also run:\e[0m"
    echo -e "\e[1m./windows_build.bat\e[0m (Windows-specific build)"
    echo -e "\e[1m./create_installer.bat\e[0m (Create Windows installer)"
fi

echo
echo -e "\e[1;33mThanks for installing 3D Model Painter!\e[0m"