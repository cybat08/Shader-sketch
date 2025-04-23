#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <memory>
#include <limits>
#include <random>

// Color wheel with harmony suggestions
#include "color_wheel.h"
#include "color_wheel_app.cpp"

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

// Special icons and symbols - only those actually used in the application
#define ICON_CHECK    "✓"
#define ICON_WAIT     "⧗"
#define ICON_WARNING  "⚠"
#define ICON_STAR     "★"
#define ICON_HEART    "♥"
#define ICON_DIAMOND  "♦"
#define ICON_CLUB     "♣"
#define ICON_SPADE    "♠"
#define ICON_SUN      "☀"
#define ICON_MOON     "☽"
#define ICON_UMBRELLA "☂"
#define ICON_SMILE    "☺"
#define ICON_FLOWER   "✿"
#define ICON_SNOWFLAKE "❄"
#define ICON_SPARKLE  "✨"

// Utility functions
void clearScreen() {
    // ANSI escape code (works on Linux, macOS, and most modern terminals)
    std::cout << "\033[2J\033[1;1H";
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
        // Use 24-bit true color ANSI escape sequence
        int r8 = static_cast<int>(r * 255);
        int g8 = static_cast<int>(g * 255);
        int b8 = static_cast<int>(b * 255);
        return "\033[38;2;" + std::to_string(r8) + ";" + std::to_string(g8) + ";" + std::to_string(b8) + "m";
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
        
        std::cout << FG_CYAN << ICON_CHECK << " " << "Created texture: " << width << "x" << height << RESET << std::endl;
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
    
    // Access to pixels
    std::vector<std::vector<Color>>& getPixels() { return pixels; }
    const std::vector<std::vector<Color>>& getPixels() const { return pixels; }
    
private:
    bool isValidCoordinate(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    int width;
    int height;
    std::vector<std::vector<Color>> pixels;
};

// Layer class to manage a texture with opacity and visibility
class Layer {
public:
    Layer(const std::string& name, int width = 30, int height = 15) 
        : name(name), visible(true), opacity(1.0f), texture(width, height) {
        std::cout << FG_GREEN << ICON_CHECK << " " << "Created layer: " << name << RESET << std::endl;
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
    
    void paint(int x, int y, const Color& color, float size = 1.0f) {
        texture.paint(x, y, color, size);
        
        std::cout << "Painting at " << x << "," << y << " with " << color.name << " " << color.symbol << " (size: " << size << ")" << std::endl;
    }
    
    void clear() {
        texture.clear();
    }
    
    Texture& getTexture() { return texture; }
    const Texture& getTexture() const { return texture; }
    
    void toggleVisibility() { visible = !visible; }
    bool isVisible() const { return visible; }
    void setVisible(bool visibility) { visible = visibility; }
    
    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    
    float getOpacity() const { return opacity; }
    void setOpacity(float newOpacity) { opacity = std::clamp(newOpacity, 0.0f, 1.0f); }
    
private:
    std::string name;
    bool visible;
    float opacity;
    Texture texture;
};

// Base class for all paint tools
class PaintTool {
public:
    PaintTool(const std::string& name, const Color& color, float size = 1.0f)
        : name(name), color(color), size(size) {
        std::cout << FG_BLUE << ICON_CHECK << " " << "Created paint tool: " << name << RESET << std::endl;
    }
    
    virtual ~PaintTool() {}
    
    virtual void apply(Layer& layer, int x, int y) {
        layer.paint(x, y, color, size);
    }
    
    // Getters and setters
    const std::string& getName() const { return name; }
    const Color& getColor() const { return color; }
    float getSize() const { return size; }
    
    void setColor(const Color& newColor) { color = newColor; }
    void setSize(float newSize) { size = std::max(0.5f, newSize); }
    
protected:
    std::string name;
    Color color;
    float size;
};

// Specialized tools
class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", Color("*", FG_RED, "Red Brush")) {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", Color(" ", RESET, "Eraser")) {}
};

class LineArtTool : public PaintTool {
public:
    LineArtTool() : PaintTool("LineArt", Color("#", FG_BLUE, "Blue LineArt")) {}
    
    void apply(Layer& layer, int x, int y) override {
        if (lastX >= 0 && lastY >= 0) {
            layer.getTexture().drawLine(lastX, lastY, x, y, getColor());
            std::cout << "Drawing line from " << lastX << "," << lastY << " to " << x << "," << y << std::endl;
        } else {
            layer.paint(x, y, getColor(), getSize());
        }
        
        lastX = x;
        lastY = y;
    }
    
private:
    int lastX = -1;
    int lastY = -1;
};

class DotBrushTool : public PaintTool {
public:
    DotBrushTool() : PaintTool("DotBrush", Color(".", FG_MAGENTA, "Magenta Dots")) {}
};

class StarBrushTool : public PaintTool {
public:
    StarBrushTool() : PaintTool("StarBrush", Color(ICON_STAR, FG_YELLOW, "Yellow Stars")) {}
};

class RainbowTool : public PaintTool {
public:
    RainbowTool() : PaintTool("Rainbow", Color(ICON_HEART, FG_RED, "Rainbow"), 0.5f) {}
    
    void apply(Layer& layer, int x, int y) override {
        static const std::vector<std::string> symbols = {
            ICON_HEART, ICON_STAR, ICON_DIAMOND, ICON_CLUB, ICON_SPADE, 
            ICON_SUN, ICON_MOON, ICON_UMBRELLA, ICON_SMILE, ICON_FLOWER, ICON_SNOWFLAKE
        };
        
        static const std::vector<std::string> colors = {
            FG_RED, FG_GREEN, FG_BLUE, FG_YELLOW, FG_MAGENTA, FG_CYAN,
            FG_BRIGHT_RED, FG_BRIGHT_GREEN, FG_BRIGHT_BLUE, 
            FG_BRIGHT_YELLOW, FG_BRIGHT_MAGENTA, FG_BRIGHT_CYAN
        };
        
        // Update hue by 30 degrees for each paint operation
        currentHue = std::fmod(currentHue + 30.0f, 360.0f);
        HSV hsv(currentHue, 0.9f, 0.9f);
        RGB rgb = hsv.toRGB();
        
        // Pick random symbol
        std::string symbol = symbols[rand() % symbols.size()];
        
        // Create a color with the new RGB values
        std::string ansiCode = rgb.toAnsiColor();
        std::string colorName = rgb.getColorName();
        
        Color rainbowColor(symbol, ansiCode, colorName);
        layer.paint(x, y, rainbowColor, getSize());
    }
    
private:
    float currentHue = 0.0f;
};

class StampTool : public PaintTool {
public:
    StampTool() : PaintTool("Stamp", Color(ICON_STAR, FG_YELLOW, "Star Stamp"), 1.0f) {}
    
    void setStampType(int type) {
        stampType = type % stampPatterns.size();
        
        // Update the tool's symbol based on the stamp type
        std::string symbol = stampPatterns[stampType][0][0].symbol;
        std::string ansiCode = stampPatterns[stampType][0][0].ansiCode;
        std::string name = "Stamp (" + stampNames[stampType] + ")";
        
        setColor(Color(symbol, ansiCode, name));
    }
    
    void apply(Layer& layer, int x, int y) override {
        // Apply the current stamp pattern centered at (x,y)
        const auto& pattern = stampPatterns[stampType];
        int patternHeight = pattern.size();
        int patternWidth = pattern[0].size();
        
        int startX = x - patternWidth / 2;
        int startY = y - patternHeight / 2;
        
        for (int dy = 0; dy < patternHeight; dy++) {
            for (int dx = 0; dx < patternWidth; dx++) {
                int paintX = startX + dx;
                int paintY = startY + dy;
                
                if (paintX >= 0 && paintX < layer.getTexture().getWidth() &&
                    paintY >= 0 && paintY < layer.getTexture().getHeight()) {
                    layer.paint(paintX, paintY, pattern[dy][dx], 1.0f);
                }
            }
        }
    }
    
