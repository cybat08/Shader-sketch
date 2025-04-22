# ShaderSketch

A C++ desktop application for painting 3D models with layer support, similar to professional image editing software.

![3D Model Painter](docs/screenshot.png)

## Features

- Load and visualize 3D models (OBJ and FBX formats)
- Layer-based painting system
- Multiple brush tools (Brush, Eraser, LineArt)
- Adjustable brush size
- Simple and intuitive interface

## Getting Started

### Prerequisites

To build the application from source, you'll need:

- C++ compiler with C++17 support
- CMake 3.10 or higher

For the full 3D version, you'll also need:
- OpenGL
- GLFW
- GLM
- Assimp
- ImGui

### Installation

#### Pre-built Binaries

Pre-built binaries are available in the [Releases](https://github.com/yourusername/3D-Model-Painter/releases) section.

#### Building from Source

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/3D-Model-Painter.git
   cd 3D-Model-Painter
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Build the application:
   ```
   cmake ..
   make
   ```

4. Run the application:
   ```
   ./3DModelPainter
   ```

## Usage

### Basic Commands

- `1`, `2`, `3` - Select tool (Brush, Eraser, LineArt)
- `+`, `-` - Increase/decrease brush size
- `p x y` - Paint at position (x,y)
- `c` - Clear current layer
- `n [name]` - New layer with optional name
- `l` - List layers
- `s index` - Select layer by index
- `v` - View canvas
- `h` - Show help
- `q` - Quit

### Example Workflow

1. Start the application
2. Create a new layer using `n Background`
3. Use the brush tool to paint on the layer with `p 10 10`
4. Create another layer with `n Details`
5. Select a different tool with `2` for eraser or `3` for line art
6. Paint on the new layer
7. View your work with `v`

## Project Structure

- `/src` - Source code
- `/bin` - Compiled binaries
- `/docs` - Documentation
- `/examples` - Example models and projects

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [OpenGL](https://www.opengl.org/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- [Assimp](https://github.com/assimp/assimp)
- [ImGui](https://github.com/ocornut/imgui)
