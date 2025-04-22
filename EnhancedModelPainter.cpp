#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <sstream>

// Color wheel with harmony suggestions
#include "color_wheel.h"
#include "color_wheel_app.cpp"

// Windows-specific headers
#ifdef _WIN32
#include <windows.h>
#endif

// ANSI color codes for terminal output
#define RESET         "\033[0m"
#define BOLD          "\033[1m"
#define FG_BLACK      "\033[30m"
#define FG_RED        "\033[31m"
#define FG_GREEN      "\033[32m"
#define FG_YELLOW     "\033[33m"
#define FG_BLUE       "\033[34m"
#define FG_MAGENTA    "\033[35m"
#define FG_CYAN       "\033[36m"
#define FG_WHITE      "\033[37m"
#define FG_BRIGHT_BLACK   "\033[90m"
#define FG_BRIGHT_RED     "\033[91m"
#define FG_BRIGHT_GREEN   "\033[92m"
#define FG_BRIGHT_YELLOW  "\033[93m"
#define FG_BRIGHT_BLUE    "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN    "\033[96m"
#define FG_BRIGHT_WHITE   "\033[97m"
#define BG_BLACK      "\033[40m"
#define BG_RED        "\033[41m"
#define BG_GREEN      "\033[42m"
#define BG_YELLOW     "\033[43m"
#define BG_BLUE       "\033[44m"
#define BG_MAGENTA    "\033[45m"
#define BG_CYAN       "\033[46m"
#define BG_WHITE      "\033[47m"
#define BG_BRIGHT_BLUE    "\033[104m"

// Utility functions
void clearScreen() {
#ifdef _WIN32
    // Windows-specific clear screen
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    // ANSI escape code (works on Linux, macOS, and Windows 10+ with ANSI support)
    std::cout << "\033[2J\033[1;1H";
#endif
}

// Function to enable ANSI colors in Windows console
void setupConsole() {
#ifdef _WIN32
    // Enable ANSI escape sequences on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void animateText(const std::string& text, int delay = 50) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

// Forward declarations
class Texture;
class Layer;
class PaintTool;
class Model;
class Project;
class Application;

// Color representation
struct Color {
    std::string symbol;  // Symbol to represent color in text mode
    std::string ansiCode; // ANSI color code
    std::string name;     // Color name

    Color() : symbol("*"), ansiCode(FG_WHITE), name("Default") {}
    Color(const std::string& s, const std::string& a, const std::string& n) : symbol(s), ansiCode(a), name(n) {}
};

// HSV and RGB color models for color wheel
struct HSV {
    float h;  // Hue [0, 360)
    float s;  // Saturation [0, 1]
    float v;  // Value [0, 1]
    
    HSV() : h(0.0f), s(0.0f), v(0.0f) {}
    HSV(float h, float s, float v) : h(h), s(s), v(v) {}
    
    // Forward declaration for RGB conversion
    struct RGB toRGB() const;
    
    // Color harmonies
    HSV complementary() const {
        return HSV(std::fmod(h + 180.0f, 360.0f), s, v);
    }
    
    std::vector<HSV> analogous(float angle = 30.0f) const {
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(h + angle, 360.0f), s, v));
        colors.push_back(HSV(std::fmod(h - angle + 360.0f, 360.0f), s, v));
        return colors;
    }
    
    std::vector<HSV> triadic() const {
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(h + 120.0f, 360.0f), s, v));
        colors.push_back(HSV(std::fmod(h + 240.0f, 360.0f), s, v));
        return colors;
    }
    
    std::vector<HSV> splitComplementary(float angle = 30.0f) const {
        HSV comp = complementary();
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(comp.h + angle, 360.0f), s, v));
        colors.push_back(HSV(std::fmod(comp.h - angle + 360.0f, 360.0f), s, v));
        return colors;
    }
    
    std::vector<HSV> tetradic() const {
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(h + 90.0f, 360.0f), s, v));
        colors.push_back(HSV(std::fmod(h + 180.0f, 360.0f), s, v));
        colors.push_back(HSV(std::fmod(h + 270.0f, 360.0f), s, v));
        return colors;
    }
};

struct RGB {
    float r;  // Red [0, 1]
    float g;  // Green [0, 1]
    float b;  // Blue [0, 1]
    
    RGB() : r(0.0f), g(0.0f), b(0.0f) {}
    RGB(float r, float g, float b) : r(r), g(g), b(b) {}
    
    HSV toHSV() const {
        float max = std::max({r, g, b});
        float min = std::min({r, g, b});
        float delta = max - min;
        
        HSV result;
        result.v = max;
        
        if (max == 0.0f) {
            result.s = 0.0f;
            result.h = 0.0f;  // Undefined, but we default to 0
            return result;
        }
        
        result.s = delta / max;
        
        if (delta == 0.0f) {
            result.h = 0.0f;  // Achromatic (gray)
            return result;
        }
        
        if (max == r) {
            result.h = 60.0f * std::fmod(((g - b) / delta), 6.0f);
        } else if (max == g) {
            result.h = 60.0f * (((b - r) / delta) + 2.0f);
        } else {
            result.h = 60.0f * (((r - g) / delta) + 4.0f);
        }
        
        if (result.h < 0.0f) result.h += 360.0f;
        
        return result;
    }
    
    std::string toAnsiColor() const {
        // Convert to 8-bit color
        int r8 = std::min(5, static_cast<int>(r * 6));
        int g8 = std::min(5, static_cast<int>(g * 6));
        int b8 = std::min(5, static_cast<int>(b * 6));
        
        // Use 256-color ANSI escape sequence
        int code = 16 + (36 * r8) + (6 * g8) + b8;
        return "\033[38;5;" + std::to_string(code) + "m";
    }
    
    std::string getColorName() const {
        // Simple color naming based on RGB values
        if (r > 0.8f && g < 0.3f && b < 0.3f) return "Red";
        if (r > 0.8f && g > 0.8f && b < 0.3f) return "Yellow";
        if (r < 0.3f && g > 0.8f && b < 0.3f) return "Green";
        if (r < 0.3f && g < 0.3f && b > 0.8f) return "Blue";
        if (r > 0.8f && g < 0.3f && b > 0.8f) return "Magenta";
        if (r < 0.3f && g > 0.8f && b > 0.8f) return "Cyan";
        if (r > 0.8f && g > 0.5f && b < 0.3f) return "Orange";
        if (r > 0.5f && g < 0.3f && b > 0.5f) return "Purple";
        if (r > 0.8f && g > 0.8f && b > 0.8f) return "White";
        if (r < 0.2f && g < 0.2f && b < 0.2f) return "Black";
        return "Custom";
    }
};

// Implementation of HSV to RGB conversion
RGB HSV::toRGB() const {
    if (s <= 0.0f) {
        return RGB(v, v, v);  // Achromatic (gray)
    }
    
    float hh = std::fmod(h, 360.0f) / 60.0f;
    int i = static_cast<int>(hh);
    float ff = hh - i;
    
    float p = v * (1.0f - s);
    float q = v * (1.0f - (s * ff));
    float t = v * (1.0f - (s * (1.0f - ff)));
    
    switch (i) {
        case 0: return RGB(v, t, p);
        case 1: return RGB(q, v, p);
        case 2: return RGB(p, v, t);
        case 3: return RGB(p, q, v);
        case 4: return RGB(t, p, v);
        default: return RGB(v, p, q);
    }
}

// Texture class to store pixel data
class Texture {
public:
    Texture(int width = 30, int height = 15) : width(width), height(height) {
        pixels.resize(height);
        for (auto& row : pixels) {
            row.resize(width);
        }
        
        std::cout << FG_CYAN << "✓ " << "Created texture: " << width << "x" << height << RESET << std::endl;
    }
    
    void clear() {
        for (auto& row : pixels) {
            std::fill(row.begin(), row.end(), Color());
        }
    }
    