    void nextPattern() {
        setStampType((stampType + 1) % stampPatterns.size());
    }
    
    void previousPattern() {
        setStampType((stampType - 1 + stampPatterns.size()) % stampPatterns.size());
    }
    
private:
    int stampType = 0;
    
    std::vector<std::string> stampNames = {
        "Star", "Heart", "Smiley", "Flower", "Snowflake"
    };
    
    std::vector<std::vector<std::vector<Color>>> stampPatterns = {
        // Star pattern
        {
            { Color(ICON_STAR, FG_YELLOW, "Star"), Color(ICON_STAR, FG_YELLOW, "Star"), Color(ICON_STAR, FG_YELLOW, "Star") },
            { Color(ICON_STAR, FG_YELLOW, "Star"), Color(ICON_STAR, FG_BRIGHT_YELLOW, "Star"), Color(ICON_STAR, FG_YELLOW, "Star") },
            { Color(ICON_STAR, FG_YELLOW, "Star"), Color(ICON_STAR, FG_YELLOW, "Star"), Color(ICON_STAR, FG_YELLOW, "Star") }
        },
        // Heart pattern
        {
            { Color(ICON_HEART, FG_RED, "Heart"), Color(ICON_HEART, FG_RED, "Heart"), Color(ICON_HEART, FG_RED, "Heart") },
            { Color(ICON_HEART, FG_RED, "Heart"), Color(ICON_HEART, FG_BRIGHT_RED, "Heart"), Color(ICON_HEART, FG_RED, "Heart") },
            { Color(ICON_HEART, FG_RED, "Heart"), Color(" ", RESET, "Empty"), Color(ICON_HEART, FG_RED, "Heart") }
        },
        // Smiley pattern
        {
            { Color(" ", RESET, "Empty"), Color(ICON_SMILE, FG_YELLOW, "Smile"), Color(" ", RESET, "Empty") },
            { Color(ICON_SMILE, FG_YELLOW, "Smile"), Color(" ", RESET, "Empty"), Color(ICON_SMILE, FG_YELLOW, "Smile") },
            { Color(" ", RESET, "Empty"), Color(ICON_SMILE, FG_YELLOW, "Smile"), Color(" ", RESET, "Empty") }
        },
        // Flower pattern
        {
            { Color(" ", RESET, "Empty"), Color(ICON_FLOWER, FG_MAGENTA, "Flower"), Color(" ", RESET, "Empty") },
            { Color(ICON_FLOWER, FG_MAGENTA, "Flower"), Color(ICON_FLOWER, FG_BRIGHT_MAGENTA, "Flower"), Color(ICON_FLOWER, FG_MAGENTA, "Flower") },
            { Color(" ", RESET, "Empty"), Color(ICON_FLOWER, FG_MAGENTA, "Flower"), Color(" ", RESET, "Empty") }
        },
        // Snowflake pattern
        {
            { Color(ICON_SNOWFLAKE, FG_CYAN, "Snowflake"), Color(" ", RESET, "Empty"), Color(ICON_SNOWFLAKE, FG_CYAN, "Snowflake") },
            { Color(" ", RESET, "Empty"), Color(ICON_SNOWFLAKE, FG_BRIGHT_CYAN, "Snowflake"), Color(" ", RESET, "Empty") },
            { Color(ICON_SNOWFLAKE, FG_CYAN, "Snowflake"), Color(" ", RESET, "Empty"), Color(ICON_SNOWFLAKE, FG_CYAN, "Snowflake") }
        }
    };
};

class PatternTool : public PaintTool {
public:
    PatternTool() : PaintTool("Pattern", Color("~", FG_CYAN, "Cyan Pattern"), 1.0f) {}
    
    void setPatternType(int type) {
        patternType = type % patternTypes.size();
        
        // Update the tool's symbol based on the pattern
        std::string name = "Pattern (" + patternNames[patternType] + ")";
        std::string symbol = patternSymbols[patternType];
        std::string ansiCode = patternColors[patternType];
        
        setColor(Color(symbol, ansiCode, name));
    }
    
    void apply(Layer& layer, int x, int y) override {
        // Apply pattern based on type
        switch (patternType) {
            case 0:  // Checkerboard
                applyCheckerboard(layer, x, y);
                break;
            case 1:  // Horizontal lines
                applyHorizontalLines(layer, x, y);
                break;
            case 2:  // Vertical lines
                applyVerticalLines(layer, x, y);
                break;
            case 3:  // Diagonal lines
                applyDiagonalLines(layer, x, y);
                break;
            case 4:  // Dots
                applyDots(layer, x, y);
                break;
            default:
                layer.paint(x, y, getColor(), getSize());
                break;
        }
    }
    
    void nextPattern() {
        setPatternType((patternType + 1) % patternTypes.size());
    }
    
    void previousPattern() {
        setPatternType((patternType - 1 + patternTypes.size()) % patternTypes.size());
    }
    
private:
    int patternType = 0;
    int patternSize = 3;  // Size of pattern application area
    
    std::vector<std::string> patternTypes = {
        "Checkerboard", "HorizontalLines", "VerticalLines", "DiagonalLines", "Dots"
    };
    
    std::vector<std::string> patternNames = {
        "Checkerboard", "Horizontal", "Vertical", "Diagonal", "Dots"
    };
    
    std::vector<std::string> patternSymbols = {
        "#", "-", "|", "/", "."
    };
    
    std::vector<std::string> patternColors = {
        FG_CYAN, FG_GREEN, FG_BLUE, FG_MAGENTA, FG_YELLOW
    };
    
    void applyCheckerboard(Layer& layer, int x, int y) {
        for (int dy = -patternSize; dy <= patternSize; dy++) {
            for (int dx = -patternSize; dx <= patternSize; dx++) {
                int newX = x + dx;
                int newY = y + dy;
                
                if ((newX + newY) % 2 == 0) {
                    layer.paint(newX, newY, getColor(), 1.0f);
                } else {
                    // Apply an alternate color for the other squares
                    Color altColor(" ", RESET, "Empty");
                    layer.paint(newX, newY, altColor, 1.0f);
                }
            }
        }
    }
    
    void applyHorizontalLines(Layer& layer, int x, int y) {
        for (int dy = -patternSize; dy <= patternSize; dy++) {
            for (int dx = -patternSize; dx <= patternSize; dx++) {
                int newX = x + dx;
                int newY = y + dy;
                
                if (newY % 2 == 0) {
                    layer.paint(newX, newY, getColor(), 1.0f);
                }
            }
        }
    }
    
    void applyVerticalLines(Layer& layer, int x, int y) {
        for (int dy = -patternSize; dy <= patternSize; dy++) {
            for (int dx = -patternSize; dx <= patternSize; dx++) {
                int newX = x + dx;
                int newY = y + dy;
                
                if (newX % 2 == 0) {
                    layer.paint(newX, newY, getColor(), 1.0f);
                }
            }
        }
    }
    
    void applyDiagonalLines(Layer& layer, int x, int y) {
        for (int dy = -patternSize; dy <= patternSize; dy++) {
            for (int dx = -patternSize; dx <= patternSize; dx++) {
                int newX = x + dx;
                int newY = y + dy;
                
                if ((newX + newY) % 3 == 0) {
                    layer.paint(newX, newY, getColor(), 1.0f);
                }
            }
        }
    }
    
    void applyDots(Layer& layer, int x, int y) {
        for (int dy = -patternSize; dy <= patternSize; dy++) {
            for (int dx = -patternSize; dx <= patternSize; dx++) {
                int newX = x + dx;
                int newY = y + dy;
                
                float distance = std::sqrt(dx*dx + dy*dy);
                if (distance <= 0.5f || (dx % 3 == 0 && dy % 3 == 0)) {
                    layer.paint(newX, newY, getColor(), 1.0f);
                }
            }
        }
    }
};

