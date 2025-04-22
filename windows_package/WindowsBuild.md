# Windows Build & Installation Guide for 3D Model Painter

This document explains how to build, package, and install the 3D Model Painter application on Windows with proper taskbar integration and Start Menu entries.

## Quick Start

1. Download the Windows package (`3DModelPainter_Windows.zip`)
2. Extract the package to a folder
3. Run `windows_build.bat` to compile the application
4. (Optional) Run `create_installer.bat` to create a Windows installer
5. Run the application using `run_3d_model_painter.bat` or the installer

## Building from Source

The Windows package includes everything needed to build the application:

1. **EnhancedModelPainter.cpp** - Main source code file
2. **windows_build.bat** - Build script that supports both Visual Studio and MinGW compilers
3. **3DModelPainter.ico** - Application icon
4. **resources.rc** - Resource script for Windows icon and version information
5. **resource.h** - Header file for resources

The build script automatically detects available compilers and selects the best option:
- If Visual Studio is installed, it uses the MSVC compiler (cl.exe)
- If MinGW is installed, it uses g++
- Both options support embedding the application icon and version information

## Windows Integration Features

### Application Icon

The application includes a proper icon that appears in:
- File Explorer
- Taskbar
- Start Menu
- Alt+Tab switcher
- Desktop shortcuts

### Start Menu Integration

When installed, the application adds entries to:
- Start Menu (Programs)
- Desktop shortcuts
- Quick Launch / Taskbar pins

### File Associations

The installer registers the application for .obj files, allowing users to:
- Double-click .obj files to open them
- Right-click and choose "Edit with 3D Model Painter"
- See the application icon for .obj files

### System Integration

The installer also:
- Adds the application to Add/Remove Programs for easy uninstallation
- Registers with Windows Search for better discovery
- Creates proper version information visible in File Properties

## Installation

### Manual Installation

1. Run the compiled executable directly
2. Copy the application files to your preferred location
3. Create shortcuts manually if desired

### Using the Installer

For a proper Windows installation:

1. Build the application with `windows_build.bat`
2. Run `create_installer.bat` to create an installer
3. Run the resulting `3DModelPainter_Setup.exe`
4. Follow the installation wizard

The installer will:
1. Copy files to Program Files
2. Create Start Menu shortcuts
3. Create Desktop shortcuts
4. Add taskbar integration
5. Register file associations
6. Add uninstall information

## Development Notes

### Resources

Windows resources are compiled into the executable and provide:
- Application icon
- Version information
- File descriptions

### Installer Technology

The installer uses NSIS (Nullsoft Scriptable Install System), which provides:
- Professional-looking installation wizard
- Registry integration
- Shortcut creation
- Uninstaller generation

### TaskBar Integration

Windows taskbar integration is achieved through:
1. Proper application registration in the registry
2. Quick Launch shortcuts
3. File type associations
4. Application icon embedding

## Troubleshooting

### Compiler issues

If the build fails:
- Ensure you have either Visual Studio or MinGW installed
- For Visual Studio, make sure you have the "Desktop Development with C++" workload
- For MinGW, ensure it's in your PATH

### Installer issues

If creating the installer fails:
- Install NSIS from https://nsis.sourceforge.io/Download
- Ensure NSIS is in your PATH or installed in the default location