# ShaderSketch 3D Painting Application

A professional-grade C++ desktop application for 3D model painting and texture editing, offering advanced creative tools for digital artists and designers with cross-platform development support.

## Features

- 3D model loading and rendering
- Direct painting on 3D model triangles
- Multiple painting tools
- Layer management
- Color selection
- 3D model transformation (rotation, scale)
- Cross-platform compatibility

## Directory Structure

```
ShaderSketch/
├── assets/           # 3D models and other resources
├── build/            # Build artifacts (created during build)
├── docs/             # Documentation
├── include/          # Header files
├── src/              # Source files
├── CMakeLists.txt    # CMake build configuration
└── build.sh          # Build script
```

## Building

### Linux/macOS

```bash
./build.sh
```

### Windows with MSYS2

```bash
# Using MSYS2 terminal
./build.sh
```

## Usage

After building, run the application:

```bash
./build/ShaderSketch_GUI_Simple
```

## Commands

- `t [0-4]` - Select tool
- `c [0-7]` - Select color
- `b [size]` - Change brush size
- `p [x] [y]` - Paint at position on canvas
- `paint3d [x] [y]` - Paint on 3D model at model view position
- `l [index]` - Select layer
- `n [name]` - New layer
- `v [index]` - Toggle layer visibility
- `cl` - Clear current layer

### 3D Model Commands
- `rx [angle]` - Rotate around X axis
- `ry [angle]` - Rotate around Y axis
- `rz [angle]` - Rotate around Z axis
- `load [file]` - Load model from OBJ file
- `reset` - Reset model transforms
- `toggle` - Toggle model visibility
- `q` - Quit