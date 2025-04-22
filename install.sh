#!/bin/bash
# 3D Model Painter Installer Script
# This script helps install the 3D Model Painter application and its dependencies

# Text formatting
BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
RESET="\033[0m"

# Function to print colored messages
print_message() {
    echo -e "${2}${1}${RESET}"
}

# Function to print step headers
print_step() {
    echo ""
    print_message "===== ${1} =====" "${BOLD}${BLUE}"
}

# Check operating system
check_os() {
    print_step "Detecting Operating System"
    
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        print_message "Linux detected!" "${GREEN}"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        print_message "macOS detected!" "${GREEN}"
    else
        OS="unknown"
        print_message "Unsupported operating system: $OSTYPE" "${YELLOW}"
        print_message "The installer will try to continue, but you may need to install dependencies manually." "${YELLOW}"
    fi
}

# Install required system dependencies
install_dependencies() {
    print_step "Installing System Dependencies"
    
    if [[ "$OS" == "linux" ]]; then
        # Check for package manager
        if command -v apt-get &> /dev/null; then
            print_message "Using apt package manager..." "${CYAN}"
            sudo apt-get update
            sudo apt-get install -y g++ cmake make
        elif command -v dnf &> /dev/null; then
            print_message "Using dnf package manager..." "${CYAN}"
            sudo dnf install -y gcc-c++ cmake make
        elif command -v pacman &> /dev/null; then
            print_message "Using pacman package manager..." "${CYAN}"
            sudo pacman -Sy gcc cmake make
        else
            print_message "Couldn't determine package manager. Please install g++, cmake, and make manually." "${YELLOW}"
        fi
    elif [[ "$OS" == "macos" ]]; then
        # Check for Homebrew
        if command -v brew &> /dev/null; then
            print_message "Using Homebrew package manager..." "${CYAN}"
            brew install cmake
        else
            print_message "Homebrew not found. Installing Homebrew..." "${CYAN}"
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            brew install cmake
        fi
        
        # Check if Xcode command line tools are installed
        if ! command -v g++ &> /dev/null; then
            print_message "Installing Xcode Command Line Tools..." "${CYAN}"
            xcode-select --install
        fi
    fi
    
    print_message "Dependencies installed!" "${GREEN}"
}

# Compile the terminal-based version
compile_app() {
    print_step "Compiling 3D Model Painter"
    
    # Create build directory
    mkdir -p terminal_build
    cd terminal_build
    
    print_message "Compiling enhanced versions..." "${CYAN}"
    
    # Compile FunModelPainter (original version)
    print_message "Compiling FunModelPainter..." "${CYAN}"
    g++ -std=c++17 ../FunModelPainter.cpp -o 3DModelPainter
    
    # Compile EnhancedModelPainter (new version with color wheel and UV tools)
    print_message "Compiling EnhancedModelPainter..." "${CYAN}"
    g++ -std=c++17 ../EnhancedModelPainter.cpp -o EnhancedModelPainter
    
    if [[ $? -eq 0 ]]; then
        print_message "Compilation successful!" "${GREEN}"
    else
        print_message "Compilation failed. Please check the error messages above." "${RED}"
        exit 1
    fi
    
    cd ..
}

# Create a desktop shortcut (for Linux)
create_shortcut() {
    if [[ "$OS" == "linux" ]]; then
        print_step "Creating Desktop Shortcut"
        
        # Create a desktop entry
        DESKTOP_FILE=~/.local/share/applications/3DModelPainter.desktop
        mkdir -p ~/.local/share/applications
        
        echo "[Desktop Entry]
Name=3D Model Painter
Comment=A colorful 3D model painting application
Exec=$(pwd)/terminal_build/EnhancedModelPainter
Terminal=true
Type=Application
Categories=Graphics;3DGraphics;" > "$DESKTOP_FILE"
        
        DESKTOP_FILE_DEMO=~/.local/share/applications/3DModelPainterDemo.desktop
        echo "[Desktop Entry]
Name=3D Model Painter Demo
Comment=A demo of the 3D model painting application
Exec=$(pwd)/terminal_build/EnhancedModelPainter --demo
Terminal=true
Type=Application
Categories=Graphics;3DGraphics;" > "$DESKTOP_FILE_DEMO"
        
        chmod +x "$DESKTOP_FILE_DEMO"
        print_message "Demo shortcut created at $DESKTOP_FILE_DEMO" "${GREEN}"
        
        chmod +x "$DESKTOP_FILE"
        
        print_message "Desktop shortcut created at $DESKTOP_FILE" "${GREEN}"
    fi
}

# Final instructions
show_instructions() {
    print_step "Installation Complete"
    
    echo -e "${BOLD}${GREEN}3D Model Painter has been successfully installed!${RESET}"
    echo ""
    echo -e "${BOLD}To run the application:${RESET}"
    echo -e "${CYAN}  ./terminal_build/3DModelPainter${RESET} (Original version)"
    echo -e "${CYAN}  ./terminal_build/EnhancedModelPainter${RESET} (Enhanced version with color wheel and UV tools)"
    echo -e "${CYAN}  ./terminal_build/EnhancedModelPainter --demo${RESET} (Run demo animation)"
    echo ""
    echo -e "${BOLD}Features:${RESET}"
    echo -e "${YELLOW}  • Interactive Color Wheel with Harmony Suggestions${RESET}"
    echo -e "${YELLOW}  • UV Unwrapping and Editing Tools${RESET}"
    echo -e "${YELLOW}  • Multiple Export Format Support${RESET}"
    echo ""
    echo -e "${BOLD}${MAGENTA}Thank you for installing 3D Model Painter!${RESET}"
}

# Main installation process
main() {
    print_message "===========================================" "${BOLD}${CYAN}"
    print_message "       3D MODEL PAINTER INSTALLER       " "${BOLD}${CYAN}"
    print_message "===========================================" "${BOLD}${CYAN}"
    echo ""
    
    check_os
    install_dependencies
    compile_app
    create_shortcut
    show_instructions
}

# Run the installer
main