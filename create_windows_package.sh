#!/bin/bash
# Script to create a Windows package for 3D Model Painter

echo "===== Creating Windows Package for 3D Model Painter ====="

# Create temp directory for packaging
mkdir -p windows_package

# Copy source files and Windows-specific files
cp EnhancedModelPainter.cpp windows_package/
cp windows_build.bat windows_package/
cp run_3d_model_painter.bat windows_package/
cp run_3d_model_painter_demo.bat windows_package/
cp windows_readme.txt windows_package/README.txt

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
echo "- Run scripts (run_3d_model_painter.bat, run_3d_model_painter_demo.bat)"
echo "- Example 3D model (cube.obj, cube.mtl)"
echo "- Documentation (README.txt)"
echo ""
echo "Instructions for Windows users:"
echo "1. Extract the ZIP file to a folder"
echo "2. Run windows_build.bat to compile the application"
echo "3. Run the application using run_3d_model_painter.bat"