    void paint(int x, int y, const Color& color, float size = 1.0f, float opacity = 1.0f) {
        if (!isValidCoordinate(x, y)) return;
        
        // Simple circle brush based on size
        int radius = static_cast<int>(size);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                float distance = std::sqrt(dx*dx + dy*dy);
                if (distance <= size) {
                    int newX = x + dx;
                    int newY = y + dy;
                    if (isValidCoordinate(newX, newY)) {
                        pixels[newY][newX] = color;
                    }
                }
            }
        }
    }
    
    const Color& getPixel(int x, int y) const {
        static const Color emptyColor;
        if (!isValidCoordinate(x, y)) return emptyColor;
        return pixels[y][x];
    }
    
    void drawLine(int x1, int y1, int x2, int y2, const Color& color) {
        // Bresenham's line algorithm
        int dx = std::abs(x2 - x1);
        int dy = -std::abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;
        
        while (true) {
            if (isValidCoordinate(x1, y1)) {
                pixels[y1][x1] = color;
            }
            
            if (x1 == x2 && y1 == y2) break;
            
            int e2 = 2 * err;
            if (e2 >= dy) {
                if (x1 == x2) break;
                err += dy;
                x1 += sx;
            }
            if (e2 <= dx) {
                if (y1 == y2) break;
                err += dx;
                y1 += sy;
            }
        }
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    bool isValidCoordinate(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    int width;
    int height;
    std::vector<std::vector<Color>> pixels;
    
    friend class Layer;
};

// Layer class to manage a texture with opacity and visibility
class Layer {
public:
    Layer(const std::string& name, int width = 30, int height = 15) 
        : name(name), visible(true), opacity(1.0f), texture(width, height) {
        std::cout << FG_GREEN << "✓ " << "Created layer: " << name << RESET << std::endl;
    }
    
    void render(bool showCoords = false) const {
        if (!visible) return;
        
        // Print coordinates header
        if (showCoords) {
            std::cout << "            0        10        20" << std::endl;
            std::cout << "   012345678901234567890123456789" << std::endl;
        }
        
        // Print the top border
        std::cout << "  +------------------------------+" << std::endl;
        
        // Print each row with left-side line numbers if requested
        for (int y = 0; y < texture.getHeight(); y++) {
            if (showCoords) {
                std::cout << std::setw(2) << y << "|";
            } else {
                std::cout << "  |";
            }
            
            for (int x = 0; x < texture.getWidth(); x++) {
                const Color& color = texture.getPixel(x, y);
                std::cout << color.ansiCode << color.symbol << RESET;
            }
            
            std::cout << "|" << std::endl;
        }
        
        // Print the bottom border
        std::cout << "  +------------------------------+" << std::endl;
    }
    
    void printInfo(bool selected = false) const {
        if (visible) {
            if (selected) {
                std::cout << BOLD << "[SELECTED]  " << FG_BRIGHT_GREEN << "● " << name << RESET << std::endl;
            } else {
                std::cout << "  " << FG_GREEN << "● " << name << RESET << std::endl;
            }
        } else {
            std::cout << FG_RED << "○ " << name << " (hidden)" << RESET << std::endl;
        }
    }
    
    const std::string& getName() const { return name; }
    bool isVisible() const { return visible; }
    float getOpacity() const { return opacity; }
    void setVisible(bool visible) { this->visible = visible; }
    void setOpacity(float opacity) { this->opacity = opacity; }
    void setName(const std::string& newName) { this->name = newName; }
    
    // Get reference to the layer's texture
    Texture& getTexture() { return texture; }
    
    // Layer public methods
    void clear() {
        texture.clear();
    }
    
private:
    std::string name;
    bool visible;
    float opacity;
    Texture texture;
};

// 3D model data structures
struct Vertex {
    float x, y, z;
    
    Vertex() : x(0), y(0), z(0) {}
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct UV {
    float u, v;
    
    UV() : u(0), v(0) {}
    UV(float u, float v) : u(u), v(v) {}
};

struct Face {
    std::vector<int> vertexIndices;       // Indices to vertices
    std::vector<int> uvIndices;           // Indices to UV coordinates
    std::vector<int> normalIndices;       // Indices to normals
    
    Face() {}
    
    // Helper to add a vertex-UV-normal triplet to the face
    void addVertex(int vIdx, int uvIdx, int nIdx) {
        vertexIndices.push_back(vIdx);
        uvIndices.push_back(uvIdx);
        normalIndices.push_back(nIdx);
    }
};

// Enhanced Model class with UV mapping support
class Model {
public:
    Model() : loaded(false), modelName("Untitled") {
        std::cout << FG_MAGENTA << "✓ " << "Model created" << RESET << std::endl;
    }
    
    bool loadModel(const std::string& path) {
        std::cout << FG_MAGENTA << "⧗ " << "Loading model from: " << path << " ";
        
        // Show spinning animation
        const std::string spinChars = "|/-\\";
        for (int i = 0; i < 10; i++) {
            std::cout << FG_BRIGHT_YELLOW << spinChars[i % spinChars.size()] << "\b" << RESET << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Extract model name from path
        size_t lastSlash = path.find_last_of("/\\");
        size_t lastDot = path.find_last_of(".");
        if (lastSlash == std::string::npos) lastSlash = 0; else lastSlash++;
        if (lastDot == std::string::npos) lastDot = path.length();
        modelName = path.substr(lastSlash, lastDot - lastSlash);
        
        // For demo purposes, create a simple cube model with UVs
        createDemoModel();
        
        loaded = true;
        std::cout << FG_BRIGHT_GREEN << "✓" << RESET << std::endl;
        return true;
    }
    
    // Create a demo cube model with UV coordinates
    void createDemoModel() {
        // Clear existing data
        vertices.clear();
        uvs.clear();
        faces.clear();
        
        // Create a simple cube
        // Front face
        vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f));  // 0: bottom-left-front
        vertices.push_back(Vertex(1.0f, -1.0f, 1.0f));   // 1: bottom-right-front
        vertices.push_back(Vertex(1.0f, 1.0f, 1.0f));    // 2: top-right-front
        vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f));   // 3: top-left-front
        
        // Back face
        vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f)); // 4: bottom-left-back
        vertices.push_back(Vertex(1.0f, -1.0f, -1.0f));  // 5: bottom-right-back
        vertices.push_back(Vertex(1.0f, 1.0f, -1.0f));   // 6: top-right-back
        vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f));  // 7: top-left-back
        
        // Create UV coordinates for a basic cube unwrap
        // Each face gets a portion of the UV space
        // Front face UVs
        uvs.push_back(UV(0.0f, 0.0f));    // 0: bottom-left
        uvs.push_back(UV(0.25f, 0.0f));   // 1: bottom-right
        uvs.push_back(UV(0.25f, 0.25f));  // 2: top-right
        uvs.push_back(UV(0.0f, 0.25f));   // 3: top-left
        
        // Back face UVs
        uvs.push_back(UV(0.25f, 0.0f));   // 4: bottom-left
        uvs.push_back(UV(0.5f, 0.0f));    // 5: bottom-right
        uvs.push_back(UV(0.5f, 0.25f));   // 6: top-right
        uvs.push_back(UV(0.25f, 0.25f));  // 7: top-left
        
        // Left face UVs
        uvs.push_back(UV(0.0f, 0.25f));   // 8: bottom-left
        uvs.push_back(UV(0.25f, 0.25f));  // 9: bottom-right
        uvs.push_back(UV(0.25f, 0.5f));   // 10: top-right
        uvs.push_back(UV(0.0f, 0.5f));    // 11: top-left
        
        // Right face UVs
        uvs.push_back(UV(0.25f, 0.25f));  // 12: bottom-left
        uvs.push_back(UV(0.5f, 0.25f));   // 13: bottom-right
        uvs.push_back(UV(0.5f, 0.5f));    // 14: top-right
        uvs.push_back(UV(0.25f, 0.5f));   // 15: top-left
        
        // Top face UVs
        uvs.push_back(UV(0.0f, 0.5f));    // 16: bottom-left
        uvs.push_back(UV(0.25f, 0.5f));   // 17: bottom-right
        uvs.push_back(UV(0.25f, 0.75f));  // 18: top-right
        uvs.push_back(UV(0.0f, 0.75f));   // 19: top-left
        
        // Bottom face UVs
        uvs.push_back(UV(0.25f, 0.5f));   // 20: bottom-left
        uvs.push_back(UV(0.5f, 0.5f));    // 21: bottom-right
        uvs.push_back(UV(0.5f, 0.75f));   // 22: top-right
        uvs.push_back(UV(0.25f, 0.75f));  // 23: top-left
        
        // Create faces with proper UV indices
        Face frontFace;
        frontFace.addVertex(0, 0, 0);
        frontFace.addVertex(1, 1, 0);
        frontFace.addVertex(2, 2, 0);
        frontFace.addVertex(3, 3, 0);
        faces.push_back(frontFace);
        
        Face backFace;
        backFace.addVertex(5, 5, 0);
        backFace.addVertex(4, 4, 0);
        backFace.addVertex(7, 7, 0);
        backFace.addVertex(6, 6, 0);
        faces.push_back(backFace);
        
        Face leftFace;
        leftFace.addVertex(4, 8, 0);
        leftFace.addVertex(0, 9, 0);
        leftFace.addVertex(3, 10, 0);
        leftFace.addVertex(7, 11, 0);
        faces.push_back(leftFace);
        
        Face rightFace;
        rightFace.addVertex(1, 12, 0);
        rightFace.addVertex(5, 13, 0);
        rightFace.addVertex(6, 14, 0);
        rightFace.addVertex(2, 15, 0);
        faces.push_back(rightFace);
        
        Face topFace;
        topFace.addVertex(3, 16, 0);
        topFace.addVertex(2, 17, 0);
        topFace.addVertex(6, 18, 0);
        topFace.addVertex(7, 19, 0);
        faces.push_back(topFace);
        
        Face bottomFace;
        bottomFace.addVertex(4, 20, 0);
        bottomFace.addVertex(5, 21, 0);
        bottomFace.addVertex(1, 22, 0);
        bottomFace.addVertex(0, 23, 0);
        faces.push_back(bottomFace);
    }
    
    // Generate a UV map image of the current unwrapping
    void generateUVMap(Texture& texture) const {
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // Clear the texture
        texture.clear();
        
        // Draw UV layout in the texture
        // Use different colors for each face
        Color faceColors[6] = {
            Color("█", FG_BRIGHT_RED, "Red"),
            Color("█", FG_BRIGHT_GREEN, "Green"),
            Color("█", FG_BRIGHT_BLUE, "Blue"),
            Color("█", FG_BRIGHT_YELLOW, "Yellow"),
            Color("█", FG_BRIGHT_MAGENTA, "Magenta"),
            Color("█", FG_BRIGHT_CYAN, "Cyan")
        };
        
        // Draw each face with its UV coordinates
        for (size_t f = 0; f < faces.size(); f++) {
            const Face& face = faces[f];
            const Color& faceColor = faceColors[f % 6];
            
            // Draw the face outlines in the UV texture
            for (size_t i = 0; i < face.vertexIndices.size(); i++) {
                int nextI = (i + 1) % face.vertexIndices.size();
                int uvIdx1 = face.uvIndices[i];
                int uvIdx2 = face.uvIndices[nextI];
                
                // Get UV coordinates
                const UV& uv1 = uvs[uvIdx1];
                const UV& uv2 = uvs[uvIdx2];
                
                // Convert to texture coordinates
                int x1 = static_cast<int>(uv1.u * (width - 1));
                int y1 = static_cast<int>((1.0f - uv1.v) * (height - 1)); // flip y for texture space
                int x2 = static_cast<int>(uv2.u * (width - 1));
                int y2 = static_cast<int>((1.0f - uv2.v) * (height - 1));
                
                // Draw a line between the points
                texture.drawLine(x1, y1, x2, y2, faceColor);
            }
            
            // Fill the face with a semi-transparent color
            // Find the center of the face in UV space
            float centerU = 0.0f, centerV = 0.0f;
            for (size_t i = 0; i < face.uvIndices.size(); i++) {
                const UV& uv = uvs[face.uvIndices[i]];
                centerU += uv.u;
                centerV += uv.v;
            }
            centerU /= face.uvIndices.size();
            centerV /= face.uvIndices.size();
            
            // Convert to texture coordinates
            int centerX = static_cast<int>(centerU * (width - 1));
            int centerY = static_cast<int>((1.0f - centerV) * (height - 1));
            
            // Draw a small marker at the center
            texture.paint(centerX, centerY, faceColor, 0.5f, 0.7f);
        }
    }
    
    // Apply a simple planar UV unwrapping on a selected face
    void applyPlanarUnwrap(int faceIndex, float scaleU = 1.0f, float scaleV = 1.0f, float offsetU = 0.0f, float offsetV = 0.0f) {
        if (faceIndex < 0 || faceIndex >= static_cast<int>(faces.size())) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Invalid face index for UV unwrapping" << RESET << std::endl;
            return;
        }
        
        Face& face = faces[faceIndex];
        
        // Find the normal of the face
        int v0Idx = face.vertexIndices[0];
        int v1Idx = face.vertexIndices[1];
        int v2Idx = face.vertexIndices[2];
        
        Vertex v0 = vertices[v0Idx];
        Vertex v1 = vertices[v1Idx];
        Vertex v2 = vertices[v2Idx];
        
        // Compute face normal using cross product
        float ux = v1.x - v0.x;
        float uy = v1.y - v0.y;
        float uz = v1.z - v0.z;
        
        float vx = v2.x - v0.x;
        float vy = v2.y - v0.y;
        float vz = v2.z - v0.z;
        
        float nx = uy * vz - uz * vy;
        float ny = uz * vx - ux * vz;
        float nz = ux * vy - uy * vx;
        
        // Normalize
        float len = std::sqrt(nx*nx + ny*ny + nz*nz);
        if (len > 0.0001f) {
            nx /= len;
            ny /= len;
            nz /= len;
        }
        
        // Find the axis with the largest component to project onto
        float absX = std::abs(nx);
        float absY = std::abs(ny);
        float absZ = std::abs(nz);
        
        // Determine projection axes
        int uAxis = 0, vAxis = 1;
        if (absZ > absX && absZ > absY) {
            // Project onto XY plane
            uAxis = 0; vAxis = 1;
        } else if (absY > absX) {
            // Project onto XZ plane
            uAxis = 0; vAxis = 2;
        } else {
            // Project onto YZ plane
            uAxis = 1; vAxis = 2;
        }
        
        // Apply planar unwrapping to face vertices
        for (size_t i = 0; i < face.vertexIndices.size(); i++) {
            Vertex& v = vertices[face.vertexIndices[i]];
            
            // Get the appropriate coordinates based on projection
            float u = 0.0f, v_ = 0.0f;
            
            if (uAxis == 0) u = v.x;
            else if (uAxis == 1) u = v.y;
            else u = v.z;
            
            if (vAxis == 0) v_ = v.x;
            else if (vAxis == 1) v_ = v.y;
            else v_ = v.z;
            
            // Apply scale and offset
            u = u * scaleU + offsetU;
            v_ = v_ * scaleV + offsetV;
            
            // Scale to [0,1] range (simple normalization)
            u = (u + 1.0f) * 0.5f;
            v_ = (v_ + 1.0f) * 0.5f;
            
            // Create a new UV coordinate and add it to the list
            uvs.push_back(UV(u, v_));
            
            // Update the face to use the new UV
            if (i < face.uvIndices.size()) {
                face.uvIndices[i] = uvs.size() - 1;
            } else {
                face.uvIndices.push_back(uvs.size() - 1);
            }
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Applied planar unwrapping to face " << faceIndex << RESET << std::endl;
    }
    
    // Automatically unwrap all faces
    void unwrapUVs() {
        // Apply planar unwrapping to each face
        for (size_t i = 0; i < faces.size(); i++) {
            applyPlanarUnwrap(i);
        }
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Applied automatic unwrapping to all faces" << RESET << std::endl;
    }
    
    // Export model to a text file
    bool exportToText(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return false;
        }
        
        file << "# 3D Model Painter Export - " << modelName << std::endl;
        file << "# Vertices: " << vertices.size() << std::endl;
        file << "# UVs: " << uvs.size() << std::endl;
        file << "# Faces: " << faces.size() << std::endl;
        file << std::endl;
        
        // Write vertices
        file << "# Vertices (x, y, z)" << std::endl;
        for (const auto& vertex : vertices) {
            file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }
        file << std::endl;
        
        // Write UVs
        file << "# UV Coordinates (u, v)" << std::endl;
        for (const auto& uv : uvs) {
            file << "vt " << uv.u << " " << uv.v << std::endl;
        }
        file << std::endl;
        
        // Write faces
        file << "# Faces (vertex_index/uv_index)" << std::endl;
        for (size_t i = 0; i < faces.size(); i++) {
            const Face& face = faces[i];
            file << "f ";
            for (size_t j = 0; j < face.vertexIndices.size(); j++) {
                // OBJ format is 1-indexed, so add 1 to all indices
                file << (face.vertexIndices[j] + 1) << "/" << (face.uvIndices[j] + 1);
                if (j < face.vertexIndices.size() - 1) {
                    file << " ";
                }
            }
            file << std::endl;
        }
        
        file.close();
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Model exported to: " << filename << RESET << std::endl;
        return true;
    }
    
    // Export model to OBJ format
    bool exportToOBJ(const std::string& filename) const {
        std::ofstream objFile(filename);
        if (!objFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return false;
        }
        
        // Create an MTL file with same base name
        std::string mtlFilename = filename;
        size_t extension = mtlFilename.find_last_of(".");
        if (extension != std::string::npos) {
            mtlFilename = mtlFilename.substr(0, extension) + ".mtl";
        } else {
            mtlFilename += ".mtl";
        }
        
        std::ofstream mtlFile(mtlFilename);
        if (!mtlFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open MTL file for writing: " << mtlFilename << RESET << std::endl;
            objFile.close();
            return false;
        }
        
        // Write OBJ header
        objFile << "# 3D Model Painter Export - " << modelName << std::endl;
        objFile << "mtllib " << mtlFilename.substr(mtlFilename.find_last_of("/\\") + 1) << std::endl;
        objFile << "o " << modelName << std::endl;
        objFile << std::endl;
        
        // Write vertices
        for (const auto& vertex : vertices) {
            objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }
        objFile << std::endl;
        
        // Write UVs
        for (const auto& uv : uvs) {
            objFile << "vt " << uv.u << " " << uv.v << std::endl;
        }
        objFile << std::endl;
        
        // Write faces with material assignments
        objFile << "usemtl material1" << std::endl;
        for (const Face& face : faces) {
            objFile << "f ";
            for (size_t i = 0; i < face.vertexIndices.size(); i++) {
                // OBJ format is 1-indexed, so add 1 to all indices
                objFile << (face.vertexIndices[i] + 1) << "/" << (face.uvIndices[i] + 1);
                if (i < face.vertexIndices.size() - 1) {
                    objFile << " ";
                }
            }
            objFile << std::endl;
        }
        
        // Write MTL file
        mtlFile << "# 3D Model Painter Material" << std::endl;
        mtlFile << "newmtl material1" << std::endl;
        mtlFile << "Ka 1.000 1.000 1.000" << std::endl;  // Ambient color
        mtlFile << "Kd 0.800 0.800 0.800" << std::endl;  // Diffuse color
        mtlFile << "Ks 0.000 0.000 0.000" << std::endl;  // Specular color
        mtlFile << "Ns 10.000" << std::endl;             // Specular exponent
        mtlFile << "d 1.000" << std::endl;               // Opacity
        
        objFile.close();
        mtlFile.close();
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Model exported to OBJ: " << filename << RESET << std::endl;
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Material file exported to: " << mtlFilename << RESET << std::endl;
        
        return true;
    }
    
    // Getters
    bool isLoaded() const { return loaded; }
    const std::string& getName() const { return modelName; }
    size_t getVertexCount() const { return vertices.size(); }
    size_t getUVCount() const { return uvs.size(); }
    size_t getFaceCount() const { return faces.size(); }
    