// 3D model vertex, UV, and face data structures
struct Vertex {
    float x, y, z;
    
    Vertex() : x(0.0f), y(0.0f), z(0.0f) {}
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct UV {
    float u, v;
    
    UV() : u(0.0f), v(0.0f) {}
    UV(float u, float v) : u(u), v(v) {}
};

struct Face {
    std::vector<int> vertexIndices;
    mutable std::vector<int> uvIndices;
    
    Face() {}
    Face(const std::vector<int>& v, const std::vector<int>& uv) : vertexIndices(v), uvIndices(uv) {}
};

// 3D Model class
class Model {
public:
    Model() : loaded(false) {}
    
    bool loadFromOBJ(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to open file: " << filename << RESET << std::endl;
            return false;
        }
        
        std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Loading model from: " << filename << " -" << RESET << std::endl;
        
        vertices.clear();
        uvs.clear();
        faces.clear();
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            
            if (token == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(Vertex(x, y, z));
            } else if (token == "vt") {
                float u, v;
                iss >> u >> v;
                uvs.push_back(UV(u, v));
            } else if (token == "f") {
                std::string faceToken;
                std::vector<int> vertexIndices, uvIndices;
                
                while (iss >> faceToken) {
                    // Parse face format: v/vt/vn or v/vt or v//vn or v
                    std::replace(faceToken.begin(), faceToken.end(), '/', ' ');
                    std::istringstream faceIss(faceToken);
                    int vi, vti, vni;
                    
                    faceIss >> vi;
                    vertexIndices.push_back(vi - 1);  // OBJ indices are 1-based
                    
                    if (faceIss >> vti) {
                        uvIndices.push_back(vti - 1);  // OBJ indices are 1-based
                    }
                }
                
                faces.push_back(Face(vertexIndices, uvIndices));
            }
        }
        
        file.close();
        
        loaded = !vertices.empty();
        name = filename;
        
        // If no UVs were loaded, create default UVs
        if (uvs.empty() && !vertices.empty()) {
            createDefaultUVs();
        }
        
