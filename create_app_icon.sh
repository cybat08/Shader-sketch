#!/bin/bash
# Script to create application icon for Windows

# This script creates an icon file for the Windows application
# It uses convert from ImageMagick to create a multi-resolution .ico file

echo "Creating application icon for 3D Model Painter..."

# Function to create a simple 3D cube icon using ASCII art and convert it to PNG
create_icon() {
  size=$1
  output=$2
  
  # Create a temporary SVG file with a 3D cube icon
  cat > temp_icon.svg << EOF
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg xmlns="http://www.w3.org/2000/svg" width="$size" height="$size" viewBox="0 0 100 100">
  <!-- Background -->
  <rect x="0" y="0" width="100" height="100" rx="15" ry="15" fill="#4A86E8" />
  
  <!-- 3D Cube -->
  <polygon points="25,30 75,30 75,70 25,70" fill="#E8E8E8" stroke="#333333" stroke-width="2" />
  <polygon points="75,30 90,15 90,55 75,70" fill="#C0C0C0" stroke="#333333" stroke-width="2" />
  <polygon points="25,70 40,85 90,55 75,70" fill="#A0A0A0" stroke="#333333" stroke-width="2" />
  
  <!-- Paint brush -->
  <path d="M80,20 L60,40 L65,45 L85,25 Z" fill="#FF5722" stroke="#333333" stroke-width="1.5" />
  <rect x="60" y="40" width="10" height="2" rx="1" transform="rotate(45 60 40)" fill="#8B4513" />

  <!-- Paint drops -->
  <circle cx="25" cy="25" r="5" fill="#FF9800" />
  <circle cx="15" cy="40" r="3" fill="#4CAF50" />
  <circle cx="30" cy="15" r="4" fill="#F44336" />
</svg>
EOF

  # Convert SVG to PNG at specified size
  convert temp_icon.svg -resize ${size}x${size} $output
}

# Create icons at various resolutions
mkdir -p icon_temp
create_icon 16 icon_temp/icon_16.png
create_icon 32 icon_temp/icon_32.png
create_icon 48 icon_temp/icon_48.png
create_icon 64 icon_temp/icon_64.png
create_icon 128 icon_temp/icon_128.png
create_icon 256 icon_temp/icon_256.png

# Combine into an .ico file
convert icon_temp/icon_16.png icon_temp/icon_32.png icon_temp/icon_48.png \
        icon_temp/icon_64.png icon_temp/icon_128.png icon_temp/icon_256.png \
        3DModelPainter.ico

# Clean up temporary files
rm -rf icon_temp temp_icon.svg

echo "Icon created: 3DModelPainter.ico"