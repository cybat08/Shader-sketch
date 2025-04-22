# Export Formats in 3D Model Painter

The 3D Model Painter supports exporting your projects in multiple formats, each with different advantages and use cases.

## Supported Export Formats

### 1. OBJ (.obj)

The Wavefront OBJ format is a standard 3D model format that includes geometry, UV mapping, and texture references.

**Features:**
- Full 3D model export with UV map
- Material properties via associated MTL file
- Industry-standard compatibility
- Preserves all painting and texture work

**Usage:**
```
x
Select format: obj
Enter filename: mymodel
```

This creates:
- `mymodel.obj` - The 3D model file
- `mymodel.mtl` - The material library file

**Best for:** Importing into other 3D software or game engines.

### 2. SVG (.svg)

Scalable Vector Graphics format exports the unwrapped texture as a vector image.

**Features:**
- Infinitely scalable vector format
- Preserves sharp lines and details
- Editable in vector graphics software
- Small file size

**Usage:**
```
x
Select format: svg
Enter filename: mytexture
```

**Best for:** Further editing in vector graphics software or using in web/print materials.

### 3. PPM (.ppm)

Portable Pixmap format provides a simple, uncompressed raster image of your texture.

**Features:**
- Simple, human-readable format
- Full color information preserved
- No compression artifacts
- Easy to parse programmatically

**Usage:**
```
x
Select format: ppm
Enter filename: mytexture
```

**Best for:** Scientific use, algorithm development, or when maximum compatibility is needed.

### 4. TXT (.txt)

Text format exports a plain text ASCII art representation of your texture.

**Features:**
- Human-readable format
- Works in any text editor
- Perfect for sharing simple previews
- Unique artistic style

**Usage:**
```
x
Select format: txt
Enter filename: mytexture
```

**Best for:** Simple previews, sharing on text-only platforms, or retro ASCII art style.

### 5. FBX (.fbx)

*Note: FBX export is only available in the full 3D version with OpenGL support.*

Autodesk FBX format is an industry-standard 3D model format with extensive features.

**Features:**
- Professional 3D production format
- Preserves hierarchies, animations, and materials
- Wide software compatibility
- Ideal for commercial 3D pipelines

**Usage:**
```
x
Select format: fbx
Enter filename: mymodel
```

**Best for:** Professional 3D workflows, game development, and animation.

## Export Options

When exporting, you can specify additional options:

### General Options

- **Resolution** - For raster formats (PPM), you can specify output resolution
- **Color Mode** - Choose between full color, grayscale, or indexed color
- **Layer Handling** - Export all layers or only visible/selected layers

### Format-Specific Options

#### OBJ Export Options
- **Include Materials** - Creates an associated MTL file
- **Triangulate Faces** - Converts quads to triangles for maximum compatibility

#### SVG Export Options
- **Outline Only** - Creates vector outlines instead of filled shapes
- **Optimize Paths** - Reduces complexity by combining similar paths

## Command Line Export

You can also export directly using the command line version of the application:

```bash
./EnhancedModelPainter --export format=obj input.txt output.obj
```

For batch processing:

```bash
./EnhancedModelPainter --batch-export format=ppm input_dir output_dir
```