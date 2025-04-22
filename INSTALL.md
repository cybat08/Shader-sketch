# Installation Guide for 3D Model Painter

This guide explains how to install and run the 3D Model Painter application on your system.

## Quick Installation

The easiest way to install 3D Model Painter is to use the provided installation script:

1. Make the installation script executable:
   ```bash
   chmod +x install.sh
   ```

2. Run the installation script:
   ```bash
   ./install.sh
   ```

The script will automatically:
- Compile all versions of the 3D Model Painter
- Set up necessary files and directories
- Create desktop shortcuts (if applicable)

## Running the Application

After installation, you can run the application in several ways:

### Enhanced Fun Model Painter (Recommended)

This version includes all the latest features with the best user experience:

```bash
./EnhancedFunModelPainter
```

### Fun Model Painter

A simpler version with basic functionality:

```bash
./FunModelPainter
```

### Color Model Painter

A version focusing on color tools:

```bash
./ColorModelPainter
```

## Manual Installation

If you prefer to build the application manually:

1. Compile the Enhanced Fun Model Painter:
   ```bash
   g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter
   ```

2. Compile the Fun Model Painter:
   ```bash
   g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter
   ```

3. Compile the Color Model Painter (if available):
   ```bash
   g++ -std=c++17 ColorModelPainter.cpp -o ColorModelPainter
   ```

## Troubleshooting

If you encounter any issues during installation:

1. Make sure you have the required dependencies installed:
   - A C++17 compatible compiler (g++ or clang++)

2. Check permissions:
   ```bash
   chmod +x *.sh
   ```

3. If you get compilation errors, try running with full diagnostics:
   ```bash
   g++ -std=c++17 -v EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter
   ```

For more assistance, please refer to the README.md file or open an issue in the project repository.

## Uninstallation

To uninstall the 3D Model Painter:

1. Remove the executable files:
   ```bash
   rm EnhancedFunModelPainter FunModelPainter ColorModelPainter
   ```

2. Remove desktop shortcuts (if created):
   ```bash
   rm ~/.local/share/applications/3d-model-painter.desktop
   ```