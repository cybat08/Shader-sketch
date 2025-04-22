# 3D Model Painter

A powerful C++ desktop application for 3D model painting, offering professional-grade texture and model editing capabilities with comprehensive export functionality and advanced packaging support.

![3D Model Painter](generated-icon.png)

## Features

- Interactive 3D model painting with multiple tools
- Color wheel with harmony suggestions for professional color selection
- UV unwrapping and editing tools
- Multiple export formats (OBJ, FBX, SVG, PPM, TXT)
- Cross-platform compatibility (Linux, macOS, Windows)
- Windows installer with proper desktop integration

## Installation

### Quick Install (via Git)

```bash
# Clone the repository
git clone https://github.com/yourusername/3D-Model-Painter.git
cd 3D-Model-Painter

# Make the installation script executable
chmod +x install.sh

# Run the installation script
./install.sh
```

The installation script will:
1. Detect your operating system
2. Check for dependencies
3. Build the appropriate version based on available libraries
4. Create convenience scripts for running the application

### Manual Installation

If you prefer to build manually:

```bash
# Build the simple version (no dependencies required)
g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter

# Build the enhanced version
g++ -std=c++17 EnhancedModelPainter.cpp -o EnhancedModelPainter

# Build the color-focused version
g++ -std=c++17 ColorModelPainter.cpp -o ColorModelPainter
```

### Full 3D Version (with OpenGL)

For the complete 3D-enabled version:

```bash
# Install required dependencies
# On Ubuntu/Debian:
sudo apt-get install libgl1-mesa-dev libglm-dev libglfw3-dev

# On macOS:
brew install glm glfw

# On Windows:
# Use the windows_build.bat script

# Build using CMake
mkdir -p build && cd build
cmake ..
make
```

## Running the Application

After installation:

```bash
# Run the best available version automatically
./run_3d_model_painter.sh

# Run the demo
./run_demo.sh

# Or run specific variants
./FunModelPainter             # Simplified version
./EnhancedModelPainter        # Enhanced features
./ColorModelPainter           # Color-focused version
./build/3DModelPainter        # Full 3D version (if built)
```

## Windows Package

For Windows users, we provide:

```bash
# Build for Windows
./windows_build.bat

# Create Windows installer
./create_installer.bat
```

The Windows package includes:
- Start menu integration
- Desktop shortcuts
- Proper application icon
- Automatic updates from GitHub releases

## Usage

### Basic Commands

- **Tool Selection**: `1-6` to select different tools
- **Brush Settings**:
  - `+/-` to increase/decrease brush size
  - `1-9, 0` to select colors
- **Painting**: `p x y` to paint at coordinates (x,y)
- **Layers**:
  - `n [name]` to create a new layer
  - `l` to list layers
  - `s index` to select a layer
- **Advanced Features**:
  - `w` to open the interactive color wheel
  - `u` to use UV unwrapping tools
  - `x` to export your project

### Color Wheel with Harmony

The color wheel provides professional color selection with color theory principles:
- Use `h`, `s`, `v` to adjust hue, saturation, and value
- Use `n`, `p` to cycle through harmony types (complementary, analogous, etc.)
- Use number keys (`1-5`) to select colors from the current harmony

## License

MIT License - See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.