# 3D Model Painter

A powerful C++ desktop application for 3D model painting with advanced interactive tools, intuitive user interface, and comprehensive export capabilities.

## Features

- **Support for multiple 3D model and texture export formats** (OBJ, FBX, SVG, PPM, TXT)
- **Multi-layer painting system** with advanced brush controls
- **Comprehensive tool selection** (Brush, Eraser, Spray, Rainbow, Stamp, Pattern)
- **Interactive color wheel with harmony suggestions** for better color selection
- **UV unwrapping and editing tools** for precise texture control
- **Flexible brush size and color controls**
- **Cross-platform support** for Windows, Linux, and macOS

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

## Version History

- v0.2.0 - Added interactive color wheel and UV unwrapping tools
- v0.1.0 - Initial release with basic painting functionality