private:
    bool loaded;
    std::string modelName;
    std::vector<Vertex> vertices;
    std::vector<UV> uvs;
    std::vector<Face> faces;
};

// Base class for all painting tools
class PaintTool {
public:
    PaintTool(const std::string& name, const Color& color) 
        : name(name), color(color), size(1.0f), useEffects(false) {}
    
    virtual ~PaintTool() {}
    
    virtual void use(Layer* layer, int x, int y) {
        if (!layer) return;
        layer->getTexture().paint(x, y, color, size);
        std::cout << "Painting at " << x << "," << y << " with " << name << " " << color.name << " (size: " << size << ")" << std::endl;
    }
    
    void setSize(float newSize) { size = std::max(0.1f, newSize); }
    float getSize() const { return size; }
    
    const std::string& getName() const { return name; }
    const Color& getColor() const { return color; }
    void setColor(const Color& newColor) { color = newColor; }
    
    void setUseEffects(bool useEffects) { this->useEffects = useEffects; }
    bool getUseEffects() const { return useEffects; }
    
protected:
    std::string name;
    Color color;
    float size;
    bool useEffects;
};

// Brush tool - standard painting with size
class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", Color("*", FG_RED, "Red Brush")) {}
    
    void use(Layer* layer, int x, int y) override {
        PaintTool::use(layer, x, y);
    }
};

