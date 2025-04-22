#!/bin/bash
# Script to create a Windows package for 3D Model Painter with proper installation

echo "===== Creating Windows Package for 3D Model Painter ====="

# Check for new GitHub releases and update README
if [ -f "./check_releases.sh" ]; then
    echo "Checking for new releases on GitHub..."
    chmod +x ./check_releases.sh
    ./check_releases.sh
    echo "-----------------------------------------------------"
fi

# Create temp directory for packaging
mkdir -p windows_package

# Copy source files and Windows-specific files
cp EnhancedModelPainter.cpp windows_package/
cp windows_build.bat windows_package/
cp run_3d_model_painter.bat windows_package/
cp run_3d_model_painter_demo.bat windows_package/
cp windows_readme.txt windows_package/README.txt
cp 3DModelPainter.ico windows_package/
cp 3DModelPainter.nsi windows_package/
cp resources.rc windows_package/
cp resource.h windows_package/
cp create_installer.bat windows_package/
cp WindowsBuild.md windows_package/

# Copy update scripts and release info
if [ -f "check_releases.sh" ]; then
    cp check_releases.sh windows_package/
    cp update_from_github.sh windows_package/
    cp create_github_release.sh windows_package/
    cp RELEASE_NOTES_TEMPLATE.md windows_package/
    
    # Create a .version_info file if it doesn't exist
    if [ ! -f ".version_info" ]; then
        echo "v0.2.0" > .version_info
    fi
    
    # Copy the version info file
    cp .version_info windows_package/
    
    echo "Added GitHub release detection and update scripts to package"
fi

# Create a basic example model file
cat > windows_package/cube.obj << 'EOF'
# Simple cube model
mtllib cube.mtl
o Cube
v -1.000000 -1.000000 1.000000
v 1.000000 -1.000000 1.000000
v 1.000000 1.000000 1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 -1.000000
v -1.000000 1.000000 -1.000000
vt 0.000000 0.000000
vt 1.000000 0.000000
vt 1.000000 1.000000
vt 0.000000 1.000000
vn 0.000000 0.000000 1.000000
vn 0.000000 0.000000 -1.000000
vn 1.000000 0.000000 0.000000
vn -1.000000 0.000000 0.000000
vn 0.000000 1.000000 0.000000
vn 0.000000 -1.000000 0.000000
usemtl Material
s off
f 1/1/1 2/2/1 3/3/1 4/4/1
f 5/1/2 8/4/2 7/3/2 6/2/2
f 2/1/3 6/2/3 7/3/3 3/4/3
f 1/1/4 4/4/4 8/3/4 5/2/4
f 4/1/5 3/2/5 7/3/5 8/4/5
f 1/1/6 5/4/6 6/3/6 2/2/6
EOF

# Create a matching MTL file
cat > windows_package/cube.mtl << 'EOF'
# Material for cube
newmtl Material
Ns 323.999994
Ka 1.000000 1.000000 1.000000
Kd 0.800000 0.800000 0.800000
Ks 0.500000 0.500000 0.500000
Ke 0.000000 0.000000 0.000000
Ni 1.450000
d 1.000000
illum 2
EOF

# Create ZIP file
cd windows_package
zip -r ../3DModelPainter_Windows.zip *
cd ..

echo "===== Windows Package Created: 3DModelPainter_Windows.zip ====="
echo "Package contains:"
echo "- Source code (EnhancedModelPainter.cpp)"
echo "- Windows build script (windows_build.bat)"
echo "- Installer creation script (create_installer.bat)"
echo "- Run scripts (run_3d_model_painter.bat, run_3d_model_painter_demo.bat)"
echo "- Example 3D model (cube.obj, cube.mtl)"
echo "- Application icon and resources"
echo "- Documentation (README.txt, WindowsBuild.md)"
echo "- GitHub release detection and auto-update scripts"
echo ""
echo "Instructions for Windows users:"
echo "1. Extract the ZIP file to a folder"
echo "2. Run windows_build.bat to compile the application"
echo "3. (Optional) Run create_installer.bat to create a Windows installer"
echo "   This will create 3DModelPainter_Setup.exe that:"
echo "   - Installs the application to Program Files"
echo "   - Creates shortcuts in Start Menu"
echo "   - Adds the application to the taskbar"
echo "   - Creates desktop shortcuts"
echo "   - Registers for Add/Remove Programs"
echo "4. Run the application using run_3d_model_painter.bat or the installer"