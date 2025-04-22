# Color Wheel with Harmony Suggestions

The 3D Model Painter includes a powerful color wheel with harmony suggestions, helping artists choose aesthetically pleasing color combinations based on color theory principles.

## Using the Color Wheel

To access the color wheel in the application:

1. Press `w` in the main application interface
2. Use keyboard controls to navigate and select colors
3. Apply selected colors to your painting tools

## Color Wheel Interface

```
======== COLOR WHEEL ========
            R G B
Current: [255,0,0] #FF0000
         [HSV: 0°, 100%, 100%]

    Y
   / \
  /   \
 G     R
 |     |
 C-----M
 |     |
 B     M
  \   /
   \ /
    W

Harmony: Complementary
1: [255,0,0] #FF0000 ■ 
2: [0,255,255] #00FFFF ■

Controls:
h/H - Adjust hue
s/S - Adjust saturation
v/V - Adjust value
n - Next harmony type
p - Previous harmony type
1-5 - Select color from harmony
r - Reset to default
q - Quit color wheel

==============================
```

## Color Harmony Types

The color wheel supports various harmony types:

### 1. Complementary
Two colors directly opposite each other on the color wheel, creating maximum contrast.
```
Example: Red (#FF0000) and Cyan (#00FFFF)
```

### 2. Analogous
Three colors that are side by side on the color wheel, creating a harmonious and seamless blend.
```
Example: Red (#FF0000), Orange-Red (#FF4000), Red-Purple (#FF0040)
```

### 3. Triadic
Three colors evenly spaced around the color wheel, creating a balanced and vibrant color scheme.
```
Example: Red (#FF0000), Green (#00FF00), Blue (#0000FF)
```

### 4. Split-Complementary
A variation of complementary harmony with one color and two adjacent colors to its complement.
```
Example: Red (#FF0000), Blue-Cyan (#00FFFF), Yellow-Green (#00FF80)
```

### 5. Square
Four colors evenly spaced around the color wheel, creating a dynamic but balanced color scheme.
```
Example: Red (#FF0000), Yellow-Green (#80FF00), Cyan (#00FFFF), Violet (#8000FF)
```

### 6. Rectangular (Tetradic)
Two complementary color pairs, creating a rich color scheme with many possibilities.
```
Example: Red (#FF0000), Orange-Yellow (#FFAA00), Cyan (#00FFFF), Blue-Violet (#0055FF)
```

### 7. Monochromatic
Different tints, tones, and shades of a single color, creating a subtle and sophisticated look.
```
Example: Red (#FF0000), Light Red (#FF8080), Dark Red (#800000)
```

## Color Models

The color wheel simultaneously displays colors in multiple formats:

- **RGB (Red, Green, Blue)**: For digital media and screen display
- **HSV (Hue, Saturation, Value)**: For intuitive color selection
- **Hex**: For web development and digital design

## Technical Implementation

The color wheel is implemented in `color_wheel.h` and operates within its own namespace to avoid conflicts with the main application's color handling.

### HSV to RGB Conversion

The color wheel uses the HSV color model internally, with conversions to RGB for display and application:

```cpp
// Convert HSV to RGB
RGB hsv_to_rgb(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    
    float r, g, b;
    if (h >= 0 && h < 60) { r = c; g = x; b = 0; }
    else if (h >= 60 && h < 120) { r = x; g = c; b = 0; }
    else if (h >= 120 && h < 180) { r = 0; g = c; b = x; }
    else if (h >= 180 && h < 240) { r = 0; g = x; b = c; }
    else if (h >= 240 && h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }
    
    return {
        static_cast<int>((r + m) * 255),
        static_cast<int>((g + m) * 255),
        static_cast<int>((b + m) * 255)
    };
}
```

### Color Harmonies

The harmony system calculates related colors based on color theory rules:

```cpp
// Get complementary color (180° opposite on the wheel)
HSV get_complementary(HSV base) {
    float h = std::fmod(base.h + 180.0f, 360.0f);
    return {h, base.s, base.v};
}

// Get analogous colors (30° to each side)
std::vector<HSV> get_analogous(HSV base) {
    float h1 = std::fmod(base.h - 30.0f + 360.0f, 360.0f);
    float h2 = std::fmod(base.h + 30.0f, 360.0f);
    return {base, {h1, base.s, base.v}, {h2, base.s, base.v}};
}

// Get triadic colors (120° apart)
std::vector<HSV> get_triadic(HSV base) {
    float h1 = std::fmod(base.h + 120.0f, 360.0f);
    float h2 = std::fmod(base.h + 240.0f, 360.0f);
    return {base, {h1, base.s, base.v}, {h2, base.s, base.v}};
}
```

## Integration with Painting Tools

The color wheel is tightly integrated with the painting tools in the application. When you select a color from the wheel, it becomes the active color for your current tool. This allows for:

1. Rapid color switching while maintaining harmony
2. Creation of professional color schemes
3. Experimentation with different color theories
4. Educational exploration of color relationships

## Example Usage

1. **Creating a Landscape**:
   - Use analogous harmony for sky gradients
   - Use complementary harmony for dramatic sunset effects
   - Use split-complementary for foliage and terrain

2. **Character Design**:
   - Use triadic harmony for balanced character color schemes
   - Use square harmony for team/group character designs

3. **Abstract Art**:
   - Use rectangular harmony for complex color relationships
   - Use monochromatic harmony for subtle depth and texture

The color wheel is a powerful tool for both beginners learning color theory and professionals seeking efficient color selection workflows.