        return loaded;
    }
    
    void createDemoModel() {
        // Create a simple cube model
        vertices.clear();
        uvs.clear();
        faces.clear();
        
        // Cube vertices (corners)
        vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f));  // 0: bottom-left-back
        vertices.push_back(Vertex(1.0f, -1.0f, -1.0f));   // 1: bottom-right-back
        vertices.push_back(Vertex(1.0f, 1.0f, -1.0f));    // 2: top-right-back
        vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f));   // 3: top-left-back
        vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f));   // 4: bottom-left-front
        vertices.push_back(Vertex(1.0f, -1.0f, 1.0f));    // 5: bottom-right-front
        vertices.push_back(Vertex(1.0f, 1.0f, 1.0f));     // 6: top-right-front
        vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f));    // 7: top-left-front
        
        // Cube UV coordinates
        // Front face UVs
        uvs.push_back(UV(0.0f, 0.0f));    // 0: bottom-left
        uvs.push_back(UV(0.25f, 0.0f));   // 1: bottom-right
        uvs.push_back(UV(0.25f, 0.5f));   // 2: top-right
        uvs.push_back(UV(0.0f, 0.5f));    // 3: top-left
        
        // Back face UVs
        uvs.push_back(UV(0.25f, 0.0f));   // 4: bottom-left
        uvs.push_back(UV(0.5f, 0.0f));    // 5: bottom-right
        uvs.push_back(UV(0.5f, 0.5f));    // 6: top-right
        uvs.push_back(UV(0.25f, 0.5f));   // 7: top-left
        
        // Left face UVs
        uvs.push_back(UV(0.5f, 0.0f));    // 8: bottom-left
        uvs.push_back(UV(0.75f, 0.0f));   // 9: bottom-right
        uvs.push_back(UV(0.75f, 0.5f));   // 10: top-right
        uvs.push_back(UV(0.5f, 0.5f));    // 11: top-left
        
        // Right face UVs
        uvs.push_back(UV(0.0f, 0.5f));    // 12: bottom-left
        uvs.push_back(UV(0.25f, 0.5f));   // 13: bottom-right
        uvs.push_back(UV(0.25f, 1.0f));   // 14: top-right
        uvs.push_back(UV(0.0f, 1.0f));    // 15: top-left
        
        // Top face UVs
        uvs.push_back(UV(0.25f, 0.5f));   // 16: bottom-left
        uvs.push_back(UV(0.5f, 0.5f));    // 17: bottom-right
        uvs.push_back(UV(0.5f, 1.0f));    // 18: top-right
        uvs.push_back(UV(0.25f, 1.0f));   // 19: top-left
        
        // Bottom face UVs
        uvs.push_back(UV(0.5f, 0.5f));    // 20: bottom-left
        uvs.push_back(UV(0.75f, 0.5f));   // 21: bottom-right
        uvs.push_back(UV(0.75f, 1.0f));   // 22: top-right
        uvs.push_back(UV(0.5f, 1.0f));    // 23: top-left
        
        // Cube faces (triangles)
        // Front face
        faces.push_back(Face({4, 5, 6}, {0, 1, 2}));
        faces.push_back(Face({4, 6, 7}, {0, 2, 3}));
        
        // Back face
        faces.push_back(Face({1, 0, 3}, {4, 5, 6}));
        faces.push_back(Face({1, 3, 2}, {4, 6, 7}));
        
        // Left face
        faces.push_back(Face({0, 4, 7}, {8, 9, 10}));
        faces.push_back(Face({0, 7, 3}, {8, 10, 11}));
        
        // Right face
        faces.push_back(Face({5, 1, 2}, {12, 13, 14}));
        faces.push_back(Face({5, 2, 6}, {12, 14, 15}));
        
        // Top face
        faces.push_back(Face({7, 6, 2}, {16, 17, 18}));
        faces.push_back(Face({7, 2, 3}, {16, 18, 19}));
        
        // Bottom face
        faces.push_back(Face({0, 1, 5}, {20, 21, 22}));
        faces.push_back(Face({0, 5, 4}, {20, 22, 23}));
        
        loaded = true;
        name = "cube";
    }
    
    void createDefaultUVs() {
        // Create simple planar projection UVs
        uvs.clear();
        
        for (const auto& vertex : vertices) {
            // Simple planar projection (normalize x,y to 0-1 range)
            float u = (vertex.x + 1.0f) * 0.5f;
            float v = (vertex.y + 1.0f) * 0.5f;
            uvs.push_back(UV(u, v));
        }
        
        // Update face UV indices to match vertex indices
        for (auto& face : faces) {
            face.uvIndices = face.vertexIndices;
        }
    }
    
    void unwrapUVs() {
        // Apply a simple unwrapping algorithm to all faces
        createDefaultUVs();
        
        // Ensure UVs are spread out
        float uvScale = 1.0f / std::sqrt(faces.size());
        int gridSize = static_cast<int>(std::ceil(std::sqrt(faces.size())));
        
        int faceIndex = 0;
        for (int y = 0; y < gridSize && faceIndex < faces.size(); y++) {
            for (int x = 0; x < gridSize && faceIndex < faces.size(); x++, faceIndex++) {
                // Calculate the UV offset for this face in the grid
                float uOffset = static_cast<float>(x) / gridSize;
                float vOffset = static_cast<float>(y) / gridSize;
                
                applyPlanarUnwrap(faceIndex, uvScale, uvScale, uOffset, vOffset);
            }
        }
    }
    
    void applyPlanarUnwrap(int faceIndex, float uScale = 1.0f, float vScale = 1.0f, float uOffset = 0.0f, float vOffset = 0.0f) {
        if (faceIndex < 0 || faceIndex >= faces.size()) return;
        
        const Face& face = faces[faceIndex];
        
        // Find the dominant axis for this face
        Vertex normal = calculateFaceNormal(face);
        char dominantAxis = 'z';
        float maxComponent = std::abs(normal.z);
        
        if (std::abs(normal.x) > maxComponent) {
            dominantAxis = 'x';
            maxComponent = std::abs(normal.x);
        }
        
        if (std::abs(normal.y) > maxComponent) {
            dominantAxis = 'y';
            maxComponent = std::abs(normal.y);
        }
        
        // Unwrap the face based on the dominant axis
        for (size_t i = 0; i < face.vertexIndices.size(); i++) {
            int vi = face.vertexIndices[i];
            int uvIndex = 0;
            
            if (i < face.uvIndices.size()) {
                uvIndex = face.uvIndices[i];
            } else {
                // Create a new UV coordinate if needed
                uvs.push_back(UV(0, 0));
                uvIndex = uvs.size() - 1;
                face.uvIndices.push_back(uvIndex);
            }
            
            const Vertex& v = vertices[vi];
            UV& uv = uvs[uvIndex];
            
            switch (dominantAxis) {
                case 'x':
                    uv.u = (v.z + 1.0f) * 0.5f * uScale + uOffset;
                    uv.v = (v.y + 1.0f) * 0.5f * vScale + vOffset;
                    break;
                case 'y':
                    uv.u = (v.x + 1.0f) * 0.5f * uScale + uOffset;
                    uv.v = (v.z + 1.0f) * 0.5f * vScale + vOffset;
                    break;
                case 'z':
                default:
                    uv.u = (v.x + 1.0f) * 0.5f * uScale + uOffset;
                    uv.v = (v.y + 1.0f) * 0.5f * vScale + vOffset;
                    break;
            }
        }
    }
    
    Vertex calculateFaceNormal(const Face& face) const {
        if (face.vertexIndices.size() < 3) return Vertex(0.0f, 0.0f, 1.0f);
        
        const Vertex& v0 = vertices[face.vertexIndices[0]];
        const Vertex& v1 = vertices[face.vertexIndices[1]];
        const Vertex& v2 = vertices[face.vertexIndices[2]];
        
        // Calculate two edges
        float e1x = v1.x - v0.x;
        float e1y = v1.y - v0.y;
        float e1z = v1.z - v0.z;
        
        float e2x = v2.x - v0.x;
        float e2y = v2.y - v0.y;
        float e2z = v2.z - v0.z;
        
        // Cross product
        float nx = e1y * e2z - e1z * e2y;
        float ny = e1z * e2x - e1x * e2z;
        float nz = e1x * e2y - e1y * e2x;
        
        // Normalize
        float length = std::sqrt(nx*nx + ny*ny + nz*nz);
        if (length > 0.0f) {
            nx /= length;
            ny /= length;
            nz /= length;
        }
        
        return Vertex(nx, ny, nz);
    }
    
    void generateUVMap(Texture& texture) const {
        if (!loaded) return;
        
        // Clear the texture
        texture.clear();
        
        // Draw a grid
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        Color gridColor(".", FG_BRIGHT_BLACK, "Grid");
        
        // Draw horizontal grid lines
        for (int x = 0; x < width; x += 5) {
            for (int y = 0; y < height; y++) {
                texture.paint(x, y, gridColor, 0.5f);
            }
        }
        
        // Draw vertical grid lines
        for (int y = 0; y < height; y += 5) {
            for (int x = 0; x < width; x++) {
                texture.paint(x, y, gridColor, 0.5f);
            }
        }
        
        // Draw UV coordinates and faces
        for (const auto& face : faces) {
            // Skip faces without UV coordinates
            if (face.uvIndices.empty()) continue;
            
            // Draw the edges of the face
            for (size_t i = 0; i < face.uvIndices.size(); i++) {
                size_t j = (i + 1) % face.uvIndices.size();
                
                int uvIndex1 = face.uvIndices[i];
                int uvIndex2 = face.uvIndices[j];
                
                if (uvIndex1 >= 0 && uvIndex1 < uvs.size() && uvIndex2 >= 0 && uvIndex2 < uvs.size()) {
                    const UV& uv1 = uvs[uvIndex1];
                    const UV& uv2 = uvs[uvIndex2];
                    
                    int x1 = static_cast<int>(uv1.u * width);
                    int y1 = static_cast<int>(uv1.v * height);
                    int x2 = static_cast<int>(uv2.u * width);
                    int y2 = static_cast<int>(uv2.v * height);
                    
                    // Draw the edge
                    texture.drawLine(x1, y1, x2, y2, Color("+", FG_BRIGHT_WHITE, "Edge"));
                }
            }
            
            // Mark the vertices
            for (int uvIndex : face.uvIndices) {
                if (uvIndex >= 0 && uvIndex < uvs.size()) {
                    const UV& uv = uvs[uvIndex];
                    
                    int x = static_cast<int>(uv.u * width);
                    int y = static_cast<int>(uv.v * height);
                    
                    // Draw the vertex
                    texture.paint(x, y, Color("o", FG_BRIGHT_GREEN, "Vertex"), 0.5f);
                }
            }
        }
    }
    
    void exportToOBJ(const std::string& filename) const {
        if (!loaded) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "No model loaded to export!" << RESET << std::endl;
            return;
        }
        
        // Create the OBJ file
        std::ofstream objFile(filename);
        if (!objFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to create OBJ file: " << filename << RESET << std::endl;
            return;
        }
        
        // Write header
        objFile << "# Exported from 3D Model Painter\n";
        objFile << "# Vertices: " << vertices.size() << "\n";
        objFile << "# UVs: " << uvs.size() << "\n";
        objFile << "# Faces: " << faces.size() << "\n\n";
        
        // Write material library reference
        std::string mtlFilename = filename.substr(0, filename.find_last_of('.')) + ".mtl";
        objFile << "mtllib " << mtlFilename << "\n\n";
        
        // Write object name
        objFile << "o " << name << "\n\n";
        
        // Write vertices
        for (const auto& v : vertices) {
            objFile << "v " << v.x << " " << v.y << " " << v.z << "\n";
        }
        objFile << "\n";
        
        // Write texture coordinates
        for (const auto& uv : uvs) {
            objFile << "vt " << uv.u << " " << uv.v << "\n";
        }
        objFile << "\n";
        
        // Write faces with material
        objFile << "usemtl TextureMaterial\n";
        for (const auto& face : faces) {
            objFile << "f";
            for (size_t i = 0; i < face.vertexIndices.size(); i++) {
                int vi = face.vertexIndices[i] + 1;  // OBJ indices are 1-based
                int vti = (i < face.uvIndices.size()) ? face.uvIndices[i] + 1 : 1;
                objFile << " " << vi << "/" << vti;
            }
            objFile << "\n";
        }
        
        objFile.close();
        
        // Create the MTL file for material definitions
        std::ofstream mtlFile(mtlFilename);
        if (!mtlFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to create MTL file: " << mtlFilename << RESET << std::endl;
            return;
        }
        
        // Write material definition
        mtlFile << "# Material for " << name << "\n\n";
        mtlFile << "newmtl TextureMaterial\n";
        mtlFile << "Ka 1.000 1.000 1.000\n";  // Ambient color
        mtlFile << "Kd 1.000 1.000 1.000\n";  // Diffuse color
        mtlFile << "Ks 0.000 0.000 0.000\n";  // Specular color
        mtlFile << "Ns 10.0\n";               // Specular exponent
        mtlFile << "illum 2\n";               // Illumination model
        
        // Reference a texture map if one is exported
        mtlFile << "map_Kd " << name << "_texture.png\n";
        
        mtlFile.close();
        
        std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Model exported to " << filename << " and " << mtlFilename << RESET << std::endl;
    }
    
    // Getters
    bool isLoaded() const { return loaded; }
    
    const std::string& getName() const { return name; }
    
    int getVertexCount() const { return vertices.size(); }
    int getUVCount() const { return uvs.size(); }
    int getFaceCount() const { return faces.size(); }
    
