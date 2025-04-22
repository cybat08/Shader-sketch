# 3D Model Painter - Color Edition

A colorful terminal-based 3D model painting application with enhanced visual features.

![3D Model Painter](screenshot.png)

## Features

- Interactive painting tools with full ANSI color support
- Multiple brush types for different artistic effects
- Layer-based editing with visibility controls
- Intuitive command-line interface with color-coded feedback
- Demonstration mode showing the application's capabilities

## Quick Start

```bash
# Run in interactive mode
./ColorModelPainter

# Run the demonstration
./ColorModelPainter --demo
```

## Tools

- **Brush**: Standard painting tool
- **Eraser**: Remove content from layers
- **LineArt**: Create line-based drawings
- **DotBrush**: Create dot patterns
- **StarBrush**: Create highlight points

## Commands

| Command | Description |
|---------|-------------|
| `1-5` | Select tool (Brush, Eraser, LineArt, DotBrush, StarBrush) |
| `+`, `-` | Increase/decrease brush size |
| `p x y` | Paint at coordinates (x,y) |
| `c` | Clear current layer |
| `n [name]` | New layer with optional name |
| `l` | List all layers |
| `s index` | Select layer by index |
| `t index` | Toggle layer visibility |
| `v` | View the canvas |
| `h` | Show help |
| `q` | Quit application |

## Building from Source

```bash
g++ -std=c++17 -o ColorModelPainter ColorModelPainter.cpp
```

## Requirements

- C++17 compatible compiler
- Terminal with ANSI color support

## License

MIT License - See LICENSE file for details

## Acknowledgments

- Created as a demonstration of terminal-based graphics capabilities
- Special thanks to the contributors of the original 3D Model Painter