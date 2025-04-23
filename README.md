# 3D Model Painter

A powerful C++ desktop application for 3D model painting, offering professional-grade texture and model editing capabilities with comprehensive export functionality and advanced packaging support.

![3D Model Painter](generated-icon.png)

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Features

- Interactive 3D model painting with multiple tools
- Color wheel with harmony suggestions for professional color selection
- Multiple painting tools (Brush, Rainbow, Pattern, Stamp, and more)
- Advanced effects for a more fun and engaging experience
- Multiple export formats (OBJ, SVG, PPM, TXT)
- Cross-platform compatibility (Linux, macOS, Windows)

## Installation

### Super Easy Installation

#### Windows Users
Simply double-click the `install.bat` file and follow the on-screen instructions.

#### Linux/Mac Users
Open a terminal in the application folder and run:
```bash
./easy_install.sh
```

The installation script automatically:
1. Detects your operating system
2. Compiles the application
3. Creates desktop shortcuts
4. Sets up a launcher to run the application with one click

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

## Documentation

Comprehensive documentation is available in the `docs/` directory:

- [Color Wheel Guide](docs/Color_Wheel.md) - Detailed information about the color wheel and harmony feature
- [Export Formats](docs/Export_Formats.md) - Information about supported export formats
- [Windows Packaging](docs/Windows_Packaging.md) - Details on Windows installation and packaging
- [Project Organization](docs/Project_Organization.md) - Overview of the project structure
- [GitHub Release Process](docs/GitHub_Releases.md) - Information about the release process

The [documentation index](docs/index.md) provides a complete overview of all available documentation.

## License

MIT License - See [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) for details on the process for submitting pull requests.

## Releases

Check the [Releases](https://github.com/3D-Model-Painter/3D-Model-Painter/releases) page for the latest versions and release notes.

<!-- RELEASE_INFO_START -->
## Latest Release: v0.2.0

Released on: 2023-11-15
[View on GitHub](https://github.com/3D-Model-Painter/3D-Model-Painter/releases/tag/v0.2.0)

### What's New:
- Added interactive color wheel with harmony suggestions
- Implemented Windows taskbar integration
- Added support for SVG and PPM export formats
- Improved layer management system
- Enhanced brush tools with new pattern options
<!-- RELEASE_INFO_END -->