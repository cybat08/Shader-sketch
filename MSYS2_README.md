# 3D Model Painter - MSYS2 Installation Guide

This guide will help you install and run the 3D Model Painter application using MSYS2 on Windows.

## What is MSYS2?

MSYS2 is a collection of tools and libraries providing you with an easy-to-use environment for building, installing and running native Windows software. It includes a command line terminal called MinGW-w64 Win64 Shell, a package manager called Pacman, and other useful development tools.

## Installation Steps

### 1. Install MSYS2

If you haven't already installed MSYS2, download and install it from the official website:
[https://www.msys2.org/](https://www.msys2.org/)

Follow the installation instructions on their website. After installation, you should have access to several different shells. For this project, we recommend using the **MINGW64** shell.

### 2. Update MSYS2 and Install Required Packages

Launch the MSYS2 MINGW64 shell and run the following commands to update the system and install required packages:

```bash
# Update package database
pacman -Syu

# Install required packages
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake
```

For advanced 3D features, you may also want to install these optional packages:

```bash
pacman -S mingw-w64-x86_64-glfw mingw-w64-x86_64-glm
```

### 3. Build and Run the 3D Model Painter

You have two options for installing the 3D Model Painter:

#### Option 1: Using the Automated Installer

Clone or download this repository, navigate to the folder in your MSYS2 MINGW64 shell, and run:

```bash
./msys2_install.sh
```

This script will install any missing dependencies, compile the application, and create shortcuts for you.

#### Option 2: Manual Build

If you prefer to build the application manually or want more control over the build process:

```bash
# Simple build (no dependencies required)
g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter.exe

# Or use the build script for more options
./build_with_msys2.sh
```

### 4. Running the Application

After building, you can run the application by:

* Double-clicking the generated `EnhancedFunModelPainter.exe` file in Windows Explorer
* Running `./EnhancedFunModelPainter.exe` from the MSYS2 shell
* Using the provided `Run_3D_Model_Painter.bat` shortcut

## Troubleshooting

If you encounter any issues during installation or while running the application, here are some common solutions:

### Missing DLLs

If you get an error about missing DLLs when running the application, make sure you're launching the application from the MSYS2 MINGW64 shell, or add the MSYS2 bin directory to your PATH:

```
C:\msys64\mingw64\bin
```

### Compilation Errors

If you encounter compilation errors:

1. Make sure you have the latest version of GCC installed in MSYS2:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   ```

2. Ensure you're using the MINGW64 shell, not the MSYS shell.

3. Try running with verbose output to see more details:
   ```bash
   g++ -std=c++17 -v EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter.exe
   ```

### Other Issues

If you encounter other issues, please open an issue on the GitHub repository with details about your problem, including:

- The exact error message
- Your MSYS2 version (`pacman -V`)
- Your GCC version (`g++ --version`)

## Advanced Features

For advanced users who want to build with GLFW and other features, check out the `build_with_msys2.sh` script which provides options for different build configurations.

---

Enjoy painting your 3D models with 3D Model Painter!