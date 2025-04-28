cmake_minimum_required(VERSION 3.10)
project(ShaderSketch VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/simple_gui.cpp
)

# Find required packages
# (None required for current implementation)

# Executable
add_executable(ShaderSketch_GUI_Simple ${SOURCES})

# Installation
install(TARGETS ShaderSketch_GUI_Simple DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/ShaderSketch/assets)

# Copy assets to build directory for testing
file(COPY assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)