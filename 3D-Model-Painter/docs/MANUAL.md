# 3D Model Painter User Manual

## Introduction

3D Model Painter is a desktop application designed for painting and texturing 3D models. It provides a layer-based approach similar to professional image editing software, making it easy to create complex textures for your 3D models.

## Installation

See the main [README.md](../README.md) file for installation instructions.

## Interface

The application provides a text-based interface with the following commands:

### Tool Selection

- `1` - Select Brush tool
- `2` - Select Eraser tool
- `3` - Select LineArt tool

### Tool Settings

- `+` - Increase brush size
- `-` - Decrease brush size

### Layer Management

- `n [name]` - Create a new layer with optional name
- `l` - List all layers
- `s index` - Select layer by index

### Canvas Operations

- `p x y` - Paint at position (x,y) using the current tool
- `c` - Clear current layer
- `v` - View the canvas with all visible layers

### Application Controls

- `h` - Show help
- `q` - Quit the application

## Workflow Examples

### Basic Painting

1. Start the application
2. Use the brush tool (selected by default)
3. Paint on the canvas using `p 10 5`
4. Increase brush size with `+`
5. Paint more areas
6. View the canvas with `v`

### Using Multiple Layers

1. Create a base layer (created by default)
2. Paint the background
3. Create a new layer with `n Details`
4. Select the LineArt tool with `3`
5. Add details to your painting
6. Switch between layers using `s 0` and `s 1`
7. Clear a layer if needed with `c`

## Advanced Features

### Demo Mode

Run the application with the `--demo` argument to see a demonstration of the painting features:

```
./3DModelPainter --demo
```

## Troubleshooting

If you encounter any issues with the application, try the following:

1. Make sure you have the correct permissions to run the executable
2. Check that your terminal supports the text-based interface
3. Try running with the `--demo` flag to test basic functionality

## Future Development

Future versions of the application will include:

- Full 3D OpenGL rendering
- Support for loading OBJ and FBX models
- PBR material support
- UV mapping tools
- Export functionality