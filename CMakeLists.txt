cmake_minimum_required(VERSION 3.10)
project(3DModelPainter VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/assimp/include
    ${PROJECT_SOURCE_DIR}/vendor/imgui
    ${PROJECT_SOURCE_DIR}/vendor/stb
    ${PROJECT_SOURCE_DIR}/vendor/json/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/application.cpp
    src/renderer.cpp
    src/model.cpp
    src/texture.cpp
    src/camera.cpp
    src/layer.cpp
    src/paint_tool.cpp
    src/shader.cpp
    src/ui.cpp
    src/project.cpp
    src/utils.cpp
    
    # Third-party sources
    vendor/glad/src/glad.c
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

# Define executable
add_executable(3DModelPainter ${SOURCES})

# Link libraries
target_link_libraries(3DModelPainter
    OpenGL::GL
    ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/assimp/lib/libassimp.a
    ${CMAKE_DL_LIBS}
    pthread
)

# Copy shader files to build directory
file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
