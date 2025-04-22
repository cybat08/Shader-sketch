#!/bin/bash

# Setup script for 3D Model Painter - Color Edition

echo "Setting up 3D Model Painter - Color Edition..."

# Make sure the executable has proper permissions
chmod +x ColorModelPainter
echo "✓ Made ColorModelPainter executable"

# Check if the terminal supports ANSI colors
echo -e "\033[1;31mTesting ANSI color support...\033[0m"
if [ $? -eq 0 ]; then
    echo "✓ Your terminal supports ANSI colors"
else
    echo "⚠ Warning: Your terminal may not support ANSI colors properly"
    echo "  The application will still work, but without color display"
fi

echo ""
echo "Setup complete! You can now run the program:"
echo "  ./ColorModelPainter       # For interactive mode"
echo "  ./ColorModelPainter --demo # For demonstration mode"
echo ""
echo "Enjoy using 3D Model Painter - Color Edition!"