// Eraser tool - clears pixels
class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", Color(" ", RESET, "Eraser")) {}
    
    void use(Layer* layer, int x, int y) override {
        if (!layer) return;
        Color blankColor(" ", RESET, "Eraser");
        layer->getTexture().paint(x, y, blankColor, size);
        std::cout << "Erasing at " << x << "," << y << " (size: " << size << ")" << std::endl;
    }
};

// Line Art tool - draws connected lines
class LineArtTool : public PaintTool {
public:
    LineArtTool() : PaintTool("LineArt", Color("-", FG_BLUE, "Blue LineArt")), lastX(-1), lastY(-1) {}
    
    void use(Layer* layer, int x, int y) override {
        if (!layer) return;
        
        if (lastX >= 0 && lastY >= 0) {
            // Draw a line from last position to current
            layer->getTexture().drawLine(lastX, lastY, x, y, color);
            std::cout << "Drawing line from " << lastX << "," << lastY << " to " << x << "," << y 
                      << " with " << name << " " << color.name << " (size: " << size << ")" << std::endl;
        } else {
            // Just paint a single point
            layer->getTexture().paint(x, y, color, size);
            std::cout << "Starting line at " << x << "," << y << " with " << name << " (size: " << size << ")" << std::endl;
        }
        
        lastX = x;
        lastY = y;
    }
    
    void resetLinePosition() {
        lastX = -1;
        lastY = -1;
    }
    
private:
    int lastX, lastY;  // Last drawn position for connected lines
};

// Rainbow Tool - changes color as it draws
class RainbowTool : public PaintTool {
public:
    RainbowTool() : PaintTool("Rainbow", Color("*", FG_RED, "Rainbow")), hue(0.0f) {}
    
    void use(Layer* layer, int x, int y) override {
        if (!layer) return;
        
        // Increment the hue for rainbow effect
        hue = std::fmod(hue + 15.0f, 360.0f);
        
        // Convert HSV to RGB
        HSV hsv(hue, 1.0f, 1.0f);
        RGB rgb = hsv.toRGB();
        std::string ansiColor = rgb.toAnsiColor();
        
        // Update tool color
        color = Color("*", ansiColor, "Rainbow");
        
        // Draw with the current color
        layer->getTexture().paint(x, y, color, size);
        std::cout << "Painting at " << x << "," << y << " with Rainbow (size: " << size << ")" << std::endl;
    }
    
private:
    float hue;  // Current hue for rainbow effect [0-360]
};

// Stamp Tool - stamps predefined patterns
class StampTool : public PaintTool {
public:
    StampTool() : PaintTool("Stamp", Color("*", FG_BRIGHT_MAGENTA, "Stamp")), currentStamp(0) {
        // Define some simple stamps as character arrays
        stamps.push_back("*");
        stamps.push_back("★");
        stamps.push_back("♥");
        stamps.push_back("♦");
        stamps.push_back("◆");
    }
    
    void use(Layer* layer, int x, int y) override {
        if (!layer) return;
        
        // Use the current stamp character
        color.symbol = stamps[currentStamp];
        
        // Paint with the stamp
        layer->getTexture().paint(x, y, color, size);
        std::cout << "Stamping at " << x << "," << y << " with " << stamps[currentStamp] 
                  << " (size: " << size << ")" << std::endl;
    }
    
    void nextStamp() {
        currentStamp = (currentStamp + 1) % stamps.size();
    }
    
private:
    std::vector<std::string> stamps;
    size_t currentStamp;
};

// Pattern Tool - draws more complex patterns
class PatternTool : public PaintTool {
public:
    PatternTool() : PaintTool("Pattern", Color("#", FG_BRIGHT_YELLOW, "Pattern")), currentPattern(0) {
        // Define some simple patterns
        patterns.push_back("grid");
        patterns.push_back("dots");
        patterns.push_back("waves");
        patterns.push_back("squares");
    }
    
    void use(Layer* layer, int x, int y) override {
        if (!layer) return;
        
        // Draw different patterns based on currentPattern
        if (patterns[currentPattern] == "grid") {
            drawGrid(layer, x, y);
        } else if (patterns[currentPattern] == "dots") {
            drawDots(layer, x, y);
        } else if (patterns[currentPattern] == "waves") {
            drawWaves(layer, x, y);
        } else if (patterns[currentPattern] == "squares") {
            drawSquares(layer, x, y);
        } else {
            // Default fallback
            PaintTool::use(layer, x, y);
        }
    }
    