private:
    std::vector<Vertex> vertices;
    std::vector<UV> uvs;
    std::vector<Face> faces;
    std::string name;
    bool loaded;
};

// Project class to manage the model and layers
class Project {
public:
    Project() : currentLayerIndex(-1) {
        std::cout << "Project created" << std::endl;
    }
    
    void addLayer(const std::string& name) {
        layers.push_back(std::make_shared<Layer>(name));
        if (currentLayerIndex < 0) {
            currentLayerIndex = 0;
        }
    }
    
    void selectLayer(int index) {
        if (index >= 0 && index < layers.size()) {
            currentLayerIndex = index;
        }
    }
    
    void toggleLayerVisibility(int index) {
        if (index >= 0 && index < layers.size()) {
            layers[index]->toggleVisibility();
        }
    }
    
    void clearCurrentLayer() {
        if (currentLayerIndex >= 0 && currentLayerIndex < layers.size()) {
            layers[currentLayerIndex]->clear();
        }
    }
    
    void paint(int x, int y, PaintTool* tool) {
        if (currentLayerIndex >= 0 && currentLayerIndex < layers.size() && tool) {
            tool->apply(*layers[currentLayerIndex], x, y);
        }
    }
    
    void loadModel(const std::string& filename) {
        if (model.loadFromOBJ(filename)) {
            std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Model loaded: " << filename << RESET << std::endl;
        } else {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to load model: " << filename << RESET << std::endl;
            model.createDemoModel();
            std::cout << FG_BRIGHT_YELLOW << "ℹ " << "Created default cube model instead." << RESET << std::endl;
        }
    }
    
    const std::vector<std::shared_ptr<Layer>>& getLayers() const { return layers; }
    
    Layer* getCurrentLayer() {
        if (currentLayerIndex >= 0 && currentLayerIndex < layers.size()) {
            return layers[currentLayerIndex].get();
        }
        return nullptr;
    }
    
    const Layer* getLayerAt(int index) const {
        if (index >= 0 && index < layers.size()) {
            return layers[index].get();
        }
        return nullptr;
    }
    
    int getCurrentLayerIndex() const { return currentLayerIndex; }
    
    const Model& getModel() const { return model; }
    Model& getMutableModel() { return model; }
    
private:
    std::vector<std::shared_ptr<Layer>> layers;
    int currentLayerIndex;
    Model model;
};

// Main application class
class Application {
public:
    Application() : running(true), showHelp(true), currentTool(nullptr) {
        std::cout << "Application initialized" << std::endl;
        
        // Create default paint tools
        tools.push_back(std::make_shared<BrushTool>());
        tools.push_back(std::make_shared<EraserTool>());
        tools.push_back(std::make_shared<LineArtTool>());
        tools.push_back(std::make_shared<RainbowTool>());
        tools.push_back(std::make_shared<StampTool>());
        tools.push_back(std::make_shared<PatternTool>());
        
        // Set current tool to the first one (Brush)
        currentTool = tools[0].get();
        
        // Create a default model (cube)
        project.loadModel("cube.obj");
        
        // Create a default layer
        project.addLayer("Background");
    }
    
    void run() {
        std::cout << "=== 3D Model Painter Started ===" << std::endl;
        
        if (showHelp) {
            displayHelp();
        }
        
        while (running) {
            // Show current state
            std::cout << std::endl;
            std::cout << "Current Tool: " << currentTool->getName() << std::endl;
            std::cout << "Brush Size: " << currentTool->getSize() << std::endl;
            std::cout << "Color: " << currentTool->getColor().name << " " << currentTool->getColor().ansiCode << currentTool->getColor().symbol << RESET << std::endl;
            
            Layer* currentLayer = project.getCurrentLayer();
            if (currentLayer) {
                std::cout << "Layer: " << currentLayer->getName() << " (Index: " << project.getCurrentLayerIndex() << ")" << std::endl;
            }
            
            // Get user input
            std::cout << "Command (h for help): ";
            std::string command;
            std::getline(std::cin, command);
            
            processCommand(command);
        }
    }
    
    void displayHelp() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "                3D MODEL PAINTER HELP                " 
                  << RESET << std::endl;
        
        std::cout << "Tool Selection:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-6" << RESET << "      - Select tools (Brush, Eraser, LineArt, RainbowTool, StampTool, PatternTool)" << std::endl;
        
        std::cout << "Brush Settings:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "+, -" << RESET << "     - Increase/decrease brush size" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-9, 0" << RESET << "   - Select color (0 = eraser)" << std::endl;
        
        std::cout << "Painting Commands:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y" << RESET << "    - Paint at position (x,y)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "c" << RESET << "        - Clear current layer" << std::endl;
        
        std::cout << "Layer Management:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "n [name]" << RESET << " - New layer with optional name" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "l" << RESET << "        - List layers" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s index" << RESET << "  - Select layer by index" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "t index" << RESET << "  - Toggle layer visibility" << std::endl;
        
        std::cout << "Advanced Features:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "w" << RESET << "        - Open interactive color wheel" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "u" << RESET << "        - Open UV unwrapping tools" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "x" << RESET << "        - Export project (TXT, PPM, SVG, OBJ)" << std::endl;
        
        std::cout << "Application Controls:" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "v" << RESET << "        - View canvas" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "h" << RESET << "        - Show this help" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "d" << RESET << "        - Run demo" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "        - Quit" << std::endl;
        
