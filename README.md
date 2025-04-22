# ShaderSketch

**ShaderSketch** is a lightweight, experimental C++ 3D painting tool using OpenGL. It allows users to paint directly onto 3D models in real-time with shader-based effects. It's designed to be minimal, modifiable, and cross-platform — ideal for developers exploring procedural shading or real-time texture manipulation.

---

## ✨ Features

- 🎨 Real-time 3D texture painting
- 📦 OBJ model loading support
- 🧠 Simple GLSL-based shaders
- 🔧 Modular C++ structure
- 🖱️ Mouse interaction for painting
- 🧰 Basic ImGui GUI integration (optional)

---

## 🗂️ Project Structure


---

## 🚀 Getting Started

### 🔧 Prerequisites

- C++17 compiler (GCC, Clang, or MSVC)
- [CMake](https://cmake.org/) (v3.10+)
- [GLFW](https://www.glfw.org/) (window/input)
- [GLEW](http://glew.sourceforge.net/) or similar (OpenGL extensions)
- OpenGL 3.3 or higher
- (Optional) [ImGui](https://github.com/ocornut/imgui) for GUI integration

---

### 🛠️ Building (Linux/Mac/Windows)

```bash
git clone https://github.com/cybat08/ShaderSketch.git
cd ShaderSketch
mkdir build
cd build
cmake ..
make