    void nextPattern() {
        currentPattern = (currentPattern + 1) % patterns.size();
        std::cout << FG_BRIGHT_YELLOW << "ℹ " << "Selected pattern: " << patterns[currentPattern] << RESET << std::endl;
    }
    
private:
    std::vector<std::string> patterns;
    size_t currentPattern;
    
    void drawGrid(Layer* layer, int x, int y) {
        // Draw a simple grid pattern
        int radius = static_cast<int>(size * 2);
        for (int i = -radius; i <= radius; i += 2) {
            for (int j = -radius; j <= radius; j += 2) {
                layer->getTexture().paint(x + i, y + j, color, 0.5f);
            }
        }
        std::cout << "Drawing grid at " << x << "," << y << " with " << color.name << " (size: " << size << ")" << std::endl;
    }
    
    void drawDots(Layer* layer, int x, int y) {
        // Draw a dot pattern
        int radius = static_cast<int>(size * 2);
        color.symbol = ".";
        for (int i = -radius; i <= radius; i += 2) {
            for (int j = -radius; j <= radius; j += 2) {
                if ((i + j) % 4 == 0) {
                    layer->getTexture().paint(x + i, y + j, color, 0.5f);
                }
            }
        }
        std::cout << "Drawing dots at " << x << "," << y << " with " << color.name << " (size: " << size << ")" << std::endl;
    }
    
    void drawWaves(Layer* layer, int x, int y) {
        // Draw a wave pattern
        int radius = static_cast<int>(size * 2);
        color.symbol = "~";
        for (int i = -radius; i <= radius; i++) {
            int yOffset = static_cast<int>(std::sin(i * 0.5) * 2);
            layer->getTexture().paint(x + i, y + yOffset, color, 0.5f);
        }
        std::cout << "Drawing waves at " << x << "," << y << " with " << color.name << " (size: " << size << ")" << std::endl;
    }
    
    void drawSquares(Layer* layer, int x, int y) {
        // Draw concentric squares
        int radius = static_cast<int>(size * 2);
        color.symbol = "□";
        for (int r = 1; r <= radius; r += 2) {
            for (int i = -r; i <= r; i++) {
                layer->getTexture().paint(x + i, y - r, color, 0.5f);  // Top
                layer->getTexture().paint(x + i, y + r, color, 0.5f);  // Bottom
                layer->getTexture().paint(x - r, y + i, color, 0.5f);  // Left
                layer->getTexture().paint(x + r, y + i, color, 0.5f);  // Right
            }
        }
        std::cout << "Drawing squares at " << x << "," << y << " with " << color.name << " (size: " << size << ")" << std::endl;
    }
};

// Project class to manage layers and model
class Project {
public:
    Project() : currentLayerIndex(0) {
        model = new Model();
        std::cout << FG_YELLOW << "✓ " << "Project created" << RESET << std::endl;
    }
    
    ~Project() {
        for (auto* layer : layers) {
            delete layer;
        }
        delete model;
    }
    
    void addLayer(const std::string& name, int width = 30, int height = 15) {
        layers.push_back(new Layer(name, width, height));
        currentLayerIndex = layers.size() - 1;
    }
    
    void removeLayer(size_t index) {
        if (index < layers.size()) {
            delete layers[index];
            layers.erase(layers.begin() + index);
            if (currentLayerIndex >= layers.size()) {
                currentLayerIndex = layers.size() - 1;
            }
        }
    }
    
    void selectLayer(size_t index) {
        if (index < layers.size()) {
            currentLayerIndex = index;
            std::cout << FG_BRIGHT_GREEN << "✓ " << "Selected layer: " << layers[currentLayerIndex]->getName() << RESET << std::endl;
        } else {
            std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid layer index" << RESET << std::endl;
        }
    }
    
    void toggleLayerVisibility(size_t index) {
        if (index < layers.size()) {
            layers[index]->setVisible(!layers[index]->isVisible());
        }
    }
    
    Layer* getCurrentLayer() {
        if (layers.empty()) return nullptr;
        return layers[currentLayerIndex];
    }
    
    void listLayers() const {
        std::cout << BOLD << FG_BRIGHT_YELLOW << "=== Layers ===" << RESET << std::endl;
        for (size_t i = 0; i < layers.size(); i++) {
            std::cout << "[" << i << "] ";
            layers[i]->printInfo(i == currentLayerIndex);
        }
    }
    
    void renderLayers() const {
        std::cout << BOLD << FG_BRIGHT_YELLOW << "=== CANVAS ===" << RESET << std::endl;
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->isVisible()) {
                layers[i]->printInfo(i == currentLayerIndex);
                layers[i]->render();
            }
        }
    }
    
    Model& getModel() const { return *model; }
    
    size_t getLayerCount() const { return layers.size(); }
    
private:
    std::vector<Layer*> layers;
    size_t currentLayerIndex;
    Model* model;
};

// Application class to manage the UI and interactions
class Application {
public:
    Application() : currentTool(nullptr), running(true), currentHsvColor(180.0f, 0.8f, 0.9f) {
        std::cout << FG_CYAN << "✓ " << "Application initialized" << RESET << std::endl;
        
        // Create some tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        tools.push_back(new LineArtTool());
        tools.push_back(new RainbowTool());
        tools.push_back(new StampTool());
        tools.push_back(new PatternTool());
        
        for (auto* tool : tools) {
            std::cout << FG_GREEN << "✓ " << "Created paint tool: " << tool->getName() << RESET << std::endl;
        }
        
        // Set initial tool
        currentTool = tools[0];
        
        // Load a default model
        project.getModel().loadModel("cube.obj");
        
        // Create a default layer
        project.addLayer("Background");
        
        // Initialize color palette
        colorPalette['1'] = Color("*", FG_RED, "Red");
        colorPalette['2'] = Color("*", FG_GREEN, "Green");
        colorPalette['3'] = Color("*", FG_BLUE, "Blue");
        colorPalette['4'] = Color("*", FG_YELLOW, "Yellow");
        colorPalette['5'] = Color("*", FG_MAGENTA, "Magenta");
        colorPalette['6'] = Color("*", FG_CYAN, "Cyan");
        colorPalette['7'] = Color("#", FG_BRIGHT_WHITE, "White");
        colorPalette['8'] = Color(".", FG_GREEN, "Dots");
        colorPalette['9'] = Color("=", FG_YELLOW, "Lines");
        colorPalette['0'] = Color(" ", RESET, "Eraser");
        
        std::cout << FG_CYAN << "=== 3D Model Painter Started ===" << RESET << std::endl;
    }
    
