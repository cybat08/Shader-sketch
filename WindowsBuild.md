# Building 3D Model Painter for Windows

This document explains how to build the 3D Model Painter application as a Windows executable.

## Method 1: Using Visual Studio

1. Install [Visual Studio Community Edition](https://visualstudio.microsoft.com/vs/community/) with C++ desktop development workload
2. Create a new C++ Console Application project
3. Add the `EnhancedModelPainter.cpp` file to your project
4. Set the project to use C++17 or higher standard:
   - Right-click on your project in Solution Explorer
   - Select "Properties"
   - Navigate to "Configuration Properties" > "C/C++" > "Language"
   - Set "C++ Language Standard" to "ISO C++17 Standard (/std:c++17)"
5. Build the project (F7 or Build > Build Solution)
6. Your executable will be in the Debug or Release folder

## Method 2: Using MinGW

1. Install MinGW-w64 with g++ compiler:
   - Download from [MinGW-w64](https://www.mingw-w64.org/downloads/)
   - Add MinGW bin directory to your PATH
2. Open Command Prompt and navigate to the directory containing `EnhancedModelPainter.cpp`
3. Compile with the following command:
   ```
   g++ -std=c++17 EnhancedModelPainter.cpp -o 3DModelPainter.exe -static
   ```
4. This will create `3DModelPainter.exe` in the current directory

## Method 3: Using CMake

1. Install [CMake](https://cmake.org/download/)
2. Install MinGW-w64 or Visual Studio with C++ support
3. Place `EnhancedModelPainter.cpp` and the provided `CMakeLists_windows.txt` in the same directory
4. Rename `CMakeLists_windows.txt` to `CMakeLists.txt`
5. Open Command Prompt and navigate to that directory
6. Create a build directory and navigate to it:
   ```
   mkdir build
   cd build
   ```
7. Configure with CMake:
   - For MinGW:
     ```
     cmake .. -G "MinGW Makefiles"
     ```
   - For Visual Studio:
     ```
     cmake .. -G "Visual Studio 16 2019" -A Win32
     ```
     (use "Visual Studio 17 2022" for VS2022)
8. Build the project:
   ```
   cmake --build . --config Release
   ```
9. Your executable will be in the build directory (or build\Release for Visual Studio)

## Running the Application

Once built, you can run the application by:
1. Double-clicking the exe file
2. Using the provided batch files (if available):
   - `run_3d_model_painter.bat` - Regular mode
   - `run_3d_model_painter_demo.bat` - Demo mode

## Notes

- The application is a console application, so it will open a command prompt window when run
- For best results, run in a terminal that supports ANSI colors (like Windows Terminal)
- If you have problems with ANSI colors, try running with the `--no-color` option:
  ```
  3DModelPainter.exe --no-color
  ```