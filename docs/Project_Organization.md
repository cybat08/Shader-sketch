# 3D Model Painter Project Organization

This document explains the organization of the 3D Model Painter project, including the different application versions, file structures, and build processes.

## Project Variants

The 3D Model Painter comes in several variants to accommodate different user needs and system capabilities:

### 1. Simple Model Painter (SimpleModelPainter)

- Located in `simple_build/SimpleModelPainter`
- No external dependencies required
- Basic functionality with ASCII-based UI
- Perfect for systems without OpenGL support

### 2. Fun Model Painter (FunModelPainter)

- Located at project root as `FunModelPainter`
- No external dependencies required
- Includes pattern tools and fun painting features
- ASCII-based art with extended character sets for more visual appeal

### 3. Enhanced Model Painter (EnhancedModelPainter)

- Located at project root as `EnhancedModelPainter`
- No external dependencies required
- Advanced tools including Rainbow and Stamp brushes
- Includes full interactive color wheel with harmony suggestions
- File export capabilities (TXT, PPM, SVG, OBJ)

### 4. Color Model Painter (ColorModelPainter)

- Located at project root as `ColorModelPainter`
- No external dependencies required
- Color-focused features with expanded color palette
- Special effects brushes including Stars and DotBrush
- Ideal for artistic users

### 5. Full 3D Model Painter (3DModelPainter)

- Located in `build/3DModelPainter` when built
- Requires OpenGL, GLM, and GLFW dependencies
- Complete 3D rendering and painting capabilities
- Professional-grade texture editing and UV unwrapping
- Most resource-intensive but offers true 3D functionality

## Directory Structure

```
3D-Model-Painter/
├── src/                    # Core source files for full 3D version
│   ├── shaders/            # GLSL shaders for OpenGL rendering
│   ├── application.cpp     # Main application logic
│   ├── camera.cpp          # 3D camera handling
│   ├── model.cpp           # 3D model loading and representation
│   ├── ...
├── build/                  # Build directory for full 3D version
├── simple_build/           # Build directory for simple version
├── docs/                   # Documentation
│   ├── GitHub_Releases.md  # GitHub release process
│   ├── Project_Organization.md  # This file
├── windows_package/        # Windows-specific packaging
├── EnhancedModelPainter.cpp  # Enhanced version main file
├── ColorModelPainter.cpp     # Color-focused version main file
├── FunModelPainter.cpp       # Fun version main file
├── *.sh                    # Various shell scripts
├── *.bat                   # Windows batch scripts
├── install.sh              # Main installation script
├── *.md                    # Various documentation files
```

## Build Process

The build process varies by version:

### Simple Builds (No Dependencies)

These are built with a straightforward g++ command:

```bash
g++ -std=c++17 [variant].cpp -o [variant]
```

Examples:
- `g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter`
- `g++ -std=c++17 EnhancedModelPainter.cpp -o EnhancedModelPainter`

### Full 3D Build (With Dependencies)

Built using CMake:

```bash
mkdir -p build && cd build
cmake ..
make
```

### Windows Build

Uses a combination of:
- `windows_build.bat` for compilation
- `create_installer.bat` for creating NSIS installer
- `resources.rc` for Windows resource integration

## Packaging Strategy

### Linux/macOS Packaging

- Simple tarball packages created with `create_github_release.sh`
- Installation handled by `install.sh`

### Windows Packaging

- NSIS installer script in `3DModelPainter.nsi`
- Creates executable installer with desktop integration
- Includes start menu shortcuts and application icon

## Update Mechanism

The application checks for updates using:
- `.version_info` file to track current version
- `check_releases.sh` to query GitHub API for latest releases
- `update_from_github.sh` to download and install updates automatically