    void run() {
        showHelp();
        
        std::string command;
        while (running) {
            displayStatus();
            std::cout << "Command (h for help): ";
            std::getline(std::cin, command);
            
            if (command.empty()) continue;
            
            // Process command
            char cmd = command[0];
            switch (cmd) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5': {
                    int toolIndex = cmd - '1';
                    if (toolIndex >= 0 && toolIndex < static_cast<int>(tools.size())) {
                        currentTool = tools[toolIndex];
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Selected tool: " << currentTool->getName() << RESET << std::endl;
                    }
                    break;
                }
                case '+': {
                    if (currentTool) {
                        currentTool->setSize(currentTool->getSize() + 0.5f);
                        std::cout << FG_BRIGHT_BLUE << "ℹ " << "Increased " << currentTool->getName() 
                                  << " size to " << currentTool->getSize() << RESET << std::endl;
                    }
                    break;
                }
                case '-': {
                    if (currentTool) {
                        currentTool->setSize(std::max(0.5f, currentTool->getSize() - 0.5f));
                        std::cout << FG_BRIGHT_BLUE << "ℹ " << "Decreased " << currentTool->getName() 
                                  << " size to " << currentTool->getSize() << RESET << std::endl;
                    }
                    break;
                }
                case 'p':
                case 'P': {
                    // Parse paint command: p x y
                    int x, y;
                    if (sscanf(command.c_str() + 1, "%d %d", &x, &y) == 2) {
                        if (currentTool && project.getCurrentLayer()) {
                            currentTool->use(project.getCurrentLayer(), x, y);
                        } else {
                            std::cout << FG_BRIGHT_RED << "⚠ " << "No active tool or layer" << RESET << std::endl;
                        }
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid paint command. Format: p x y" << RESET << std::endl;
                    }
                    break;
                }
                case 'c':
                case 'C': {
                    auto* layer = project.getCurrentLayer();
                    if (layer) {
                        layer->clear();
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Cleared layer: " << layer->getName() << RESET << std::endl;
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "No active layer to clear" << RESET << std::endl;
                    }
                    break;
                }
                case 'n':
                case 'N': {
                    // Parse new layer command: n [name]
                    std::string name = "Layer " + std::to_string(project.getLayerCount() + 1);
                    if (command.length() > 2) {
                        name = command.substr(2);
                    }
                    project.addLayer(name);
                    std::cout << FG_BRIGHT_GREEN << "✓ " << "Created new layer: " << name << RESET << std::endl;
                    break;
                }
                case 'l':
                case 'L': {
                    project.listLayers();
                    break;
                }
                case 's':
                case 'S': {
                    // Parse select layer command: s index
                    int index;
                    if (sscanf(command.c_str() + 1, "%d", &index) == 1) {
                        project.selectLayer(index);
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid select command. Format: s index" << RESET << std::endl;
                    }
                    break;
                }
                case 't':
                case 'T': {
                    // Parse toggle layer visibility: t index
                    int index;
                    if (sscanf(command.c_str() + 1, "%d", &index) == 1) {
                        project.toggleLayerVisibility(index);
                        std::cout << FG_BRIGHT_YELLOW << "✓ " << "Toggled visibility of layer " << index << RESET << std::endl;
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid toggle command. Format: t index" << RESET << std::endl;
                    }
                    break;
                }
                case 'v':
                case 'V': {
                    clearScreen();
                    std::cout << BOLD << FG_BRIGHT_CYAN << "=== Current Canvas ===" << RESET << std::endl;
                    project.renderLayers();
                    break;
                }
                case 'h':
                case 'H': {
                    showHelp();
                    break;
                }
                case 'd':
                case 'D': {
                    runDemo();
                    break;
                }
                case 'w':
                case 'W': {
                    showColorWheel();
                    break;
                }
                case 'u':
                case 'U': {
                    showUVTools();
                    break;
                }
                case 'x':
                case 'X': {
                    exportProject();
                    break;
                }
                case 'q':
                case 'Q': {
                    clearScreen();
                    std::cout << BOLD << FG_BRIGHT_RED << "⚠ " << "Are you sure you want to quit? (y/n): " << RESET;
                    std::string confirm;
                    std::getline(std::cin, confirm);
                    if (confirm == "y" || confirm == "Y") {
                        running = false;
                        std::cout << BOLD << FG_BRIGHT_RED << "✓ " << "Quitting application..." << RESET << std::endl;
                    } else {
                        std::cout << FG_BRIGHT_BLUE << "ℹ " << "Quit operation cancelled" << RESET << std::endl;
                    }
                    break;
                }
                default: {
                    // Check if it's a color selection
                    if (colorPalette.find(cmd) != colorPalette.end() && currentTool) {
                        currentTool->setColor(colorPalette[cmd]);
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Color set to: " << colorPalette[cmd].name << RESET << std::endl;
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Unknown command: " << cmd << ". Type 'h' for help." << RESET << std::endl;
                    }
                    break;
                }
            }
        }
    }
    
    // Display the interactive color wheel
    void showColorWheel() {
        clearScreen();
        
        // Initialize currentHsvColor if it's the first time
        if (currentHsvColor.h == 0.0f && currentHsvColor.s == 0.0f && currentHsvColor.v == 0.0f) {
            currentHsvColor = HSV(180.0f, 0.8f, 0.9f);  // Default to a nice cyan
        }
        
        // Create an interactive color wheel with our current color
        InteractiveColorWheel colorWheel(currentHsvColor.h, currentHsvColor.s, currentHsvColor.v);
        
        // Run the color wheel and get the selected color
        color_wheel::Color selectedColor = colorWheel.run();
        
        // Update the current tool color if we have one
        if (currentTool) {
            int r = selectedColor.r;
            int g = selectedColor.g;
            int b = selectedColor.b;
            
            // Convert to 0-1 range for our RGB
            float rNorm = r / 255.0f;
            float gNorm = g / 255.0f;
            float bNorm = b / 255.0f;
            
            RGB toolRgb(rNorm, gNorm, bNorm);
            std::string ansiCode = toolRgb.toAnsiColor();
            std::string colorName = toolRgb.getColorName();
            
            // Update HSV values for our application
            HSV newHsv = toolRgb.toHSV();
            currentHsvColor = newHsv;
            
            currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, colorName));
            std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                      << "███" << RESET << " " << colorName << " (" << ColorWheelAdapter::getAnsiColorCode(selectedColor) 
                      << "███" << RESET << ")" << std::endl;
            
            // Short pause to see the color
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    
    // Display UV Unwrapping Tools
    void showUVTools() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "             UV UNWRAPPING AND EDITING             " 
                  << RESET << std::endl << std::endl;
        
        // Check if a model is loaded
        if (!project.getModel().isLoaded()) {
            std::cout << FG_BRIGHT_RED << "⚠ " << "No model loaded! Please load a model first with 'm' command." << RESET << std::endl;
            std::cout << std::endl << "Press Enter to return...";
            std::cin.get();
            return;
        }
        
        // Show model information
        const Model& model = project.getModel();
        std::cout << BOLD << "Model: " << FG_BRIGHT_CYAN << model.getName() << RESET << std::endl;
        std::cout << "Vertices: " << FG_BRIGHT_YELLOW << model.getVertexCount() << RESET << std::endl;
        std::cout << "Faces: " << FG_BRIGHT_YELLOW << model.getFaceCount() << RESET << std::endl;
        std::cout << "UV Coordinates: " << FG_BRIGHT_YELLOW << model.getUVCount() << RESET << std::endl << std::endl;
        
        // Create a texture for displaying the UV map
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) {
            std::cout << FG_BRIGHT_RED << "⚠ " << "No active layer! Please create a layer first." << RESET << std::endl;
            std::cout << std::endl << "Press Enter to return...";
            std::cin.get();
            return;
        }
        
        // Generate and display UV map
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "UV Map:" << RESET << std::endl;
        
        // Generate UV map on current layer texture
        model.generateUVMap(currentLayer->getTexture());
        
        // Display the UV map
        currentLayer->render(true);
        
        // Show UV editing options
        std::cout << std::endl;
        std::cout << BOLD << FG_GREEN << "UV Unwrapping Options:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "u" << RESET << " - Apply automatic unwrapping to all faces" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p [face_index]" << RESET << " - Apply planar unwrapping to specific face" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s [scale] [offset]" << RESET << " - Adjust UV scale and offset" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "r" << RESET << " - Reset UV coordinates" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << " - Return to main menu" << std::endl;
        
        // Get user input
        std::cout << std::endl;
        std::cout << "Enter command: ";
        std::string command;
        std::getline(std::cin, command);
        
        // Process command
        if (command.empty() || command == "q" || command == "Q") {
            // Return to main menu
            return;
        } else if (command == "u" || command == "U") {
            // Apply automatic unwrapping
            std::cout << FG_BRIGHT_CYAN << "⧗ " << "Applying automatic unwrapping..." << RESET << std::endl;
            Model& mutableModel = const_cast<Model&>(model);
            mutableModel.unwrapUVs();
            
            // Regenerate and display the UV map
            mutableModel.generateUVMap(currentLayer->getTexture());
            currentLayer->render(true);
            
            std::cout << FG_BRIGHT_GREEN << "✓ " << "Unwrapping applied!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV tools menu again
            showUVTools();
        } else if (command[0] == 'p' || command[0] == 'P') {
            // Parse face index
            int faceIndex = -1;
            if (sscanf(command.c_str() + 1, "%d", &faceIndex) == 1) {
                // Apply planar unwrapping to specific face
                std::cout << FG_BRIGHT_CYAN << "⧗ " << "Applying planar unwrapping to face " << faceIndex << "..." << RESET << std::endl;
                Model& mutableModel = const_cast<Model&>(model);
                mutableModel.applyPlanarUnwrap(faceIndex);
                
                // Regenerate and display the UV map
                mutableModel.generateUVMap(currentLayer->getTexture());
                currentLayer->render(true);
                
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Planar unwrapping applied to face " << faceIndex << "!" << RESET << std::endl;
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid face index! Format: p [face_index]" << RESET << std::endl;
            }
            
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV tools menu again
            showUVTools();
        } else if (command[0] == 's' || command[0] == 'S') {
            // Parse scale and offset
            float scale = 1.0f, offset = 0.0f;
            if (sscanf(command.c_str() + 1, "%f %f", &scale, &offset) == 2) {
                // Apply scale and offset to all faces
                std::cout << FG_BRIGHT_CYAN << "⧗ " << "Adjusting UV scale to " << scale << " and offset to " << offset << "..." << RESET << std::endl;
                Model& mutableModel = const_cast<Model&>(model);
                
                // Apply to all faces
                for (int i = 0; i < mutableModel.getFaceCount(); i++) {
                    mutableModel.applyPlanarUnwrap(i, scale, scale, offset, offset);
                }
                
                // Regenerate and display the UV map
                mutableModel.generateUVMap(currentLayer->getTexture());
                currentLayer->render(true);
                
                std::cout << FG_BRIGHT_GREEN << "✓ " << "UV scale and offset adjusted!" << RESET << std::endl;
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid scale/offset! Format: s [scale] [offset]" << RESET << std::endl;
            }
            
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV tools menu again
            showUVTools();
        } else if (command == "r" || command == "R") {
            // Reset UV coordinates
            std::cout << FG_BRIGHT_CYAN << "⧗ " << "Resetting UV coordinates..." << RESET << std::endl;
            Model& mutableModel = const_cast<Model&>(model);
            mutableModel.createDemoModel(); // Reset to default UVs
            
            // Regenerate and display the UV map
            mutableModel.generateUVMap(currentLayer->getTexture());
            currentLayer->render(true);
            
            std::cout << FG_BRIGHT_GREEN << "✓ " << "UV coordinates reset to default!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV tools menu again
            showUVTools();
        } else {
            // Unknown command
            std::cout << FG_BRIGHT_RED << "⚠ " << "Unknown command!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV tools menu again
            showUVTools();
        }
    }
    
    // Export project in various formats
    void exportProject() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "             EXPORT PROJECT             " 
                  << RESET << std::endl << std::endl;
        
        std::cout << BOLD << FG_GREEN << "Export Options:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1" << RESET << " - Export as TXT (text representation)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "2" << RESET << " - Export as PPM (image)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "3" << RESET << " - Export as SVG (vector graphics)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "4" << RESET << " - Export as OBJ/MTL (3D model)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << " - Return to main menu" << std::endl;
        
        // Get user input
        std::cout << std::endl;
        std::cout << "Enter option: ";
        std::string option;
        std::getline(std::cin, option);
        
        if (option.empty() || option == "q" || option == "Q") {
            return;
        }
        
        // Get export filename
        std::cout << "Enter base filename (without extension): ";
        std::string filename;
        std::getline(std::cin, filename);
        
        if (filename.empty()) {
            filename = "export_" + std::to_string(std::time(nullptr));
        }
        
        // Process export option
        if (option == "1") {
            // Export as TXT
            std::string txtFilename = "test_txt_" + filename + ".txt";
            exportAsTXT(txtFilename);
        } else if (option == "2") {
            // Export as PPM
            std::string ppmFilename = "test_ppm_" + filename + ".ppm";
            exportAsPPM(ppmFilename);
        } else if (option == "3") {
            // Export as SVG
            std::string svgFilename = "test_export_" + filename + ".svg";
            exportAsSVG(svgFilename);
        } else if (option == "4") {
            // Export as OBJ/MTL
            std::string objFilename = "test_obj_" + filename + ".obj";
            project.getModel().exportToOBJ(objFilename);
        } else {
            std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid option!" << RESET << std::endl;
        }
        
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
    
    // Export as TXT (simple text representation)
    void exportAsTXT(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return;
        }
        
        file << "3D Model Painter Export - Text Format" << std::endl;
        file << "Model: " << project.getModel().getName() << std::endl;
        file << "Layers: " << project.getLayerCount() << std::endl << std::endl;
        
        // Export each layer
        for (size_t i = 0; i < project.getLayerCount(); i++) {
            Layer* layer = project.getCurrentLayer(); // This should be adjusted to get layer by index
            if (layer && layer->isVisible()) {
                file << "=== Layer: " << layer->getName() << " ===" << std::endl;
                
                // Simple text representation of the texture
                Texture& texture = layer->getTexture();
                for (int y = 0; y < texture.getHeight(); y++) {
                    for (int x = 0; x < texture.getWidth(); x++) {
                        const Color& color = texture.getPixel(x, y);
                        file << color.symbol;
                    }
                    file << std::endl;
                }
                
                file << std::endl;
            }
        }
        
        file.close();
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported as TXT to: " << filename << RESET << std::endl;
    }
    
    // Export as PPM (simple image format)
    void exportAsPPM(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return;
        }
        
        // Get the current layer
        Layer* layer = project.getCurrentLayer();
        if (!layer) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "No active layer to export" << RESET << std::endl;
            file.close();
            return;
        }
        
        Texture& texture = layer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // PPM header (P3 format - ASCII RGB)
        file << "P3" << std::endl;
        file << "# 3D Model Painter Export - " << project.getModel().getName() << std::endl;
        file << width << " " << height << std::endl;
        file << "255" << std::endl;  // Max color value
        
        // Write pixel data
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& color = texture.getPixel(x, y);
                
                // Convert ANSI color to RGB values
                // This is a simplification - in a real app, we'd have proper color conversion
                int r = 200, g = 200, b = 200;  // Default gray
                
                if (color.ansiCode.find(FG_RED) != std::string::npos) {
                    r = 255; g = 0; b = 0;
                } else if (color.ansiCode.find(FG_GREEN) != std::string::npos) {
                    r = 0; g = 255; b = 0;
                } else if (color.ansiCode.find(FG_BLUE) != std::string::npos) {
                    r = 0; g = 0; b = 255;
                } else if (color.ansiCode.find(FG_YELLOW) != std::string::npos) {
                    r = 255; g = 255; b = 0;
                } else if (color.ansiCode.find(FG_MAGENTA) != std::string::npos) {
                    r = 255; g = 0; b = 255;
                } else if (color.ansiCode.find(FG_CYAN) != std::string::npos) {
                    r = 0; g = 255; b = 255;
                } else if (color.ansiCode.find(FG_WHITE) != std::string::npos ||
                           color.ansiCode.find(FG_BRIGHT_WHITE) != std::string::npos) {
                    r = 255; g = 255; b = 255;
                } else if (color.ansiCode.find(FG_BLACK) != std::string::npos) {
                    r = 0; g = 0; b = 0;
                }
                
                // For empty space (eraser), use white
                if (color.symbol == " ") {
                    r = 255; g = 255; b = 255;
                }
                
                file << r << " " << g << " " << b << " ";
            }
            file << std::endl;
        }
        
        file.close();
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported as PPM to: " << filename << RESET << std::endl;
    }
    
    // Export as SVG (vector graphics)
    void exportAsSVG(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return;
        }
        
        // Get the current layer
        Layer* layer = project.getCurrentLayer();
        if (!layer) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "No active layer to export" << RESET << std::endl;
            file.close();
            return;
        }
        
        Texture& texture = layer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // Scale factor for SVG (make it larger for better visibility)
        const int scale = 20;
        
        // SVG header
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << std::endl;
        file << "<svg width=\"" << (width * scale) << "\" height=\"" << (height * scale) 
             << "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
        file << "  <title>3D Model Painter Export - " << project.getModel().getName() << "</title>" << std::endl;
        file << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>" << std::endl;
        
        // Create a group for the texture
        file << "  <g>" << std::endl;
        
        // Draw each pixel as a rectangle
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& color = texture.getPixel(x, y);
                
                // Skip empty spaces
                if (color.symbol == " ") continue;
                
                // Convert ANSI color to hex RGB
                std::string hexColor = "#808080";  // Default gray
                
                if (color.ansiCode.find(FG_RED) != std::string::npos) {
                    hexColor = "#FF0000";
                } else if (color.ansiCode.find(FG_GREEN) != std::string::npos) {
                    hexColor = "#00FF00";
                } else if (color.ansiCode.find(FG_BLUE) != std::string::npos) {
                    hexColor = "#0000FF";
                } else if (color.ansiCode.find(FG_YELLOW) != std::string::npos) {
                    hexColor = "#FFFF00";
                } else if (color.ansiCode.find(FG_MAGENTA) != std::string::npos) {
                    hexColor = "#FF00FF";
                } else if (color.ansiCode.find(FG_CYAN) != std::string::npos) {
                    hexColor = "#00FFFF";
                } else if (color.ansiCode.find(FG_WHITE) != std::string::npos ||
                           color.ansiCode.find(FG_BRIGHT_WHITE) != std::string::npos) {
                    hexColor = "#FFFFFF";
                } else if (color.ansiCode.find(FG_BLACK) != std::string::npos) {
                    hexColor = "#000000";
                }
                
                // Draw rectangle for this pixel
                file << "    <rect x=\"" << (x * scale) << "\" y=\"" << (y * scale) 
                     << "\" width=\"" << scale << "\" height=\"" << scale 
                     << "\" fill=\"" << hexColor << "\"/>" << std::endl;
                
                // Add text content for symbols
                if (color.symbol != "*" && color.symbol != " ") {
                    file << "    <text x=\"" << (x * scale + scale/2) << "\" y=\"" << (y * scale + scale/2 + 5) 
                         << "\" font-size=\"" << (scale/2) << "\" text-anchor=\"middle\" fill=\"black\">"
                         << color.symbol << "</text>" << std::endl;
                }
            }
        }
        
        file << "  </g>" << std::endl;
        file << "</svg>" << std::endl;
        
        file.close();
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported as SVG to: " << filename << RESET << std::endl;
    }
    
    void showHelp() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "                3D MODEL PAINTER HELP                " 
                  << RESET << std::endl << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Tool Selection:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-5" << RESET << "      - Select tools (Brush, Eraser, LineArt, RainbowTool, StampTool)" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Brush Settings:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "+, -" << RESET << "     - Increase/decrease brush size" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-9, 0" << RESET << "   - Select color (0 = eraser)" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Painting Commands:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y" << RESET << "    - Paint at position (x,y)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "c" << RESET << "        - Clear current layer" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Layer Management:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "n [name]" << RESET << " - New layer with optional name" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "l" << RESET << "        - List layers" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s index" << RESET << "  - Select layer by index" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "t index" << RESET << "  - Toggle layer visibility" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Advanced Features:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "w" << RESET << "        - Open enhanced interactive color wheel with harmony suggestions" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "u" << RESET << "        - Open UV unwrapping tools" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "x" << RESET << "        - Export project (TXT, PPM, SVG, OBJ)" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Application Controls:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "v" << RESET << "        - View canvas" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "h" << RESET << "        - Show this help" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "d" << RESET << "        - Run demo" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "        - Quit" << std::endl;
        
        std::cout << std::endl;
        std::cout << BOLD << FG_BRIGHT_CYAN << "===============================" << RESET << std::endl;
    }
    
    void displayStatus() {
        if (currentTool) {
            std::cout << "Current Tool: " << currentTool->getName() << std::endl;
            std::cout << "Brush Size: " << currentTool->getSize() << std::endl;
            std::cout << "Color: " << currentTool->getColor().ansiCode << currentTool->getColor().name 
                      << " " << currentTool->getColor().symbol << RESET << std::endl;
        }
        
        Layer* layer = project.getCurrentLayer();
        if (layer) {
            std::cout << "Layer: " << layer->getName() << " (Index: " << project.getLayerCount() - 1 << ")" << std::endl;
        }
    }
    
    // Function to run a demo with cool animations
    void runDemo() {
        clearScreen();
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "=== 3D Model Painter Demo Animation ===" << RESET << std::endl;
        
        // Create some layers if needed
        if (project.getLayerCount() < 2) {
            project.addLayer("Animation Layer");
        }
        
        // Set up tools for demo
        PaintTool* brushTool = tools[0];  // Brush
        PaintTool* rainbowTool = tools[3]; // Rainbow 
        PaintTool* stampTool = tools[4];   // Stamp
        PaintTool* patternTool = tools[5]; // Pattern
        
        // Part 1: Draw a rainbow frame
        auto* layer = project.getCurrentLayer();
        if (!layer) return;
        
        // Clear the layer
        layer->clear();
        
        std::cout << FG_BRIGHT_BLUE << "⧗ " << "Creating rainbow frame..." << RESET << std::endl;
        
        // Draw a colorful frame
        rainbowTool->setSize(1.5f);
        rainbowTool->setUseEffects(true);
        
        // Draw top and bottom edges
        for (int x = 2; x < 28; x++) {
            rainbowTool->use(layer, x, 2);
            rainbowTool->use(layer, x, 12);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            layer->render();
        }
        
        // Draw left and right edges
        for (int y = 2; y <= 12; y++) {
            rainbowTool->use(layer, 2, y);
            rainbowTool->use(layer, 27, y);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            layer->render();
        }
        
        // Part 2: Add some patterns inside
        std::cout << FG_BRIGHT_GREEN << "⧗ " << "Adding patterns..." << RESET << std::endl;
        
        // Draw pattern at corners
        patternTool->setSize(1.0f);
        dynamic_cast<PatternTool*>(patternTool)->nextPattern(); // Choose a pattern
        patternTool->use(layer, 6, 6);
        layer->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        dynamic_cast<PatternTool*>(patternTool)->nextPattern();
        patternTool->use(layer, 23, 6);
        layer->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        dynamic_cast<PatternTool*>(patternTool)->nextPattern();
        patternTool->use(layer, 14, 7);
        layer->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        // Part 3: Add some stamps
        std::cout << FG_BRIGHT_CYAN << "⧗ " << "Adding decorative stamps..." << RESET << std::endl;
        
        stampTool->setSize(1.5f);
        stampTool->setUseEffects(true);
        
        for (int i = 0; i < 5; i++) {
            dynamic_cast<StampTool*>(stampTool)->nextStamp();
            int x = 5 + (rand() % 20);
            int y = 3 + (rand() % 8);
            stampTool->use(layer, x, y);
            layer->render();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        
        // Part 4: Write some text
        std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Creating text..." << RESET << std::endl;
        
        // Simple "3D" text
        brushTool->setSize(1.0f);
        brushTool->setColor(Color("*", FG_BRIGHT_CYAN, "Cyan"));
        
        // Draw "3D" (very simplified)
        std::vector<std::pair<int, int>> letterPoints = {
            {12, 4}, {13, 4}, {14, 4}, {15, 4}, // top of 3
            {15, 5}, {15, 6}, // right side of 3
            {13, 7}, {14, 7}, // middle of 3
            {15, 8}, {15, 9}, // right side of 3
            {12, 10}, {13, 10}, {14, 10}, {15, 10}, // bottom of 3
            
            {18, 4}, {18, 5}, {18, 6}, {18, 7}, {18, 8}, {18, 9}, {18, 10}, // left side of D
            {19, 4}, {20, 4}, {21, 4}, // top of D
            {22, 5}, {22, 6}, {22, 7}, {22, 8}, {22, 9}, // right side of D
            {19, 10}, {20, 10}, {21, 10}, // bottom of D
        };
        
        for (const auto& point : letterPoints) {
            brushTool->use(layer, point.first, point.second);
            layer->render();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        // Final animation
        std::cout << FG_BRIGHT_YELLOW << "⧗ " << "Adding final touches..." << RESET << std::endl;
        
        // Add some sparkles
        rainbowTool->setSize(0.5f);
        for (int i = 0; i < 30; i++) {
            int x = 3 + (rand() % 24);
            int y = 3 + (rand() % 8);
            rainbowTool->use(layer, x, y);
            layer->render();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "✨ " << "Demo completed successfully!" << RESET << std::endl;
        std::cout << std::endl << FG_BRIGHT_CYAN << "Press Enter to continue..." << RESET;
        std::cin.get();
        clearScreen();
    }
    
    ~Application() {
        for (auto* tool : tools) {
            delete tool;
        }
    }
    
private:
    Project project;
    std::vector<PaintTool*> tools;
    PaintTool* currentTool;
    bool running;
    std::map<char, Color> colorPalette;
    HSV currentHsvColor; // Current color in HSV for color wheel
};

int main(int argc, char* argv[]) {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Setup console for proper color display on Windows
    setupConsole();
    
    try {
        Application app;
        
        if (argc > 1 && std::string(argv[1]) == "--demo") {
            app.runDemo();
        } else {
            app.run();
        }
        
        clearScreen();
        std::cout << BOLD << FG_BRIGHT_GREEN << "=== Application ran successfully! ===" << RESET << std::endl;
        
        // Goodbye message
        std::string goodbye = std::string(FG_BRIGHT_CYAN) + "Thanks for using 3D Model Painter! See you next time!" + std::string(RESET);
        animateText(goodbye, 30);
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << BOLD << FG_BRIGHT_RED << "Error: " << e.what() << RESET << std::endl;
        return 1;
    }
}