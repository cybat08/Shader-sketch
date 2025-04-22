# 3D Model Painter

[![Latest Release](https://img.shields.io/github/v/release/3D-Model-Painter/3D-Model-Painter?style=flat-square&logo=github)](https://github.com/3D-Model-Painter/3D-Model-Painter/releases/latest)

A powerful C++ desktop application for 3D model painting with advanced interactive tools, intuitive user interface, and comprehensive export capabilities.

## Features

- **Support for multiple 3D model and texture export formats** (OBJ, FBX, SVG, PPM, TXT)
- **Multi-layer painting system** with advanced brush controls
- **Comprehensive tool selection** (Brush, Eraser, Spray, Rainbow, Stamp, Pattern)
- **Interactive color wheel with harmony suggestions** for better color selection
- **UV unwrapping and editing tools** for precise texture control
- **Flexible brush size and color controls**
- **Cross-platform support** for Windows, Linux, and macOS
- **Windows taskbar integration** with proper installer
- **Automated GitHub release detection** and updates

## Installation

### Quick Installation

Run the installer script:

```bash
./install.sh
```

The installer will:
1. Detect your operating system
2. Install necessary dependencies
3. Compile the application
4. Create shortcuts (on Linux)
5. Provide usage instructions

### Windows Installation

For Windows users, we provide a dedicated Windows package:

1. Download the latest Windows package from the [Releases](https://github.com/3D-Model-Painter/3D-Model-Painter/releases) page
2. Extract the ZIP file to a folder of your choice
3. Run `windows_build.bat` to compile the application
4. (Optional) Run `create_installer.bat` to create a Windows installer with:
   - Start Menu integration
   - Desktop shortcuts
   - Taskbar integration
   - File associations
5. Run the application using the shortcuts or `run_3d_model_painter.bat`

### Manual Installation

If the installer doesn't work for you, follow these steps:

1. Ensure you have C++ compiler (g++ or equivalent) and make installed
2. Compile the application:
   ```bash
   g++ -std=c++17 FunModelPainter.cpp -o 3DModelPainter
   ```
3. Run the application:
   ```bash
   ./3DModelPainter
   ```

## Versions

The package includes two main versions of the application:

### Original Version (`3DModelPainter`)
The original version with basic functionality.

### Enhanced Version (`EnhancedModelPainter`)
The enhanced version with additional features:
- Interactive color wheel with harmony suggestions
- UV unwrapping and editing tools
- Multiple export formats support

## Usage

### Basic Commands (Enhanced Version)

- `1-5` - Select tools (Brush, Eraser, LineArt, Rainbow, Stamp, Pattern)
- `+/-` - Adjust tool size
- `1-9, 0` - Select color (0 = eraser)
- `p x y` - Paint at position (x,y)
- `c` - Clear current layer
- `n [name]` - New layer with optional name
- `l` - List layers
- `s index` - Select layer by index
- `t index` - Toggle layer visibility
- `w` - Open interactive color wheel with harmony suggestions
- `u` - Open UV unwrapping and editing tools
- `x` - Export project (TXT, PPM, SVG, OBJ)
- `v` - View canvas
- `h` - Show help
- `d` - Run demo
- `q` - Quit application

### Color Wheel Usage

The interactive color wheel provides:
- Visual color selection
- HSV color model control
- Harmony color suggestions (complementary, analogous, triadic, etc.)
- Real-time color feedback

### UV Unwrapping Tools

The UV unwrapping interface allows:
- Automatic UV unwrapping
- Planar unwrapping for specific faces
- UV scale and offset adjustment
- UV map visualization

## Export Formats

- **TXT** - Text-based representation of the model
- **PPM** - Portable Pixmap format for textures
- **SVG** - Scalable Vector Graphics format
- **OBJ/MTL** - Wavefront OBJ format with material files

## License

This software is released under the MIT License. See the LICENSE file for details.

## Auto-Update System

The application includes an automated GitHub release detection system:

### Checking for Updates

To check for new releases and updates:

```bash
./check_releases.sh
```

This script will:
- Check for new releases on GitHub
- Update the README.md with latest release information
- Add release badge with current version

### Full Update Process

For a complete update with asset downloading:

```bash
./update_from_github.sh
```

This enhanced script will:
- Check for new releases on GitHub
- Update the README with release information
- Download all release assets automatically
- Create release notes files
- Offer to extract ZIP archives

### Creating New Releases

To create a new GitHub release:

```bash
./create_github_release.sh
```

This tool will:
- Check your current version
- Allow version updating
- Create standardized release notes
- Package Windows distribution
- Upload assets to GitHub
- Create the release with proper tags

## Version History

- v0.2.0 - Added interactive color wheel and UV unwrapping tools
- v0.1.0 - Initial release with basic painting functionality

<!-- RELEASE_INFO_START -->
## Latest Release: v0.2.0

Released on: 2023-12-01
[View on GitHub](https://github.com/3D-Model-Painter/3D-Model-Painter/releases/latest)

### What's New:
- Enhanced UI with better color representation
- Added Windows taskbar integration
- Added automatic GitHub release detection
- Improved export functionality
- Fixed various bugs and performance issues
<!-- RELEASE_INFO_END -->