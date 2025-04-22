# 3D Model Painter v0.2.0 Release Notes

This release introduces the enhanced colorful version of the 3D Model Painter with significant visual improvements and additional features.

## What's New

- **ANSI Color Support**: Full color interface with highlighted commands and visual elements
- **Enhanced UI**: Improved canvas display with coordinate grid and borders
- **New Tools**: Added DotBrush and StarBrush tools for more creative options
- **Extended Color Palette**: More color options with intuitive selection
- **Visual Feedback**: Better visual feedback for all operations
- **Improved Layer Management**: Enhanced layer display with selection highlighting
- **Interactive Demo**: Added demonstration mode showing advanced capabilities

## System Requirements

- C++17 compatible compiler
- Terminal with ANSI color support
- 50+ column width terminal recommended for best display

## Installation

Simply download the release package, extract it, and run the ColorModelPainter executable:

```
$ ./ColorModelPainter         # Interactive mode
$ ./ColorModelPainter --demo  # Demonstration mode
```

## Usage Tips

1. Use the numeric keys (1-5) to select different tools
2. The 'p x y' command lets you paint at specific coordinates
3. Try the demo mode to see a showcase of available features
4. Use 'c' followed by a number to change colors (e.g., 'c1' for red)

## Known Issues

- Some special characters may not display correctly in all terminals
- Canvas size is fixed at 30x15 pixels

## Coming Soon

- OpenGL-based 3D rendering
- File save/load capability
- Custom brush creation
- Multiple models support

## Feedback

We welcome your feedback and suggestions for future improvements!