        std::cout << "===============================" << std::endl;
    }
    
    void processCommand(const std::string& command) {
        if (command.empty()) return;
        
        char cmd = command[0];
        
        switch (cmd) {
            case 'h':
            case 'H':
                displayHelp();
                break;
                
            case 'q':
            case 'Q':
                running = false;
                break;
                
            case 'v':
            case 'V':
                viewCanvas();
                break;
                
            case 'p':
            case 'P':
                // Paint at specific coordinates
                processPaintCommand(command);
                break;
                
            case 'c':
            case 'C':
                // Clear current layer
                project.clearCurrentLayer();
                break;
                
            case 'n':
            case 'N':
                // New layer
                processNewLayerCommand(command);
                break;
                
            case 'l':
            case 'L':
                // List layers
                listLayers();
                break;
                
            case 's':
            case 'S':
                // Select layer
                processSelectLayerCommand(command);
                break;
                
            case 't':
            case 'T':
                // Toggle layer visibility
                processToggleLayerCommand(command);
                break;
                
            case 'w':
            case 'W':
                // Open color wheel
                showColorWheel();
                break;
                
            case 'u':
            case 'U':
                // Open UV unwrapping tools
                showUVEditor();
                break;
                
            case 'x':
            case 'X':
                // Export project
                exportProject();
                break;
                
            case 'd':
            case 'D':
                // Run demo
                runDemo();
                break;
                
            case '+':
                // Increase brush size
                if (currentTool) {
                    currentTool->setSize(currentTool->getSize() + 0.5f);
                }
                break;
                
            case '-':
                // Decrease brush size
                if (currentTool) {
                    currentTool->setSize(std::max(0.5f, currentTool->getSize() - 0.5f));
                }
                break;
                
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
                // Tool selection (1-6)
                {
                    int toolIndex = cmd - '1';
                    if (toolIndex >= 0 && toolIndex < tools.size()) {
                        currentTool = tools[toolIndex].get();
                    }
                }
                break;
                
            default:
                // Check for color selection with number keys
                if (cmd >= '0' && cmd <= '9' && currentTool) {
                    int colorIndex = (cmd == '0') ? 9 : (cmd - '1');
                    setColorByIndex(colorIndex);
                } else {
                    std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Unknown command: " << command << RESET << std::endl;
                }
                break;
        }
    }
    
    void processPaintCommand(const std::string& command) {
        std::istringstream iss(command);
        char p;
        int x, y;
        
        if (iss >> p >> x >> y) {
            project.paint(x, y, currentTool);
        } else {
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid paint command. Format: p x y" << RESET << std::endl;
        }
    }
    
    void processNewLayerCommand(const std::string& command) {
        std::string layerName = "Layer " + std::to_string(project.getLayers().size());
        
        if (command.length() > 1) {
            layerName = command.substr(2);  // Skip 'n '
        }
        
        project.addLayer(layerName);
        project.selectLayer(project.getLayers().size() - 1);
    }
    
    void processSelectLayerCommand(const std::string& command) {
        std::istringstream iss(command);
        char s;
        int index;
        
        if (iss >> s >> index) {
            project.selectLayer(index);
        } else {
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid select command. Format: s index" << RESET << std::endl;
        }
    }
    
    void processToggleLayerCommand(const std::string& command) {
        std::istringstream iss(command);
        char t;
        int index;
        
        if (iss >> t >> index) {
            project.toggleLayerVisibility(index);
        } else {
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid toggle command. Format: t index" << RESET << std::endl;
        }
    }
    
    void viewCanvas() {
        clearScreen();
        
        std::cout << BOLD << "=== CANVAS ===" << RESET << std::endl;
        
        const auto& layers = project.getLayers();
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->isVisible()) {
                if (i == project.getCurrentLayerIndex()) {
                    std::cout << "[SELECTED]  ";
                }
                std::cout << ICON_DIAMOND << " " << layers[i]->getName() << std::endl;
                layers[i]->render(true);
            }
        }
    }
    
    void listLayers() {
        clearScreen();
        
        std::cout << BOLD << "=== LAYERS ===" << RESET << std::endl;
        
        const auto& layers = project.getLayers();
        for (size_t i = 0; i < layers.size(); i++) {
            std::cout << i << ": ";
            
            if (i == project.getCurrentLayerIndex()) {
                std::cout << FG_BRIGHT_CYAN << BOLD << "[SELECTED] " << RESET;
            }
            
            std::cout << (layers[i]->isVisible() ? ICON_CHECK : " ") << " ";
            std::cout << layers[i]->getName() << std::endl;
        }
    }
    
    void setColorByIndex(int index) {
        if (!currentTool) return;
        
        std::vector<Color> colors = {
            Color("*", FG_RED, "Red"),
            Color("*", FG_GREEN, "Green"),
            Color("*", FG_BLUE, "Blue"),
            Color("*", FG_YELLOW, "Yellow"),
            Color("*", FG_MAGENTA, "Magenta"),
            Color("*", FG_CYAN, "Cyan"),
            Color("*", FG_WHITE, "White"),
            Color("*", FG_BRIGHT_RED, "Bright Red"),
            Color("*", FG_BRIGHT_GREEN, "Bright Green"),
            Color(" ", RESET, "Eraser")
        };
        
        if (index >= 0 && index < colors.size()) {
            currentTool->setColor(colors[index]);
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
            std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Color updated to " << ansiCode 
                      << "███" << RESET << " " << colorName << " (" << ColorWheelAdapter::getAnsiColorCode(selectedColor) 
                      << "███" << RESET << ")" << std::endl;
            
            // Short pause to see the color
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    
    // Display UV Unwrapping Tools
    void showUVEditor() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "             UV UNWRAPPING AND EDITING             " 
                  << RESET << std::endl << std::endl;
        
        // Check if a model is loaded
        if (!project.getModel().isLoaded()) {
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "No model loaded! Please load a model first." << RESET << std::endl;
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
        
        // Create a temporary layer for displaying the UV map if none exists
        Layer* uvLayer = project.getCurrentLayer();
        if (!uvLayer) {
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "No active layer! Creating a UV layer." << RESET << std::endl;
            project.addLayer("UV Layer");
            uvLayer = project.getCurrentLayer();
        }
        
        // Generate and display UV map
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "UV Map:" << RESET << std::endl;
        
        // Generate UV map on current layer texture
        if (uvLayer) {
            model.generateUVMap(uvLayer->getTexture());
            uvLayer->render(true);
        }
        
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
            std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Applying automatic unwrapping..." << RESET << std::endl;
            Model& mutableModel = project.getMutableModel();
            mutableModel.unwrapUVs();
            
            // Regenerate and display the UV map
            if (uvLayer) {
                mutableModel.generateUVMap(uvLayer->getTexture());
                uvLayer->render(true);
            }
            
            std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Unwrapping applied!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV editor menu again
            showUVEditor();
        } else if (command[0] == 'p' || command[0] == 'P') {
            // Parse face index
            int faceIndex = -1;
            if (sscanf(command.c_str() + 1, "%d", &faceIndex) == 1) {
                // Apply planar unwrapping to specific face
                std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Applying planar unwrapping to face " << faceIndex << "..." << RESET << std::endl;
                Model& mutableModel = project.getMutableModel();
                mutableModel.applyPlanarUnwrap(faceIndex);
                
                // Regenerate and display the UV map
                if (uvLayer) {
                    mutableModel.generateUVMap(uvLayer->getTexture());
                    uvLayer->render(true);
                }
                
                std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Planar unwrapping applied to face " << faceIndex << "!" << RESET << std::endl;
            } else {
                std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid face index! Format: p [face_index]" << RESET << std::endl;
            }
            
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV editor menu again
            showUVEditor();
        } else if (command[0] == 's' || command[0] == 'S') {
            // Parse scale and offset
            float scale = 1.0f, offset = 0.0f;
            if (sscanf(command.c_str() + 1, "%f %f", &scale, &offset) == 2) {
                // Apply scale and offset to all faces
                std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Adjusting UV scale to " << scale << " and offset to " << offset << "..." << RESET << std::endl;
                Model& mutableModel = project.getMutableModel();
                
                // Apply to all faces
                for (int i = 0; i < mutableModel.getFaceCount(); i++) {
                    mutableModel.applyPlanarUnwrap(i, scale, scale, offset, offset);
                }
                
                // Regenerate and display the UV map
                if (uvLayer) {
                    mutableModel.generateUVMap(uvLayer->getTexture());
                    uvLayer->render(true);
                }
                
                std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "UV scale and offset adjusted!" << RESET << std::endl;
            } else {
                std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid scale/offset! Format: s [scale] [offset]" << RESET << std::endl;
            }
            
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV editor menu again
            showUVEditor();
        } else if (command == "r" || command == "R") {
            // Reset UV coordinates
            std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Resetting UV coordinates..." << RESET << std::endl;
            Model& mutableModel = project.getMutableModel();
            mutableModel.createDemoModel(); // Reset to default UVs
            
            // Regenerate and display the UV map
            if (uvLayer) {
                mutableModel.generateUVMap(uvLayer->getTexture());
                uvLayer->render(true);
            }
            
            std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "UV coordinates reset to default!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV editor menu again
            showUVEditor();
        } else {
            // Unknown command
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Unknown command!" << RESET << std::endl;
            std::cout << std::endl << "Press Enter to continue...";
            std::cin.get();
            
            // Show the UV editor menu again
            showUVEditor();
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
            std::cout << FG_BRIGHT_RED << ICON_WARNING << " " << "Invalid option!" << RESET << std::endl;
        }
        
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
    
    // Export as TXT (simple text representation)
    void exportAsTXT(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to create file: " << filename << RESET << std::endl;
            return;
        }
        
        file << "===== 3D Model Painter Export =====" << std::endl;
        file << "Timestamp: " << std::time(nullptr) << std::endl;
        file << "Model: " << project.getModel().getName() << std::endl;
        file << "Vertices: " << project.getModel().getVertexCount() << std::endl;
        file << "Faces: " << project.getModel().getFaceCount() << std::endl;
        file << "UVs: " << project.getModel().getUVCount() << std::endl;
        file << "Layers: " << project.getLayers().size() << std::endl;
        file << "===============================" << std::endl << std::endl;
        
        // Export each visible layer
        const auto& layers = project.getLayers();
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->isVisible()) {
                file << "Layer " << i << ": " << layers[i]->getName() << std::endl;
                
                // Get texture dimensions
                const Texture& texture = layers[i]->getTexture();
                int width = texture.getWidth();
                int height = texture.getHeight();
                
                // Print top border
                file << "  +";
                for (int x = 0; x < width; x++) {
                    file << "-";
                }
                file << "+" << std::endl;
                
                // Print texture content
                for (int y = 0; y < height; y++) {
                    file << "  |";
                    for (int x = 0; x < width; x++) {
                        file << texture.getPixel(x, y).symbol;
                    }
                    file << "|" << std::endl;
                }
                
                // Print bottom border
                file << "  +";
                for (int x = 0; x < width; x++) {
                    file << "-";
                }
                file << "+" << std::endl << std::endl;
            }
        }
        
        file.close();
        
        std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Exported as TXT to: " << filename << RESET << std::endl;
    }
    
    // Export as PPM (image format)
    void exportAsPPM(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to create file: " << filename << RESET << std::endl;
            return;
        }
        
        // Get the dimensions from the first layer
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "No active layer to export!" << RESET << std::endl;
            return;
        }
        
        const Texture& texture = currentLayer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // PPM header (P6 format)
        file << "P6\n";
        file << width << " " << height << "\n";
        file << "255\n";  // Max color value
        
        // Composite all visible layers (bottom to top)
        const auto& layers = project.getLayers();
        std::vector<std::vector<Color>> compositeImage(height, std::vector<Color>(width));
        
        // Start with empty image
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                compositeImage[y][x] = Color(" ", FG_BLACK, "Empty");
            }
        }
        
        // Composite layers from bottom to top
        for (const auto& layer : layers) {
            if (layer->isVisible()) {
                const Texture& layerTexture = layer->getTexture();
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        const Color& pixel = layerTexture.getPixel(x, y);
                        if (pixel.symbol != " ") {  // Only replace non-empty pixels
                            compositeImage[y][x] = pixel;
                        }
                    }
                }
            }
        }
        
        // Write pixel data
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& color = compositeImage[y][x];
                
                // Parse ANSI color code to get RGB values
                // This is a simple approximation for the demo
                unsigned char r = 0, g = 0, b = 0;
                
                // Convert ansiCode to RGB (very simplified)
                if (color.ansiCode.find("31") != std::string::npos || color.ansiCode.find("91") != std::string::npos) {
                    r = 255;  // Red
                } else if (color.ansiCode.find("32") != std::string::npos || color.ansiCode.find("92") != std::string::npos) {
                    g = 255;  // Green
                } else if (color.ansiCode.find("34") != std::string::npos || color.ansiCode.find("94") != std::string::npos) {
                    b = 255;  // Blue
                } else if (color.ansiCode.find("33") != std::string::npos || color.ansiCode.find("93") != std::string::npos) {
                    r = 255; g = 255;  // Yellow
                } else if (color.ansiCode.find("35") != std::string::npos || color.ansiCode.find("95") != std::string::npos) {
                    r = 255; b = 255;  // Magenta
                } else if (color.ansiCode.find("36") != std::string::npos || color.ansiCode.find("96") != std::string::npos) {
                    g = 255; b = 255;  // Cyan
                } else if (color.ansiCode.find("37") != std::string::npos || color.ansiCode.find("97") != std::string::npos) {
                    r = 255; g = 255; b = 255;  // White
                }
                
                // True color format "38;2;r;g;b"
                size_t trueColorPos = color.ansiCode.find("38;2;");
                if (trueColorPos != std::string::npos) {
                    std::string rgbPart = color.ansiCode.substr(trueColorPos + 5);
                    std::istringstream iss(rgbPart);
                    
                    int ri, gi, bi;
                    char delimiter;
                    if (iss >> delimiter >> ri >> delimiter >> gi >> delimiter >> bi) {
                        r = static_cast<unsigned char>(ri);
                        g = static_cast<unsigned char>(gi);
                        b = static_cast<unsigned char>(bi);
                    }
                }
                
                // Write RGB bytes
                file.put(r);
                file.put(g);
                file.put(b);
            }
        }
        
        file.close();
        
        std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Exported as PPM to: " << filename << RESET << std::endl;
    }
    
    // Export as SVG (scalable vector graphics)
    void exportAsSVG(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "Failed to create file: " << filename << RESET << std::endl;
            return;
        }
        
        // Get texture dimensions
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) {
            std::cerr << FG_BRIGHT_RED << ICON_WARNING << " " << "No active layer to export!" << RESET << std::endl;
            return;
        }
        
        const Texture& texture = currentLayer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // Scale factor for SVG (10x original size)
        int cellSize = 20;
        int svgWidth = width * cellSize;
        int svgHeight = height * cellSize;
        
        // SVG header
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
        file << "<svg width=\"" << svgWidth << "\" height=\"" << svgHeight 
             << "\" viewBox=\"0 0 " << svgWidth << " " << svgHeight 
             << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        
        // Background rectangle
        file << "  <rect width=\"" << svgWidth << "\" height=\"" << svgHeight 
             << "\" fill=\"white\" stroke=\"none\" />\n";
        
        // Grid lines (optional)
        file << "  <!-- Grid lines -->\n";
        file << "  <g stroke=\"#ddd\" stroke-width=\"1\">\n";
        
        // Vertical grid lines
        for (int x = 0; x <= width; x++) {
            int pos = x * cellSize;
            file << "    <line x1=\"" << pos << "\" y1=\"0\" x2=\"" << pos 
                 << "\" y2=\"" << svgHeight << "\" />\n";
        }
        
        // Horizontal grid lines
        for (int y = 0; y <= height; y++) {
            int pos = y * cellSize;
            file << "    <line x1=\"0\" y1=\"" << pos << "\" x2=\"" << svgWidth 
                 << "\" y2=\"" << pos << "\" />\n";
        }
        
        file << "  </g>\n";
        
        // Export each visible layer
        const auto& layers = project.getLayers();
        for (const auto& layer : layers) {
            if (layer->isVisible()) {
                file << "  <!-- Layer: " << layer->getName() << " -->\n";
                file << "  <g>\n";
                
                const Texture& layerTexture = layer->getTexture();
                
                // Draw each pixel as a filled rectangle or symbol
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        const Color& pixel = layerTexture.getPixel(x, y);
                        if (pixel.symbol != " ") {  // Skip empty pixels
                            int rectX = x * cellSize;
                            int rectY = y * cellSize;
                            
                            // Parse color for SVG fill
                            std::string fillColor = "#000000";  // Default black
                            
                            // Very simplified color conversion
                            if (pixel.ansiCode.find("31") != std::string::npos || pixel.ansiCode.find("91") != std::string::npos) {
                                fillColor = "#FF0000";  // Red
                            } else if (pixel.ansiCode.find("32") != std::string::npos || pixel.ansiCode.find("92") != std::string::npos) {
                                fillColor = "#00FF00";  // Green
                            } else if (pixel.ansiCode.find("34") != std::string::npos || pixel.ansiCode.find("94") != std::string::npos) {
                                fillColor = "#0000FF";  // Blue
                            } else if (pixel.ansiCode.find("33") != std::string::npos || pixel.ansiCode.find("93") != std::string::npos) {
                                fillColor = "#FFFF00";  // Yellow
                            } else if (pixel.ansiCode.find("35") != std::string::npos || pixel.ansiCode.find("95") != std::string::npos) {
                                fillColor = "#FF00FF";  // Magenta
                            } else if (pixel.ansiCode.find("36") != std::string::npos || pixel.ansiCode.find("96") != std::string::npos) {
                                fillColor = "#00FFFF";  // Cyan
                            } else if (pixel.ansiCode.find("37") != std::string::npos || pixel.ansiCode.find("97") != std::string::npos) {
                                fillColor = "#FFFFFF";  // White
                            }
                            
                            // True color format "38;2;r;g;b"
                            size_t trueColorPos = pixel.ansiCode.find("38;2;");
                            if (trueColorPos != std::string::npos) {
                                std::string rgbPart = pixel.ansiCode.substr(trueColorPos + 5);
                                std::istringstream iss(rgbPart);
                                
                                int r, g, b;
                                char delimiter;
                                if (iss >> delimiter >> r >> delimiter >> g >> delimiter >> b) {
                                    char hexColor[8];
                                    sprintf(hexColor, "#%02X%02X%02X", r, g, b);
                                    fillColor = hexColor;
                                }
                            }
                            
                            // Draw the pixel
                            if (pixel.symbol == "*") {
                                // Draw a circle for asterisk
                                int centerX = rectX + cellSize / 2;
                                int centerY = rectY + cellSize / 2;
                                int radius = cellSize / 3;
                                
                                file << "    <circle cx=\"" << centerX << "\" cy=\"" << centerY 
                                     << "\" r=\"" << radius << "\" fill=\"" << fillColor << "\" />\n";
                            } else if (pixel.symbol == "#") {
                                // Draw a square
                                int inset = cellSize / 6;
                                file << "    <rect x=\"" << (rectX + inset) << "\" y=\"" << (rectY + inset) 
                                     << "\" width=\"" << (cellSize - 2*inset) << "\" height=\"" << (cellSize - 2*inset) 
                                     << "\" fill=\"" << fillColor << "\" />\n";
                            } else if (pixel.symbol == "+") {
                                // Draw a plus sign
                                int centerX = rectX + cellSize / 2;
                                int centerY = rectY + cellSize / 2;
                                int size = cellSize / 2;
                                
                                file << "    <line x1=\"" << (centerX - size/2) << "\" y1=\"" << centerY 
                                     << "\" x2=\"" << (centerX + size/2) << "\" y2=\"" << centerY 
                                     << "\" stroke=\"" << fillColor << "\" stroke-width=\"2\" />\n";
                                file << "    <line x1=\"" << centerX << "\" y1=\"" << (centerY - size/2) 
                                     << "\" x2=\"" << centerX << "\" y2=\"" << (centerY + size/2) 
                                     << "\" stroke=\"" << fillColor << "\" stroke-width=\"2\" />\n";
                            } else if (pixel.symbol == ".") {
                                // Draw a small dot
                                int centerX = rectX + cellSize / 2;
                                int centerY = rectY + cellSize / 2;
                                int radius = cellSize / 6;
                                
                                file << "    <circle cx=\"" << centerX << "\" cy=\"" << centerY 
                                     << "\" r=\"" << radius << "\" fill=\"" << fillColor << "\" />\n";
                            } else {
                                // Default: draw a filled rectangle
                                file << "    <rect x=\"" << rectX << "\" y=\"" << rectY 
                                     << "\" width=\"" << cellSize << "\" height=\"" << cellSize 
                                     << "\" fill=\"" << fillColor << "\" />\n";
                                
                                // Add text for the symbol
                                int textX = rectX + cellSize / 2;
                                int textY = rectY + cellSize / 2;
                                
                                // Calculate a contrasting color for text
                                std::string textColor = (fillColor == "#FFFFFF" || fillColor == "#FFFF00") ? "#000000" : "#FFFFFF";
                                
                                file << "    <text x=\"" << textX << "\" y=\"" << textY 
                                     << "\" text-anchor=\"middle\" dominant-baseline=\"middle\" fill=\"" << textColor 
                                     << "\" font-size=\"" << (cellSize / 2) << "\">" << pixel.symbol << "</text>\n";
                            }
                        }
                    }
                }
                
                file << "  </g>\n";
            }
        }
        
        // Close SVG
        file << "</svg>\n";
        
        file.close();
        
        std::cout << FG_BRIGHT_GREEN << ICON_CHECK << " " << "Exported as SVG to: " << filename << RESET << std::endl;
    }
    
    // Run a visual demo
    void runDemo() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "                RUNNING DEMO                " 
                  << RESET << std::endl << std::endl;
        
        // Reset project to a clean state
        project = Project();
        project.loadModel("cube.obj");
        
        // Create layers for the demo
        project.addLayer("Background");
        project.addLayer("Details Layer");
        project.selectLayer(1);  // Select the details layer
        
        // Setup tools for the demo
        auto brushTool = std::make_shared<BrushTool>();
        auto starBrushTool = std::make_shared<StarBrushTool>();
        auto lineArtTool = std::make_shared<LineArtTool>();
        
        // Demo: Draw a simple border on the background
        project.selectLayer(0);
        brushTool->setColor(Color("*", FG_BRIGHT_GREEN, "Green Brush"));
        brushTool->setSize(1.0f);
        
        // Animated border drawing
        std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Drawing border..." << RESET << std::endl;
        
        Layer* bgLayer = project.getCurrentLayer();
        if (bgLayer) {
            int width = bgLayer->getTexture().getWidth();
            int height = bgLayer->getTexture().getHeight();
            
            // Draw top and bottom borders
            for (int x = 4; x < width - 4; x++) {
                brushTool->apply(*bgLayer, x, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                bgLayer->render();
                
                brushTool->apply(*bgLayer, x, height - 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                bgLayer->render();
            }
            
            // Draw left and right borders
            for (int y = 1; y < height - 1; y++) {
                brushTool->apply(*bgLayer, 4, y);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                bgLayer->render();
                
                brushTool->apply(*bgLayer, width - 5, y);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                bgLayer->render();
            }
        }
        
        // Draw decorative pattern in the details layer
        project.selectLayer(1);
        lineArtTool->setColor(Color("#", FG_BLUE, "Blue LineArt"));
        lineArtTool->setSize(1.0f);
        
        std::cout << FG_BRIGHT_CYAN << ICON_WAIT << " " << "Drawing frame..." << RESET << std::endl;
        
        Layer* detailsLayer = project.getCurrentLayer();
        if (detailsLayer) {
            int width = detailsLayer->getTexture().getWidth();
            int height = detailsLayer->getTexture().getHeight();
            
            // Draw horizontal lines
            for (int x = 5; x < width - 5; x++) {
                lineArtTool->apply(*detailsLayer, x, 2);
                lineArtTool->apply(*detailsLayer, x, 12);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                detailsLayer->render();
            }
            
            // Draw stars at the corners
            starBrushTool->setColor(Color(ICON_STAR, FG_YELLOW, "Yellow Stars"));
            starBrushTool->apply(*detailsLayer, 7, 4);
            starBrushTool->apply(*detailsLayer, 22, 4);
            starBrushTool->apply(*detailsLayer, 7, 10);
            starBrushTool->apply(*detailsLayer, 22, 10);
            detailsLayer->render();
        }
        
        // Show the final result
        std::cout << FG_BRIGHT_GREEN << ICON_SPARKLE << " " << "Demo completed successfully!" << RESET << std::endl;
        viewCanvas();
        
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
    
private:
    bool running;
    bool showHelp;
    Project project;
    std::vector<std::shared_ptr<PaintTool>> tools;
    PaintTool* currentTool;
    HSV currentHsvColor;
};

int main() {
    // Set random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create and run the application
    Application app;
    app.run();
    
    return 0;
}