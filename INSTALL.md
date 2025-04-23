# Installation Guide for 3D Model Painter

This guide explains how to install and run the 3D Model Painter application on your system.

## Super Easy Installation

### Windows Users

1. Simply double-click the `install.bat` file
2. Wait for the installation to complete
3. Double-click the `Run_3D_Model_Painter.bat` file to start the application

That's it! No additional steps required.

### Windows Users with MSYS2

If you have MSYS2 installed on your Windows system, you can use our optimized MSYS2 installer:

1. Open an MSYS2 MINGW64 terminal
2. Navigate to the application folder
3. Run the MSYS2 installer script:
   ```bash
   ./msys2_install.sh
   ```
4. Start the application using:
   ```bash
   ./EnhancedFunModelPainter.exe
   ```
   
   Or double-click `Run_3D_Model_Painter.bat` from Windows Explorer.

For more details, see the [MSYS2 installation guide](MSYS2_README.md).

### Linux/Mac Users

1. Open a terminal in the application folder
2. Run the easy install script:
   ```bash
   ./easy_install.sh
   ```
3. Start the application using:
   ```bash
   ./EnhancedFunModelPainter
   ```
   
   Or using the desktop shortcut created during installation.

The script automatically detects your system and sets up everything for you.

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

### General Issues

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

### MSYS2-Specific Issues

If you're using MSYS2 and encounter problems:

1. Make sure you're using the correct MSYS2 shell (MINGW64 is recommended)

2. Update your MSYS2 packages:
   ```bash
   pacman -Syu
   ```

3. Install the required development tools:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
   ```

4. If you get "command not found" errors, make sure the tools are in your PATH:
   ```bash
   echo $PATH
   # Make sure /mingw64/bin is included
   ```

5. For specific MSYS2 dependency issues, see [MSYS2_README.md](MSYS2_README.md)

For more assistance, please refer to the README.md file or open an issue in the project repository.

## Uninstallation

To uninstall the 3D Model Painter:

### Linux/macOS

1. Remove the executable files:
   ```bash
   rm EnhancedFunModelPainter FunModelPainter ColorModelPainter
   ```

2. Remove desktop shortcuts (if created):
   ```bash
   rm ~/.local/share/applications/3d-model-painter.desktop
   ```

### Windows

1. Delete all `.exe` and `.bat` files from the application directory
2. Remove the Start Menu shortcut if it was created

### MSYS2 (Windows)

1. Remove the executable files:
   ```bash
   rm *.exe
   ```

2. If you installed packages specifically for this application, they remain installed
   in your MSYS2 environment and can be used by other applications