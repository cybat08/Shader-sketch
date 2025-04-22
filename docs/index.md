# 3D Model Painter Documentation

Welcome to the official documentation for 3D Model Painter, a powerful C++ desktop application for 3D model painting and texture editing.

## Documentation Index

### Getting Started
- [Installation Guide](../README.md#installation)
- [Quick Start](../README.md#running-the-application)
- [Basic Usage](../README.md#usage)

### User Guides
- [Color Wheel Guide](Color_Wheel.md)
- [Export Formats](Export_Formats.md)
- [Windows Installation](Windows_Packaging.md)

### Project Documentation
- [Project Organization](Project_Organization.md)
- [GitHub Release Process](GitHub_Releases.md)
- [Contributing Guidelines](../CONTRIBUTING.md)

### Technical Reference
- [Build Process](Project_Organization.md#build-process)
- [Versioning System](GitHub_Releases.md#versioning)
- [Windows Packaging](Windows_Packaging.md)

## Application Variants

3D Model Painter comes in several variants to suit different needs:

| Variant | Description | Dependencies |
|---------|-------------|--------------|
| Simple Model Painter | Basic functionality with ASCII-based UI | None |
| Fun Model Painter | Includes pattern tools and fun painting features | None |
| Enhanced Model Painter | Advanced tools including color wheel with harmony | None |
| Color Model Painter | Color-focused features with expanded palette | None |
| Full 3D Model Painter | Complete 3D rendering and painting capabilities | OpenGL, GLM, GLFW |

See [Project Organization](Project_Organization.md) for more details on each variant.

## Key Features

### Painting and Editing
- Multiple painting tools (Brush, Eraser, LineArt, RainbowTool, StampTool, PatternTool)
- Adjustable brush sizes
- Layer-based editing with visibility toggling
- Color selection with professional harmony suggestions

### Color Wheel
- Interactive color wheel with visual feedback
- Multiple harmony types (Complementary, Analogous, Triadic, etc.)
- HSV color model with RGB/Hex conversion
- See [Color Wheel Guide](Color_Wheel.md) for detailed usage

### Export Capabilities
- Multiple export formats (OBJ, SVG, PPM, TXT, FBX)
- Material properties preservation
- Customizable export options
- See [Export Formats](Export_Formats.md) for details

### Cross-Platform Support
- Linux, macOS, and Windows compatibility
- Consistent experience across platforms
- Windows-specific enhancements for better integration

## Command Reference

### Basic Commands

| Command | Description |
|---------|-------------|
| `1-6` | Select tools (Brush, Eraser, LineArt, RainbowTool, StampTool, PatternTool) |
| `+/-` | Increase/decrease brush size |
| `1-9, 0` | Select color (0 = eraser) |
| `p x y` | Paint at position (x,y) |
| `c` | Clear current layer |
| `n [name]` | Create a new layer with optional name |
| `l` | List layers |
| `s index` | Select layer by index |
| `t index` | Toggle layer visibility |
| `w` | Open interactive color wheel |
| `u` | Open UV unwrapping tools |
| `x` | Export project (TXT, PPM, SVG, OBJ) |
| `v` | View canvas |
| `h` | Show help |
| `d` | Run demo |
| `q` | Quit application |

### Color Wheel Commands

| Command | Description |
|---------|-------------|
| `h/H` | Adjust hue (decrease/increase) |
| `s/S` | Adjust saturation (decrease/increase) |
| `v/V` | Adjust value (decrease/increase) |
| `n` | Next harmony type |
| `p` | Previous harmony type |
| `1-5` | Select color from harmony |
| `r` | Reset to default colors |
| `q` | Quit color wheel and return to main application |

## Update and Maintenance

3D Model Painter includes automatic update detection from GitHub releases:

1. The application checks for new releases on GitHub
2. Users are notified when a new version is available
3. The `update_from_github.sh` script can automatically update the application

For more information on releases and versions, see [GitHub Release Process](GitHub_Releases.md).

## Get Involved

We welcome contributions to the 3D Model Painter project! Please see our [Contributing Guidelines](../CONTRIBUTING.md) for more information on how to get involved.

## License

3D Model Painter is released under the MIT License - See [LICENSE](../LICENSE) for details.