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
#include <memory>
#include <limits>
#include <random>
#include <map>
#include <functional>

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
#define BG_BRIGHT_GREEN   "\033[102m"
#define BG_BRIGHT_MAGENTA "\033[105m"

// Special characters and icons - only keeping those actively used in the application
#define ICON_CHECK        "✓"
#define ICON_WARNING      "⚠"
#define ICON_INFO         "ℹ"
#define ICON_HEART        "♥"
#define ICON_DIAMOND      "♦"
#define ICON_STAR         "★"
#define ICON_SPARKLE      "✨"
#define ICON_SUN          "☀"
#define ICON_MOON         "☽"
#define ICON_UMBRELLA     "☂"
#define ICON_SMILE        "☺"
#define ICON_FLOWER       "❀"
#define ICON_SNOWFLAKE    "❄"
#define ICON_CIRCLE       "○"
#define ICON_TRIANGLE     "△"

// Forward declarations
class PaintTool;
class Layer;
class Project;

// Context-sensitive help tooltip system
class HelpTooltip {
public:
    // Types of tooltips
    enum class ToolTipType {
        BASIC,          // Basic usage information
        TECHNIQUE,      // Advanced technique explanation
        SHORTCUT,       // Keyboard shortcut reminder
        WARNING,        // Warning or caution about a feature
        PRO_TIP         // Professional tip/best practice
    };
    
    // Default constructor needed for std::map
    HelpTooltip() : content("No tooltip content"), type(ToolTipType::BASIC) {}
    
    // Constructor
    HelpTooltip(const std::string& content, ToolTipType type = ToolTipType::BASIC)
        : content(content), type(type) {}
    
    // Display the tooltip with appropriate styling
    void display() const {
        std::string prefix;
        std::string color;
        
        switch (type) {
            case ToolTipType::BASIC:
                prefix = std::string(ICON_INFO) + " TIP: ";
                color = FG_CYAN;
                break;
            case ToolTipType::TECHNIQUE:
                prefix = std::string(ICON_STAR) + " TECHNIQUE: ";
                color = FG_YELLOW;
                break;
            case ToolTipType::SHORTCUT:
                prefix = std::string(ICON_DIAMOND) + " SHORTCUT: ";
                color = FG_GREEN;
                break;
            case ToolTipType::WARNING:
                prefix = std::string(ICON_WARNING) + " CAUTION: ";
                color = FG_RED;
                break;
            case ToolTipType::PRO_TIP:
                prefix = std::string(ICON_SPARKLE) + " PRO TIP: ";
                color = FG_MAGENTA;
                break;
        }
        
        std::cout << color << prefix << RESET << content << std::endl;
    }
    
    // Static method to get the appropriate tooltip for a context
    static HelpTooltip getTooltipForContext(const std::string& context);
    
private:
    std::string content;
    ToolTipType type;
};

// Global tooltip repository
class TooltipManager {
public:
    static TooltipManager& getInstance() {
        static TooltipManager instance;
        return instance;
    }
    
    // Get tooltip for a specific context
    HelpTooltip getTooltip(const std::string& context) const {
        auto it = tooltips.find(context);
        if (it != tooltips.end()) {
            return it->second;
        }
        
        // Default tooltip if context not found
        return HelpTooltip("No specific tip available for this feature.", 
                          HelpTooltip::ToolTipType::BASIC);
    }
    
    // Show tooltip for a specific context
    void showTooltip(const std::string& context) const {
        getTooltip(context).display();
    }
    
private:
    TooltipManager() {
        // Initialize all tooltips
        initializeTooltips();
    }
    
    void initializeTooltips() {
        // Tool-specific tooltips
        tooltips["brush"] = HelpTooltip(
            "Apply varying pressure for different brush effects. Try quick, short strokes for texture.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["eraser"] = HelpTooltip(
            "Use small eraser size for detail work and larger sizes for broader changes.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["lineart"] = HelpTooltip(
            "Create perfect curves by using multiple connected line segments rather than one long line.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["rainbow"] = HelpTooltip(
            "Rainbow tool creates smooth color transitions. Try drawing in circular motions for interesting patterns.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["stamp"] = HelpTooltip(
            "Combine multiple stamps with slight position variations to create complex patterns.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["pattern"] = HelpTooltip(
            "Pattern tool works best when applied systematically across larger areas.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["gradient"] = HelpTooltip(
            "For subtle gradients, choose colors that are closer together on the color wheel.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
        
        tooltips["fill"] = HelpTooltip(
            "Before using fill, ensure there are no gaps in the outline to prevent color leaking.",
            HelpTooltip::ToolTipType::WARNING
        );
        
        tooltips["effects"] = HelpTooltip(
            "Layer multiple effects with low intensity for more natural-looking results.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
        
        tooltips["text"] = HelpTooltip(
            "Text tool allows basic formatting. Use with pattern tool for interesting text backgrounds.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        // Feature-specific tooltips
        tooltips["layers"] = HelpTooltip(
            "Work non-destructively by using separate layers for different elements of your painting.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
        
        tooltips["color_wheel"] = HelpTooltip(
            "Try complementary colors for strong contrast or analogous colors for harmony.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["export"] = HelpTooltip(
            "SVG format is best for graphics that need scaling, while OBJ preserves 3D information.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
        
        tooltips["uv_unwrap"] = HelpTooltip(
            "Ensure your UV islands don't overlap to avoid texture artifacts on the 3D model.",
            HelpTooltip::ToolTipType::WARNING
        );
        
        // Shortcut tooltips
        tooltips["shortcuts"] = HelpTooltip(
            "Use number keys 1-0 to quickly switch between tools and colors.",
            HelpTooltip::ToolTipType::SHORTCUT
        );
        
        tooltips["view_shortcuts"] = HelpTooltip(
            "Press 'v' for view mode, 'c' to clear, '+/-' to adjust brush size.",
            HelpTooltip::ToolTipType::SHORTCUT
        );
        
        // Technique tooltips
        tooltips["blending"] = HelpTooltip(
            "For smooth color blending, use multiple layers with reduced opacity.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["texturing"] = HelpTooltip(
            "Create realistic textures by combining pattern tool with manual brush detail work.",
            HelpTooltip::ToolTipType::TECHNIQUE
        );
        
        tooltips["highlighting"] = HelpTooltip(
            "Add highlights after base colors for better control over the final look.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
        
        tooltips["workflow"] = HelpTooltip(
            "Develop your painting in stages: sketch, base colors, details, then final effects.",
            HelpTooltip::ToolTipType::PRO_TIP
        );
    }
    
    std::map<std::string, HelpTooltip> tooltips;
};

// Implementation of the static method
HelpTooltip HelpTooltip::getTooltipForContext(const std::string& context) {
    return TooltipManager::getInstance().getTooltip(context);
}

// Simple HSV color structure with conversion to RGB
struct HSV {
    float h, s, v;
    
    HSV(float h = 0.0f, float s = 0.0f, float v = 0.0f)
        : h(h), s(s), v(v) {}
};

// Simple RGB color structure
struct RGB {
    float r, g, b;
    
    RGB(float r = 0.0f, float g = 0.0f, float b = 0.0f)
        : r(r), g(g), b(b) {}
    
    // Convert to ANSI color code
    std::string toAnsiColor() const {
        int ri = static_cast<int>(r * 255);
        int gi = static_cast<int>(g * 255);
        int bi = static_cast<int>(b * 255);
        return "\033[38;2;" + std::to_string(ri) + ";" + 
               std::to_string(gi) + ";" + std::to_string(bi) + "m";
    }
    
    // Get a descriptive name based on the color
    std::string getColorName() const {
        // Simplified color name determination
        if (r > 0.7f && g < 0.3f && b < 0.3f) return "Red";
        if (r < 0.3f && g > 0.7f && b < 0.3f) return "Green";
        if (r < 0.3f && g < 0.3f && b > 0.7f) return "Blue";
        if (r > 0.7f && g > 0.7f && b < 0.3f) return "Yellow";
        if (r > 0.7f && g < 0.3f && b > 0.7f) return "Magenta";
        if (r < 0.3f && g > 0.7f && b > 0.7f) return "Cyan";
        if (r > 0.7f && g > 0.5f && b > 0.3f) return "Orange";
        if (r > 0.5f && g > 0.3f && b > 0.7f) return "Purple";
        if (r > 0.7f && g > 0.7f && b > 0.7f) return "White";
        if (r < 0.3f && g < 0.3f && b < 0.3f) return "Black";
        return "Custom";
    }
    
    // Convert to HSV color model
    HSV toHSV() const {
        float max = std::max(std::max(r, g), b);
        float min = std::min(std::min(r, g), b);
        float delta = max - min;
        
        HSV hsv;
        hsv.v = max;
        
        if (max > 0.0f)
            hsv.s = delta / max;
        else
            hsv.s = 0.0f;
        
        if (hsv.s == 0.0f)
            hsv.h = 0.0f;
        else {
            if (r == max)
                hsv.h = (g - b) / delta;
            else if (g == max)
                hsv.h = 2.0f + (b - r) / delta;
            else
                hsv.h = 4.0f + (r - g) / delta;
            
            hsv.h *= 60.0f;
            if (hsv.h < 0.0f)
                hsv.h += 360.0f;
        }
        
        return hsv;
    }
};

// Convert HSV to RGB
RGB HSVtoRGB(const HSV& hsv) {
    if (hsv.s == 0.0f) {
        // Achromatic (gray)
        return RGB(hsv.v, hsv.v, hsv.v);
    }
    
    float h = hsv.h / 60.0f;
    int i = static_cast<int>(h);
    float f = h - i;
    float p = hsv.v * (1.0f - hsv.s);
    float q = hsv.v * (1.0f - hsv.s * f);
    float t = hsv.v * (1.0f - hsv.s * (1.0f - f));
    
    switch (i % 6) {
        case 0: return RGB(hsv.v, t, p);
        case 1: return RGB(q, hsv.v, p);
        case 2: return RGB(p, hsv.v, t);
        case 3: return RGB(p, q, hsv.v);
        case 4: return RGB(t, p, hsv.v);
        case 5: return RGB(hsv.v, p, q);
        default: return RGB(0.0f, 0.0f, 0.0f);
    }
}

// Enhanced Color class for terminal-based painting
class Color {
public:
    Color() : symbol(" "), ansiColor(RESET), name("None") {}
    
    Color(const std::string& symbol, const std::string& ansiColor, const std::string& name)
        : symbol(symbol), ansiColor(ansiColor), name(name) {}
    
    const std::string& getSymbol() const { return symbol; }
    const std::string& getAnsiColor() const { return ansiColor; }
    const std::string& getName() const { return name; }
    
    std::string toString() const {
        return ansiColor + symbol + RESET;
    }
    
private:
    std::string symbol;
    std::string ansiColor;
    std::string name;
};

// Texture class for storing the painted pixels
class Texture {
public:
    Texture(int width, int height)
        : width(width), height(height), pixels(width * height, Color()) {
        std::cout << "Created texture: " << width << "x" << height << std::endl;
    }
    
    void clear() {
        std::fill(pixels.begin(), pixels.end(), Color());
    }
    
    void paint(int x, int y, const Color& color, float size) {
        // If size is 1 or less, just paint a single pixel
        if (size <= 1.0f) {
            setPixel(x, y, color);
            return;
        }
        
        // Otherwise, paint a circle with radius based on size
        int radius = static_cast<int>(size + 0.5f);
        
        // Use a circular brush pattern
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                // Check if point is within circle
                if (dx*dx + dy*dy <= radius*radius) {
                    setPixel(x + dx, y + dy, color);
                }
            }
        }
    }
    
    // Fill from current point until boundary is reached
    void fill(int x, int y, const Color& color) {
        if (!isInBounds(x, y)) return;
        
        Color targetColor = getPixel(x, y);
        floodFill(x, y, targetColor, color);
    }
    
    // Gradient fill between two colors
    void gradientFill(int x1, int y1, int x2, int y2, const RGB& color1, const RGB& color2) {
        // Calculate distance between points
        float totalDistance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
        
        // Fill all pixels along the line with interpolated colors
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float distToPoint1 = std::sqrt(std::pow(x - x1, 2) + std::pow(y - y1, 2));
                float factor = std::min(1.0f, distToPoint1 / totalDistance);
                
                // Linear interpolation between colors
                RGB interpolatedRGB(
                    color1.r * (1.0f - factor) + color2.r * factor,
                    color1.g * (1.0f - factor) + color2.g * factor,
                    color1.b * (1.0f - factor) + color2.b * factor
                );
                
                // Convert RGB to ANSI color
                std::string ansiColor = interpolatedRGB.toAnsiColor();
                std::string colorName = interpolatedRGB.getColorName();
                
                setPixel(x, y, Color("*", ansiColor, colorName));
            }
        }
    }
    
    const Color& getPixel(int x, int y) const {
        if (isInBounds(x, y)) {
            return pixels[y * width + x];
        }
        static const Color emptyColor;
        return emptyColor;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int width;
    int height;
    std::vector<Color> pixels;
    
    void setPixel(int x, int y, const Color& color) {
        if (isInBounds(x, y)) {
            pixels[y * width + x] = color;
        }
    }
    
    bool isInBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    
    // Recursive flood fill (be careful with stack overflow on large areas)
    void floodFill(int x, int y, const Color& targetColor, const Color& replacementColor) {
        if (!isInBounds(x, y)) return;
        
        Color currentColor = getPixel(x, y);
        if (currentColor.getName() != targetColor.getName()) return;
        
        setPixel(x, y, replacementColor);
        
        // Recursively fill adjacent pixels
        floodFill(x+1, y, targetColor, replacementColor);
        floodFill(x-1, y, targetColor, replacementColor);
        floodFill(x, y+1, targetColor, replacementColor);
        floodFill(x, y-1, targetColor, replacementColor);
    }
};

// Layer class for organizing painting in layers
class Layer {
public:
    Layer(const std::string& name, int width, int height)
        : name(name), texture(width, height), visible(true) {
        std::cout << "Created layer: " << name << std::endl;
    }
    
    const std::string& getName() const { return name; }
    Texture& getTexture() { return texture; }
    const Texture& getTexture() const { return texture; }
    
    bool isVisible() const { return visible; }
    void setVisible(bool value) { visible = value; }
    
    void toggleVisibility() {
        visible = !visible;
        std::cout << "Layer '" << name << "' is now " 
                  << (visible ? "visible" : "hidden") << std::endl;
    }
    
    void paint(int x, int y, const Color& color, float size) {
        texture.paint(x, y, color, size);
    }
    
    void clear() {
        texture.clear();
        std::cout << "Cleared layer: " << name << std::endl;
    }
    
private:
    std::string name;
    Texture texture;
    bool visible;
};

// Base class for all painting tools (with animation and effects)
class PaintTool {
public:
    PaintTool(const std::string& name, const Color& color, float size = 1.0f)
        : name(name), color(color), size(size), useAnimation(false), 
          useEffects(false), effectsIntensity(0.5f) {
        std::cout << FG_BLUE << ICON_CHECK << " " << "Created paint tool: " << name << RESET << std::endl;
    }
    
    virtual ~PaintTool() {}
    
    // Standard apply method
    virtual void apply(Layer& layer, int x, int y) {
        layer.paint(x, y, color, size);
    }
    
    // Apply with animation (for tools that support it)
    virtual void applyWithAnimation(Layer& layer, int x, int y) {
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        // Default animation just shows a simple "growing" effect
        for (float i = 0.2f; i <= size; i += 0.2f) {
            Color animColor = color;
            layer.paint(x, y, animColor, i);
            animationDelay(15);
        }
    }
    
    // Apply with fancy effects (for tools that support it)
    virtual void applyWithEffects(Layer& layer, int x, int y) {
        if (!useEffects) {
            apply(layer, x, y);
            return;
        }
        
        // Default effect adds some sparkles
        apply(layer, x, y);
        
        // Number of particles based on intensity
        int particles = static_cast<int>(effectsIntensity * 5.0f);
        
        // Add sparkle particles around the brush point
        for (int i = 0; i < particles; i++) {
            int dx = (std::rand() % 5) - 2; 
            int dy = (std::rand() % 5) - 2;
            Color sparkle(ICON_SPARKLE, FG_BRIGHT_YELLOW, "Sparkle");
            layer.paint(x + dx, y + dy, sparkle, 0.5f);
        }
    }
    
    // Getters and setters
    const std::string& getName() const { return name; }
    const Color& getColor() const { return color; }
    float getSize() const { return size; }
    
    void setColor(const Color& newColor) { color = newColor; }
    void setSize(float newSize) { size = std::max(0.5f, newSize); }
    
    bool hasAnimation() const { return useAnimation; }
    void toggleAnimation() { 
        useAnimation = !useAnimation; 
        std::cout << "Animation " << (useAnimation ? "enabled" : "disabled") 
                  << " for " << name << std::endl;
    }
    
    bool hasEffects() const { return useEffects; }
    void toggleEffects() { 
        useEffects = !useEffects; 
        std::cout << "Special effects " << (useEffects ? "enabled" : "disabled") 
                  << " for " << name << std::endl;
    }
    
    void setEffectsIntensity(float intensity) {
        effectsIntensity = std::max(0.1f, std::min(intensity, 1.0f));
        std::cout << "Effects intensity set to " << (effectsIntensity * 100.0f) << "%" << std::endl;
    }
    
protected:
    std::string name;
    Color color;
    float size;
    bool useAnimation;
    bool useEffects;
    float effectsIntensity;
    
    // Helper for animations
    void animationDelay(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
};

// Improved specialized tools

// Brush tool - standard painting with size
class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", Color("*", FG_RED, "Red Brush")) {
        // Enable animation and effects by default for brush
        useAnimation = true;
        useEffects = true;
    }
};

// Eraser tool - clears pixels
class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", Color(" ", RESET, "Eraser")) {}
};

// LineArt tool - draws connected lines
class LineArtTool : public PaintTool {
public:
    LineArtTool() : PaintTool("LineArt", Color("#", FG_BLUE, "Blue LineArt")) {
        lastX = -1;
        lastY = -1;
    }
    
    void apply(Layer& layer, int x, int y) override {
        if (lastX >= 0 && lastY >= 0) {
            // Draw a line from last position to current position
            drawLine(layer, lastX, lastY, x, y, color, size);
        } else {
            // Just draw a single point
            layer.paint(x, y, color, size);
        }
        
        lastX = x;
        lastY = y;
    }
    
    void applyWithAnimation(Layer& layer, int x, int y) override {
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        if (lastX >= 0 && lastY >= 0) {
            // Line animation - draw line segment by segment
            animateDrawLine(layer, lastX, lastY, x, y, color, size);
        } else {
            // Just draw a single point with animation
            for (float i = 0.2f; i <= size; i += 0.2f) {
                layer.paint(x, y, color, i);
                animationDelay(15);
            }
        }
        
        lastX = x;
        lastY = y;
    }
    
    void reset() {
        lastX = -1;
        lastY = -1;
    }
    
private:
    int lastX, lastY;
    
    // Draw a line using Bresenham's algorithm
    void drawLine(Layer& layer, int x0, int y0, int x1, int y1, const Color& color, float size) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            layer.paint(x0, y0, color, size);
            
            if (x0 == x1 && y0 == y1) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
    
    // Animate drawing a line
    void animateDrawLine(Layer& layer, int x0, int y0, int x1, int y1, 
                        const Color& color, float size) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            layer.paint(x0, y0, color, size);
            animationDelay(10); // Small delay for animation
            
            if (x0 == x1 && y0 == y1) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
};

// RainbowTool - changes colors as you paint
class RainbowTool : public PaintTool {
public:
    RainbowTool() : PaintTool("Rainbow", Color(ICON_HEART, FG_RED, "Rainbow"), 0.5f), 
                   currentHue(0.0f) {
        // Enable animation and effects by default
        useAnimation = true;
        useEffects = true;
    }
    
    void apply(Layer& layer, int x, int y) override {
        static const std::vector<std::string> symbols = {
            ICON_HEART, ICON_STAR, ICON_DIAMOND, ICON_SUN, ICON_MOON, 
            ICON_CIRCLE, ICON_TRIANGLE, ICON_UMBRELLA, ICON_SMILE, ICON_FLOWER, ICON_SNOWFLAKE
        };
        
        // Update hue by 30 degrees for each paint operation
        currentHue = std::fmod(currentHue + 30.0f, 360.0f);
        HSV hsv(currentHue, 0.9f, 0.9f);
        RGB rgb = HSVtoRGB(hsv);
        
        // Pick random symbol
        std::string symbol = symbols[rand() % symbols.size()];
        
        // Create a color with the new RGB values
        std::string ansiCode = rgb.toAnsiColor();
        std::string colorName = rgb.getColorName();
        
        Color rainbowColor(symbol, ansiCode, colorName);
        layer.paint(x, y, rainbowColor, getSize());
    }
    
    void applyWithAnimation(Layer& layer, int x, int y) override {
        static const std::vector<std::string> symbols = {
            ICON_HEART, ICON_STAR, ICON_DIAMOND, ICON_SUN, ICON_MOON, 
            ICON_CIRCLE, ICON_TRIANGLE, ICON_UMBRELLA, ICON_SMILE, ICON_FLOWER, ICON_SNOWFLAKE
        };
        
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        // Rainbow color animation - cycle through several colors quickly
        float startHue = currentHue;
        for (float hueOffset = 0; hueOffset <= 120.0f; hueOffset += 30.0f) {
            float hue = std::fmod(startHue + hueOffset, 360.0f);
            HSV hsv(hue, 0.9f, 0.9f);
            RGB rgb = HSVtoRGB(hsv);
            
            // Pick random symbol
            std::string symbol = symbols[rand() % symbols.size()];
            
            // Create a color with the new RGB values
            std::string ansiCode = rgb.toAnsiColor();
            std::string colorName = rgb.getColorName();
            
            Color rainbowColor(symbol, ansiCode, colorName);
            
            // Paint with increasing size for animation effect
            float animSize = size * ((hueOffset + 30.0f) / 150.0f);
            layer.paint(x, y, rainbowColor, animSize);
            
            animationDelay(20);
        }
        
        currentHue = std::fmod(startHue + 30.0f, 360.0f);
    }
    
private:
    float currentHue;
};

// StampTool - places predefined shapes
class StampTool : public PaintTool {
public:
    StampTool() : PaintTool("Stamp", Color(ICON_STAR, FG_BRIGHT_YELLOW, "Yellow Stars"), 1.0f),
                 currentShape(0) {
        // Define some stamp shapes
        shapes.push_back({
            {0, 0, "★"}  // Simple star
        });
        
        shapes.push_back({
            {-1, -1, "♦"}, {0, -1, "♦"}, {1, -1, "♦"},
            {-1,  0, "♦"}, {0,  0, "◆"}, {1,  0, "♦"},
            {-1,  1, "♦"}, {0,  1, "♦"}, {1,  1, "♦"}
            // Diamond pattern
        });
        
        shapes.push_back({
            {0, -2, "♥"}, 
            {-1, -1, "♥"}, {0, -1, "♥"}, {1, -1, "♥"},
            {-2, 0, "♥"}, {-1, 0, "♥"}, {0, 0, "♥"}, {1, 0, "♥"}, {2, 0, "♥"},
            {-1, 1, "♥"}, {0, 1, "♥"}, {1, 1, "♥"},
            {0, 2, "♥"}
            // Heart shape
        });
        
        shapes.push_back({
            {-1, -1, "●"}, {0, -1, "●"}, {1, -1, "●"},
            {-1,  0, "●"}, {0,  0, "☺"}, {1,  0, "●"},
            {-1,  1, "●"}, {0,  1, "●"}, {1,  1, "●"}
            // Smiley face
        });
    }
    
    void apply(Layer& layer, int x, int y) override {
        // Apply the current shape at the given position
        applyShape(layer, x, y, shapes[currentShape], color);
    }
    
    void applyWithAnimation(Layer& layer, int x, int y) override {
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        // Animate the stamp expanding
        for (float scale = 0.2f; scale <= 1.0f; scale += 0.2f) {
            applyShapeWithScale(layer, x, y, shapes[currentShape], color, scale);
            animationDelay(50);
        }
    }
    
    void nextShape() {
        currentShape = (currentShape + 1) % shapes.size();
        std::cout << "Stamp shape changed to pattern " << (currentShape + 1) << std::endl;
    }
    
    void previousShape() {
        currentShape = (currentShape + shapes.size() - 1) % shapes.size();
        std::cout << "Stamp shape changed to pattern " << (currentShape + 1) << std::endl;
    }
    
private:
    struct ShapePoint {
        int dx, dy;
        std::string symbol;
    };
    
    std::vector<std::vector<ShapePoint>> shapes;
    size_t currentShape;
    
    void applyShape(Layer& layer, int x, int y, 
                   const std::vector<ShapePoint>& shape, const Color& color) {
        for (const auto& point : shape) {
            Color pointColor(point.symbol, color.getAnsiColor(), color.getName());
            layer.paint(x + point.dx, y + point.dy, pointColor, size);
        }
    }
    
    void applyShapeWithScale(Layer& layer, int x, int y, 
                            const std::vector<ShapePoint>& shape, 
                            const Color& color, float scale) {
        for (const auto& point : shape) {
            Color pointColor(point.symbol, color.getAnsiColor(), color.getName());
            int scaledX = x + static_cast<int>(point.dx * scale);
            int scaledY = y + static_cast<int>(point.dy * scale);
            layer.paint(scaledX, scaledY, pointColor, size * scale);
        }
    }
};

// GradientTool - creates a gradient between two points
class GradientTool : public PaintTool {
public:
    GradientTool() : PaintTool("Gradient", Color("*", FG_BRIGHT_MAGENTA, "Gradient"), 1.0f),
                    startX(-1), startY(-1), endX(-1), endY(-1), 
                    startColor(RGB(1.0f, 0.0f, 1.0f)), // Start with magenta
                    endColor(RGB(0.0f, 1.0f, 1.0f))    // End with cyan
    {}
    
    void apply(Layer& layer, int x, int y) override {
        // First click sets start point
        if (startX < 0) {
            startX = x;
            startY = y;
            
            // Create a marker at the start point
            Color startMarker("+", FG_BRIGHT_RED, "Start Point");
            layer.paint(x, y, startMarker, size);
            
            std::cout << "Gradient start point set at " << x << "," << y << std::endl;
        }
        // Second click sets end point and creates gradient
        else if (endX < 0) {
            endX = x;
            endY = y;
            
            // Create a marker at the end point
            Color endMarker("+", FG_BRIGHT_GREEN, "End Point");
            layer.paint(x, y, endMarker, size);
            
            std::cout << "Gradient end point set at " << x << "," << y 
                      << ". Creating gradient..." << std::endl;
            
            // Apply the gradient
            layer.getTexture().gradientFill(startX, startY, endX, endY, startColor, endColor);
            
            // Reset for next gradient
            reset();
        }
    }
    
    void reset() {
        startX = -1;
        startY = -1;
        endX = -1;
        endY = -1;
    }
    
    void setStartColor(const RGB& color) {
        startColor = color;
        std::cout << "Gradient start color updated" << std::endl;
    }
    
    void setEndColor(const RGB& color) {
        endColor = color;
        std::cout << "Gradient end color updated" << std::endl;
    }
    
private:
    int startX, startY;
    int endX, endY;
    RGB startColor, endColor;
};

// FillTool - flood fill from a point
class FillTool : public PaintTool {
public:
    FillTool() : PaintTool("Fill", Color("■", FG_BRIGHT_CYAN, "Fill"), 1.0f) {}
    
    void apply(Layer& layer, int x, int y) override {
        // Apply flood fill algorithm
        layer.getTexture().fill(x, y, color);
    }
    
    void applyWithAnimation(Layer& layer, int x, int y) override {
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        // This is a simple animation - in a real app you'd want
        // to animate the actual flood fill algorithm
        
        // Show a "splash" animation first
        for (int radius = 1; radius <= 5; radius++) {
            for (int angle = 0; angle < 360; angle += 45) {
                double rad = angle * 3.14159 / 180.0;
                int dx = static_cast<int>(cos(rad) * radius);
                int dy = static_cast<int>(sin(rad) * radius);
                
                Color splashColor("·", color.getAnsiColor(), color.getName());
                layer.paint(x + dx, y + dy, splashColor, 0.5f);
            }
            animationDelay(30);
        }
        
        // Then do the actual fill
        apply(layer, x, y);
    }
};

// PatternTool - creates repeating patterns
class PatternTool : public PaintTool {
public:
    PatternTool() : PaintTool("Pattern", Color("*", FG_BRIGHT_BLUE, "Pattern"), 1.0f),
                   currentPattern(0) {
        // Define some patterns
        patterns.push_back([](int x, int y) -> std::string {
            // Checkerboard pattern
            return ((x + y) % 2 == 0) ? "■" : "□";
        });
        
        patterns.push_back([](int x, int y) -> std::string {
            // Stripes pattern
            return (x % 3 == 0) ? "║" : (y % 3 == 0) ? "═" : "·";
        });
        
        patterns.push_back([](int x, int y) -> std::string {
            // Grid pattern
            return (x % 3 == 0 && y % 3 == 0) ? "+" : 
                   (x % 3 == 0) ? "|" : 
                   (y % 3 == 0) ? "-" : " ";
        });
        
        patterns.push_back([](int x, int y) -> std::string {
            // Dotted pattern
            return ((x + y) % 3 == 0) ? "•" : " ";
        });
    }
    
    void apply(Layer& layer, int x, int y) override {
        // Apply current pattern in a 5x5 area around the point
        int radius = 2;
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                int patternX = x + dx;
                int patternY = y + dy;
                
                std::string symbol = patterns[currentPattern](patternX, patternY);
                Color patternColor(symbol, color.getAnsiColor(), color.getName());
                
                layer.paint(patternX, patternY, patternColor, size);
            }
        }
    }
    
    void nextPattern() {
        currentPattern = (currentPattern + 1) % patterns.size();
        std::cout << "Pattern changed to design " << (currentPattern + 1) << std::endl;
    }
    
    void previousPattern() {
        currentPattern = (currentPattern + patterns.size() - 1) % patterns.size();
        std::cout << "Pattern changed to design " << (currentPattern + 1) << std::endl;
    }
    
private:
    using PatternFunction = std::function<std::string(int, int)>;
    std::vector<PatternFunction> patterns;
    size_t currentPattern;
};

// SpecialEffectsTool - applies various special effects
class SpecialEffectsTool : public PaintTool {
public:
    SpecialEffectsTool() : PaintTool("Effects", Color("✨", FG_BRIGHT_YELLOW, "Effects"), 1.0f),
                         currentEffect(0) {
        // Define available effects
        effects = {
            { "Sparkle", &SpecialEffectsTool::applySparkleEffect },
            { "Blur", &SpecialEffectsTool::applyBlurEffect },
            { "Rainbow Burst", &SpecialEffectsTool::applyRainbowBurstEffect },
            { "Star Burst", &SpecialEffectsTool::applyStarBurstEffect }
        };
        
        // Enable animation by default
        useAnimation = true;
    }
    
    void apply(Layer& layer, int x, int y) override {
        // Apply the current effect
        (this->*effects[currentEffect].function)(layer, x, y);
    }
    
    void nextEffect() {
        currentEffect = (currentEffect + 1) % effects.size();
        std::cout << "Effect changed to " << effects[currentEffect].name << std::endl;
    }
    
    void previousEffect() {
        currentEffect = (currentEffect + effects.size() - 1) % effects.size();
        std::cout << "Effect changed to " << effects[currentEffect].name << std::endl;
    }
    
    std::string getCurrentEffectName() const {
        return effects[currentEffect].name;
    }
    
private:
    struct Effect {
        std::string name;
        void (SpecialEffectsTool::*function)(Layer&, int, int);
    };
    
    std::vector<Effect> effects;
    size_t currentEffect;
    
    // Effect implementations
    void applySparkleEffect(Layer& layer, int x, int y) {
        // Create sparkles in a random pattern
        int radius = static_cast<int>(size * 3.0f);
        
        for (int i = 0; i < 20; i++) {
            int dx = (std::rand() % (2 * radius + 1)) - radius;
            int dy = (std::rand() % (2 * radius + 1)) - radius;
            
            // Skip if too far from center (create circular pattern)
            if (dx*dx + dy*dy > radius*radius) continue;
            
            // Random sparkle character
            std::string symbol = (std::rand() % 2 == 0) ? "✨" : "⋆";
            
            // Random sparkle color
            float hue = static_cast<float>(std::rand() % 360);
            HSV hsv(hue, 0.9f, 0.9f);
            RGB rgb = HSVtoRGB(hsv);
            std::string ansiColor = rgb.toAnsiColor();
            
            Color sparkleColor(symbol, ansiColor, "Sparkle");
            layer.paint(x + dx, y + dy, sparkleColor, 0.5f);
            
            if (useAnimation) {
                animationDelay(5);
            }
        }
    }
    
    void applyBlurEffect(Layer& layer, int x, int y) {
        // Simple blur effect - averages neighboring pixels
        int radius = static_cast<int>(size * 2.0f);
        
        // For a better blur, we'd need to access and analyze the pixels,
        // but for demonstration purposes, we'll just draw a soft pattern
        
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                // Distance from center
                float distance = std::sqrt(dx*dx + dy*dy);
                if (distance > radius) continue;
                
                // Normalized distance (0.0-1.0)
                float normDistance = distance / radius;
                
                // Less opaque as we get farther from center
                float opacity = 1.0f - normDistance;
                
                // A soft blur character
                std::string symbol = "░";
                if (normDistance < 0.3f) symbol = "▓";
                else if (normDistance < 0.6f) symbol = "▒";
                
                // Create a subtle color for the blur
                Color blurColor(symbol, FG_BRIGHT_CYAN, "Blur");
                layer.paint(x + dx, y + dy, blurColor, 0.5f * opacity);
                
                if (useAnimation) {
                    animationDelay(2);
                }
            }
        }
    }
    
    void applyRainbowBurstEffect(Layer& layer, int x, int y) {
        // Creates a rainbow burst pattern
        int rays = 12;
        int length = static_cast<int>(size * 5.0f);
        
        for (int i = 0; i < rays; i++) {
            float angle = (i * 360.0f / rays) * 3.14159f / 180.0f;
            
            // Create rainbow colors based on angle
            float hue = (i * 360.0f / rays);
            
            for (int dist = 1; dist <= length; dist++) {
                int dx = static_cast<int>(std::cos(angle) * dist);
                int dy = static_cast<int>(std::sin(angle) * dist);
                
                HSV hsv(hue, 1.0f, 1.0f);
                RGB rgb = HSVtoRGB(hsv);
                std::string ansiColor = rgb.toAnsiColor();
                
                // Use different symbols based on distance
                std::string symbol = "*";
                if (dist <= length / 3) symbol = "★";
                else if (dist <= 2 * length / 3) symbol = "*";
                else symbol = "·";
                
                Color rayColor(symbol, ansiColor, "Rainbow");
                layer.paint(x + dx, y + dy, rayColor, 0.5f);
                
                if (useAnimation) {
                    animationDelay(2);
                }
            }
        }
    }
    
    void applyStarBurstEffect(Layer& layer, int x, int y) {
        // Creates a star burst effect
        int numStars = 30;
        int maxDistance = static_cast<int>(size * 4.0f);
        
        // Create burst animation
        for (int step = 0; step < 5; step++) {
            int stepDistance = maxDistance * (step + 1) / 5;
            
            for (int i = 0; i < numStars; i++) {
                float angle = (std::rand() % 360) * 3.14159f / 180.0f;
                float distance = (std::rand() % stepDistance) + 1;
                
                int dx = static_cast<int>(std::cos(angle) * distance);
                int dy = static_cast<int>(std::sin(angle) * distance);
                
                std::string symbol = (i % 3 == 0) ? "★" : "☆";
                Color starColor(symbol, FG_BRIGHT_YELLOW, "Star");
                layer.paint(x + dx, y + dy, starColor, 0.5f);
            }
            
            if (useAnimation) {
                animationDelay(50);
            }
        }
    }
};

// TextTool - allows adding text
class TextTool : public PaintTool {
public:
    TextTool() : PaintTool("Text", Color("T", FG_WHITE, "Text"), 1.0f),
                currentText("Hello") {}
    
    void apply(Layer& layer, int x, int y) override {
        // Draw each character of the text
        for (size_t i = 0; i < currentText.size(); i++) {
            std::string charStr(1, currentText[i]);
            Color textColor(charStr, color.getAnsiColor(), color.getName());
            layer.paint(x + i, y, textColor, size);
        }
    }
    
    void applyWithAnimation(Layer& layer, int x, int y) override {
        if (!useAnimation) {
            apply(layer, x, y);
            return;
        }
        
        // Type out the text character by character
        for (size_t i = 0; i < currentText.size(); i++) {
            std::string charStr(1, currentText[i]);
            Color textColor(charStr, color.getAnsiColor(), color.getName());
            layer.paint(x + i, y, textColor, size);
            animationDelay(100); // Typing delay
        }
    }
    
    void setText(const std::string& text) {
        currentText = text;
        std::cout << "Text tool set to: \"" << currentText << "\"" << std::endl;
    }
    
    const std::string& getText() const {
        return currentText;
    }
    
private:
    std::string currentText;
};

// The Project class manages layers, the model, and project metadata
class Project {
public:
    Project() {
        std::cout << "Project created" << std::endl;
        setDefaultDimensions(30, 15); // Default texture dimensions
    }
    
    // Create a new layer with the standard project dimensions
    Layer* addLayer(const std::string& name) {
        layers.push_back(std::make_unique<Layer>(name, width, height));
        currentLayerIndex = layers.size() - 1;
        return getCurrentLayer();
    }
    
    // Get the current active layer
    Layer* getCurrentLayer() {
        if (currentLayerIndex < layers.size()) {
            return layers[currentLayerIndex].get();
        }
        return nullptr;
    }
    
    // Select a layer by index
    bool selectLayer(size_t index) {
        if (index < layers.size()) {
            currentLayerIndex = index;
            return true;
        }
        return false;
    }
    
    // Toggle layer visibility
    bool toggleLayerVisibility(size_t index) {
        if (index < layers.size()) {
            layers[index]->toggleVisibility();
            return true;
        }
        return false;
    }
    
    // Get all layers
    const std::vector<std::unique_ptr<Layer>>& getLayers() const {
        return layers;
    }
    
    // Set the default project dimensions
    void setDefaultDimensions(int w, int h) {
        width = w;
        height = h;
    }
    
    // Load a model from OBJ file (stub for this implementation)
    bool loadModel(const std::string& filename) {
        std::cout << "Loading model from: " << filename << std::endl;
        return true;
    }
    
    // Export project in various formats
    bool exportAsTXT(const std::string& filename) const;
    bool exportAsPPM(const std::string& filename) const;
    bool exportAsSVG(const std::string& filename) const;
    bool exportAsOBJ(const std::string& filename) const;
    
private:
    std::vector<std::unique_ptr<Layer>> layers;
    size_t currentLayerIndex = 0;
    int width = 0, height = 0;
};

// The main application class
class Application {
public:
    Application() : project(), canvas(), showColorNames(true), showKeyboardShortcuts(true) {
        std::cout << "Application initialized" << std::endl;
        
        // Initialize tools
        initializeTools();
        
        // Set up project
        project.setDefaultDimensions(30, 15);
        project.addLayer("Background");
        
        // Set initial color palette
        initializeColorPalette();
        
        std::cout << "=== 3D Model Painter Enhanced Edition ===" << std::endl;
    }
    
    // Run the main application loop
    void run() {
        std::cout << "Welcome to the Enhanced 3D Model Painter!" << std::endl;
        
        // Main loop
        bool running = true;
        while (running) {
            displayStatus();
            
            std::cout << "Command (h for help): ";
            std::string command;
            std::getline(std::cin, command);
            
            if (command.empty()) continue;
            
            running = processCommand(command);
        }
        
        std::cout << "Thank you for using 3D Model Painter!" << std::endl;
    }
    
    // Run a predefined demo
    void runDemo() {
        std::cout << "Running 3D Model Painter Demo..." << std::endl;
        
        // Create a demo layer
        Layer* demoLayer = project.addLayer("Demo Layer");
        
        // Draw a fun scene
        drawDemoScene(*demoLayer);
        
        // Display the result
        displayCanvas();
        
        std::cout << ICON_SPARKLE << " Demo completed successfully!" << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
    
private:
    Project project;
    std::vector<std::unique_ptr<PaintTool>> tools;
    PaintTool* currentTool = nullptr;
    std::map<char, Color> colorPalette;
    std::vector<std::vector<char>> canvas;
    
    // UI options
    bool showColorNames;
    bool showKeyboardShortcuts;
    
    // Initialize all available tools
    void initializeTools() {
        tools.push_back(std::make_unique<BrushTool>());
        tools.push_back(std::make_unique<EraserTool>());
        tools.push_back(std::make_unique<LineArtTool>());
        tools.push_back(std::make_unique<RainbowTool>());
        tools.push_back(std::make_unique<StampTool>());
        tools.push_back(std::make_unique<PatternTool>());
        tools.push_back(std::make_unique<GradientTool>());
        tools.push_back(std::make_unique<FillTool>());
        tools.push_back(std::make_unique<SpecialEffectsTool>());
        tools.push_back(std::make_unique<TextTool>());
        
        for (auto& tool : tools) {
            std::cout << FG_GREEN << ICON_CHECK << " " << "Created paint tool: " << tool->getName() << RESET << std::endl;
        }
        
        // Set initial tool
        currentTool = tools[0].get();
    }
    
    // Set up initial color palette
    void initializeColorPalette() {
        colorPalette['1'] = Color("*", FG_RED, "Red");
        colorPalette['2'] = Color("*", FG_GREEN, "Green");
        colorPalette['3'] = Color("*", FG_BLUE, "Blue");
        colorPalette['4'] = Color("*", FG_YELLOW, "Yellow");
        colorPalette['5'] = Color("*", FG_MAGENTA, "Magenta");
        colorPalette['6'] = Color("*", FG_CYAN, "Cyan");
        colorPalette['7'] = Color("*", FG_WHITE, "White");
        colorPalette['8'] = Color("*", FG_BRIGHT_RED, "Bright Red");
        colorPalette['9'] = Color("*", FG_BRIGHT_GREEN, "Bright Green");
        colorPalette['0'] = Color(" ", RESET, "Eraser");
    }
    
    // Process user commands
    bool processCommand(const std::string& cmdLine) {
        std::istringstream cmd(cmdLine);
        std::string command;
        cmd >> command;
        
        if (command.empty()) return true;
        
        char commandChar = command[0];
        
        switch (commandChar) {
            // Tool selection
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '0': {
                int toolIndex = commandChar - '1';
                if (toolIndex >= 0 && toolIndex < static_cast<int>(tools.size())) {
                    currentTool = tools[toolIndex].get();
                    std::cout << "Selected tool: " << currentTool->getName() << std::endl;
                    
                    // Show contextual help for the selected tool
                    std::string tooltipContext = getLowercaseToolName();
                    TooltipManager::getInstance().showTooltip(tooltipContext);
                    
                } else if (commandChar == '0') {
                    // Shortcut for selecting eraser tool
                    for (size_t i = 0; i < tools.size(); i++) {
                        if (tools[i]->getName() == "Eraser") {
                            currentTool = tools[i].get();
                            std::cout << "Selected tool: " << currentTool->getName() << std::endl;
                            
                            // Show eraser tool tooltip
                            TooltipManager::getInstance().showTooltip("eraser");
                            break;
                        }
                    }
                }
                break;
            }
            
            // Color selection
            case '#': {
                if (command.length() == 7) { // #RRGGBB format
                    try {
                        int r = std::stoi(command.substr(1, 2), nullptr, 16);
                        int g = std::stoi(command.substr(3, 2), nullptr, 16);
                        int b = std::stoi(command.substr(5, 2), nullptr, 16);
                        
                        RGB rgb(r / 255.0f, g / 255.0f, b / 255.0f);
                        std::string ansiColor = rgb.toAnsiColor();
                        std::string colorName = rgb.getColorName();
                        
                        if (currentTool) {
                            currentTool->setColor(Color("*", ansiColor, colorName));
                            std::cout << "Color set to: " << command << std::endl;
                        }
                    } catch (...) {
                        std::cout << "Invalid hex color format. Use #RRGGBB." << std::endl;
                    }
                } else {
                    std::cout << "Invalid hex color format. Use #RRGGBB." << std::endl;
                }
                break;
            }
            
            // Brush size adjustment
            case '+': {
                if (currentTool) {
                    currentTool->setSize(currentTool->getSize() + 0.5f);
                    std::cout << "Increased " << currentTool->getName() 
                              << " size to " << currentTool->getSize() << std::endl;
                }
                break;
            }
            case '-': {
                if (currentTool) {
                    currentTool->setSize(currentTool->getSize() - 0.5f);
                    std::cout << "Decreased " << currentTool->getName() 
                              << " size to " << currentTool->getSize() << std::endl;
                }
                break;
            }
            
            // Painting commands
            case 'p': case 'P': {
                // Show painting technique tooltip for current tool
                TooltipManager::getInstance().showTooltip("painting_technique_" + getLowercaseToolName());
                
                int x, y;
                if (cmd >> x >> y) {
                    Layer* layer = project.getCurrentLayer();
                    if (layer && currentTool) {
                        // Check if the user wants animation
                        bool useAnimation = false;
                        bool useEffects = false;
                        
                        std::string option;
                        if (cmd >> option) {
                            useAnimation = (option == "a" || option == "animation");
                            useEffects = (option == "e" || option == "effects");
                        }
                        
                        std::cout << "Painting at " << x << "," << y 
                                  << " with " << currentTool->getName();
                        
                        if (useAnimation) {
                            std::cout << " (with animation)";
                            currentTool->applyWithAnimation(*layer, x, y);
                        } else if (useEffects) {
                            std::cout << " (with effects)";
                            if (auto* effectsTool = dynamic_cast<SpecialEffectsTool*>(currentTool)) {
                                effectsTool->apply(*layer, x, y);
                            } else {
                                currentTool->applyWithEffects(*layer, x, y);
                            }
                        } else {
                            std::cout << " (size: " << currentTool->getSize() << ")";
                            currentTool->apply(*layer, x, y);
                        }
                        
                        std::cout << std::endl;
                    } else {
                        std::cout << "No active layer or tool selected!" << std::endl;
                    }
                } else {
                    std::cout << "Invalid coordinates! Use format: p x y" << std::endl;
                }
                break;
            }
            
            // Clear current layer
            case 'c': case 'C': {
                Layer* layer = project.getCurrentLayer();
                if (layer) {
                    std::cout << "Clearing layer: " << layer->getName() << std::endl;
                    layer->clear();
                } else {
                    std::cout << "No active layer to clear!" << std::endl;
                }
                break;
            }
            
            // New layer
            case 'n': case 'N': {
                // Show layer management tooltip
                TooltipManager::getInstance().showTooltip("layers");
                
                std::string name;
                std::getline(cmd, name);
                name = name.empty() ? "Layer " + std::to_string(project.getLayers().size() + 1) : name.substr(1); // Skip leading space
                
                Layer* layer = project.addLayer(name);
                if (layer) {
                    std::cout << "Created new layer: " << name << std::endl;
                } else {
                    std::cout << "Failed to create layer!" << std::endl;
                }
                break;
            }
            
            // List layers
            case 'l': case 'L': {
                const auto& layers = project.getLayers();
                if (layers.empty()) {
                    std::cout << "No layers defined!" << std::endl;
                } else {
                    std::cout << "Layers:" << std::endl;
                    for (size_t i = 0; i < layers.size(); i++) {
                        std::cout << (i == project.getCurrentLayer()->getName().size() ? "[SELECTED]  " : "  ")
                                  << (layers[i]->isVisible() ? "● " : "○ ")
                                  << layers[i]->getName() << " (Index: " << i << ")" << std::endl;
                    }
                }
                break;
            }
            
            // Select layer
            case 's': case 'S': {
                size_t index;
                if (cmd >> index) {
                    if (project.selectLayer(index)) {
                        std::cout << "Selected layer: " << project.getCurrentLayer()->getName() 
                                  << " (Index: " << index << ")" << std::endl;
                    } else {
                        std::cout << "Invalid layer index: " << index << std::endl;
                    }
                } else {
                    std::cout << "Invalid layer index! Use format: s <index>" << std::endl;
                }
                break;
            }
            
            // Toggle layer visibility
            case 't': case 'T': {
                size_t index;
                if (cmd >> index) {
                    if (project.toggleLayerVisibility(index)) {
                        // Visibility message printed by the toggle function
                    } else {
                        std::cout << "Invalid layer index: " << index << std::endl;
                    }
                } else {
                    std::cout << "Invalid layer index! Use format: t <index>" << std::endl;
                }
                break;
            }
            
            // View canvas
            case 'v': case 'V': {
                // Show view shortcuts tooltip
                TooltipManager::getInstance().showTooltip("view_shortcuts");
                
                displayCanvas();
                break;
            }
            
            // Help
            case 'h': case 'H': {
                // Show keyboard shortcuts tooltip
                TooltipManager::getInstance().showTooltip("shortcuts");
                
                displayHelp();
                break;
            }
            
            // Run demo
            case 'd': case 'D': {
                runDemo();
                break;
            }
            
            // Open color wheel
            case 'w': case 'W': {
                openColorWheel();
                break;
            }
            
            // Export project
            case 'x': case 'X': {
                exportProject();
                break;
            }
            
            // Animation toggle
            case 'a': case 'A': {
                if (currentTool) {
                    if (auto* tool = dynamic_cast<PaintTool*>(currentTool)) {
                        tool->toggleAnimation();
                    } else {
                        std::cout << "Current tool doesn't support animation." << std::endl;
                    }
                }
                break;
            }
            
            // Effects toggle
            case 'e': case 'E': {
                if (currentTool) {
                    if (auto* tool = dynamic_cast<PaintTool*>(currentTool)) {
                        tool->toggleEffects();
                    } else {
                        std::cout << "Current tool doesn't support effects." << std::endl;
                    }
                }
                break;
            }
            
            // Set text for TextTool
            case 'f': case 'F': {
                TextTool* textTool = nullptr;
                for (auto& tool : tools) {
                    if (tool->getName() == "Text") {
                        textTool = dynamic_cast<TextTool*>(tool.get());
                        break;
                    }
                }
                
                if (textTool) {
                    std::string text;
                    std::getline(cmd, text);
                    if (!text.empty()) {
                        // Skip leading space
                        text = text.substr(1);
                        textTool->setText(text);
                    } else {
                        std::cout << "Current text: \"" << textTool->getText() << "\"" << std::endl;
                        std::cout << "Enter new text: ";
                        std::getline(std::cin, text);
                        textTool->setText(text);
                    }
                } else {
                    std::cout << "Text tool not available!" << std::endl;
                }
                break;
            }
            
            // Toggle UI options
            case 'o': case 'O': {
                std::string option;
                if (cmd >> option) {
                    if (option == "color" || option == "c") {
                        showColorNames = !showColorNames;
                        std::cout << "Color names " << (showColorNames ? "enabled" : "disabled") << std::endl;
                    } else if (option == "shortcuts" || option == "s") {
                        showKeyboardShortcuts = !showKeyboardShortcuts;
                        std::cout << "Keyboard shortcuts " << (showKeyboardShortcuts ? "enabled" : "disabled") << std::endl;
                    } else {
                        std::cout << "Unknown option. Available options: color, shortcuts" << std::endl;
                    }
                } else {
                    std::cout << "UI Options:" << std::endl;
                    std::cout << "  Color names: " << (showColorNames ? "enabled" : "disabled") << std::endl;
                    std::cout << "  Keyboard shortcuts: " << (showKeyboardShortcuts ? "enabled" : "disabled") << std::endl;
                }
                break;
            }
            
            // Quit
            case 'q': case 'Q': {
                std::cout << "Are you sure you want to quit? (y/n): ";
                std::string confirm;
                std::cin >> confirm;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (confirm == "y" || confirm == "Y") {
                    return false; // Exit the application
                }
                break;
            }
            
            default: {
                std::cout << "Unknown command: " << command << std::endl;
                std::cout << "Type 'h' for help." << std::endl;
                break;
            }
        }
        
        return true;
    }
    
    // Display application status
    void displayStatus() {
        std::cout << FG_BRIGHT_BLUE << BOLD << "\n===============================" << RESET << std::endl;
        
        // Display current tool
        if (currentTool) {
            std::cout << "Current Tool: " << FG_BRIGHT_GREEN << currentTool->getName() << RESET << std::endl;
            std::cout << "Brush Size: " << FG_BRIGHT_YELLOW << currentTool->getSize() << RESET << std::endl;
            
            // Display current color if the tool uses one
            const Color& color = currentTool->getColor();
            std::cout << "Color: " << color.getAnsiColor() << color.getName() << " "
                      << color.getSymbol() << RESET << std::endl;
            
            // Display special tool status if available
            if (auto* stampTool = dynamic_cast<StampTool*>(currentTool)) {
                // Could display current stamp pattern here
            } else if (auto* patternTool = dynamic_cast<PatternTool*>(currentTool)) {
                // Could display current pattern type here
            } else if (auto* effectsTool = dynamic_cast<SpecialEffectsTool*>(currentTool)) {
                std::cout << "Current Effect: " << FG_BRIGHT_MAGENTA 
                          << effectsTool->getCurrentEffectName() << RESET << std::endl;
            } else if (auto* textTool = dynamic_cast<TextTool*>(currentTool)) {
                std::cout << "Current Text: \"" << FG_BRIGHT_WHITE 
                          << textTool->getText() << RESET << "\"" << std::endl;
            }
            
            if (dynamic_cast<PaintTool*>(currentTool)) {
                std::cout << "Animation: " << (dynamic_cast<PaintTool*>(currentTool)->hasAnimation() ? 
                                            FG_GREEN + std::string("ON") : 
                                            FG_RED + std::string("OFF")) << RESET << std::endl;
                
                std::cout << "Effects: " << (dynamic_cast<PaintTool*>(currentTool)->hasEffects() ? 
                                          FG_GREEN + std::string("ON") : 
                                          FG_RED + std::string("OFF")) << RESET << std::endl;
            }
        }
        
        // Display current layer
        Layer* layer = project.getCurrentLayer();
        if (layer) {
            std::cout << "Layer: " << FG_BRIGHT_CYAN << layer->getName() << RESET 
                      << " (Index: " << project.getLayers().size() - 1 << ")" << std::endl;
        }
    }
    
    // Display help text
    void displayHelp() {
        std::cout << std::endl;
        std::cout << BOLD << BG_BLUE << FG_WHITE << "                3D MODEL PAINTER HELP                " << RESET << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Tool Selection:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-" << tools.size() << RESET << "      - Select tools (";
        for (size_t i = 0; i < tools.size(); i++) {
            std::cout << tools[i]->getName();
            if (i < tools.size() - 1) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Brush Settings:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "+, -" << RESET << "     - Increase/decrease brush size" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-9, 0" << RESET << "   - Select color (0 = eraser)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "#RRGGBB" << RESET << "  - Set custom color in hex format" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Painting Commands:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y" << RESET << "    - Paint at position (x,y)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y a" << RESET << "  - Paint with animation" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y e" << RESET << "  - Paint with effects" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "c" << RESET << "        - Clear current layer" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Layer Management:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "n [name]" << RESET << " - New layer with optional name" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "l" << RESET << "        - List layers" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s index" << RESET << "  - Select layer by index" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "t index" << RESET << "  - Toggle layer visibility" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Special Tool Commands:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "a" << RESET << "        - Toggle animation for current tool" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "e" << RESET << "        - Toggle effects for current tool" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "f [text]" << RESET << " - Set text for Text tool" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Advanced Features:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "w" << RESET << "        - Open enhanced color wheel with harmony suggestions for professional color selection" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "x" << RESET << "        - Export project (TXT, PPM, SVG, OBJ)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "o [option]" << RESET << " - Toggle UI options (color, shortcuts)" << std::endl;
        
        std::cout << BOLD << FG_BRIGHT_GREEN << "Application Controls:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "v" << RESET << "        - View canvas" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "h" << RESET << "        - Show this help" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "d" << RESET << "        - Run demo" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "        - Quit" << std::endl;
    }
    
    // Display the current canvas with all layers
    void displayCanvas() {
        const auto& layers = project.getLayers();
        Layer* currentLayer = project.getCurrentLayer();
        
        if (layers.empty()) {
            std::cout << "No layers to display!" << std::endl;
            return;
        }
        
        std::cout << "=== CANVAS ===" << std::endl;
        
        // Display each visible layer
        for (const auto& layer : layers) {
            if (!layer->isVisible()) {
                std::cout << (layer.get() == currentLayer ? "[SELECTED]  " : "  ")
                          << "○ " << layer->getName() << " (Hidden)" << std::endl;
                continue;
            }
            
            std::cout << (layer.get() == currentLayer ? "[SELECTED]  " : "  ")
                      << "● " << layer->getName() << std::endl;
            
            const Texture& texture = layer->getTexture();
            int width = texture.getWidth();
            int height = texture.getHeight();
            
            // Print column numbers
            std::cout << "            ";
            for (int x = 0; x < width; x += 10) {
                std::cout << std::setw(10) << x;
            }
            std::cout << std::endl;
            
            std::cout << "   ";
            for (int x = 0; x < width; x++) {
                std::cout << x % 10;
            }
            std::cout << std::endl;
            
            // Print top border
            std::cout << "  +" << std::string(width, '-') << "+" << std::endl;
            
            // Print rows with pixel content
            for (int y = 0; y < height; y++) {
                std::cout << std::setw(2) << y << "|";
                
                for (int x = 0; x < width; x++) {
                    const Color& pixel = texture.getPixel(x, y);
                    std::cout << pixel.toString();
                }
                
                std::cout << "|" << std::endl;
            }
            
            // Print bottom border
            std::cout << "  +" << std::string(width, '-') << "+" << std::endl;
        }
    }
    
    // Open the interactive color wheel with harmony suggestions
    void openColorWheel() {
        if (!currentTool) return;
        
        // Show tooltip for color wheel feature
        TooltipManager::getInstance().showTooltip("color_wheel");
        
        // Get current RGB color from the tool
        const Color& toolColor = currentTool->getColor();
        std::string colorName = toolColor.getName();
        
        // Get initial HSV values based on color name to start with something sensible
        float h = 0.0f;   // Default red hue
        float s = 1.0f;   // Full saturation
        float v = 1.0f;   // Full brightness
        
        // Approximate hue values based on color name
        if (colorName.find("Red") != std::string::npos) {
            h = 0.0f;
        } else if (colorName.find("Orange") != std::string::npos) {
            h = 30.0f;
        } else if (colorName.find("Yellow") != std::string::npos) {
            h = 60.0f;
        } else if (colorName.find("Green") != std::string::npos) {
            h = 120.0f;
        } else if (colorName.find("Cyan") != std::string::npos) {
            h = 180.0f;
        } else if (colorName.find("Blue") != std::string::npos) {
            h = 240.0f;
        } else if (colorName.find("Purple") != std::string::npos) {
            h = 270.0f;
        } else if (colorName.find("Magenta") != std::string::npos) {
            h = 300.0f;
        }
        
        // Create a color wheel with either the estimated or default HSV values
        InteractiveColorWheel colorWheel(h, s, v);
        
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "☆ Opening Color Wheel with Harmony Suggestions ☆" << RESET << std::endl;
        std::cout << "Use h+/h- to adjust hue, s+/s- for saturation, v+/v- for brightness" << std::endl;
        std::cout << "Press 'n/p' to cycle through different harmony types" << std::endl;
        std::cout << "Press numbers 1-5 to select colors from the current harmony" << std::endl;
        std::cout << "Press 'a' to toggle between simplified and advanced visualization" << std::endl;
        
        // Run the color wheel UI
        color_wheel::Color selectedColor = colorWheel.run();
        
        // Convert back to our RGB format
        RGB newRgb(selectedColor.r / 255.0f, selectedColor.g / 255.0f, selectedColor.b / 255.0f);
        
        // Apply to the current tool
        std::string ansiCode = newRgb.toAnsiColor();
        std::string newColorName = newRgb.getColorName();
        
        Color newColor(toolColor.getSymbol(), ansiCode, newColorName);
        currentTool->setColor(newColor);
        
        std::cout << BOLD << FG_GREEN << "✓ " << RESET 
                  << "Color updated! Selected " << newColorName << " (" 
                  << selectedColor.getHexCode() << ")" << std::endl;
        
        // Display a general color theory tip since we don't have direct access to the harmonyName
        std::cout << FG_BRIGHT_CYAN << "ℹ Color Tip: " << RESET 
                  << "Color harmony creates visually appealing combinations. "
                  << "Try complementary colors for contrast or analogous colors for a harmonious feel." << std::endl;
    }
    
    // Helper method to get lowercase tool name for tooltip context
    std::string getLowercaseToolName() const {
        if (!currentTool) return "";
        
        std::string name = currentTool->getName();
        std::transform(name.begin(), name.end(), name.begin(), 
                    [](unsigned char c){ return std::tolower(c); });
        
        // Remove "tool" suffix if present
        const std::string suffix = "tool";
        if (name.size() > suffix.size() && 
            name.substr(name.size() - suffix.size()) == suffix) {
            name = name.substr(0, name.size() - suffix.size());
        }
        
        return name;
    }
    
    // Export project in various formats
    void exportProject() {
        std::cout << "Export format:" << std::endl;
        std::cout << "1. Text file (.txt)" << std::endl;
        std::cout << "2. PPM image (.ppm)" << std::endl;
        std::cout << "3. SVG vector graphic (.svg)" << std::endl;
        std::cout << "4. 3D model (.obj)" << std::endl;
        
        // Show export tooltip with best practices
        TooltipManager::getInstance().showTooltip("export");
        
        std::cout << "Select format (1-4): ";
        
        int format;
        std::cin >> format;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::string filename;
        std::cout << "Enter filename (without extension): ";
        std::getline(std::cin, filename);
        
        if (filename.empty()) {
            filename = "export_" + std::to_string(std::time(nullptr));
        }
        
        bool success = false;
        
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) {
            std::cout << "No active layer to export!" << std::endl;
            return;
        }
        
        switch (format) {
            case 1: // TXT
                success = exportAsTXT(filename + ".txt");
                break;
            case 2: // PPM
                success = exportAsPPM(filename + ".ppm");
                break;
            case 3: // SVG
                success = exportAsSVG(filename + ".svg");
                break;
            case 4: // OBJ
                success = exportAsOBJ(filename + ".obj", filename + ".mtl");
                break;
            default:
                std::cout << "Invalid format selection!" << std::endl;
                return;
        }
        
        if (success) {
            std::cout << "Export completed successfully!" << std::endl;
        } else {
            std::cout << "Export failed!" << std::endl;
        }
    }
    
    // Export as a text file
    bool exportAsTXT(const std::string& filename) {
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) return false;
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to create file: " << filename << std::endl;
            return false;
        }
        
        const Texture& texture = currentLayer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // Write dimensions
        file << width << " " << height << std::endl;
        
        // Write layer name
        file << currentLayer->getName() << std::endl;
        
        // Write pixels
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& pixel = texture.getPixel(x, y);
                file << pixel.getSymbol();
            }
            file << std::endl;
        }
        
        std::cout << "Exported text file: " << filename << std::endl;
        return true;
    }
    
    // Export as a PPM image
    bool exportAsPPM(const std::string& filename) {
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) return false;
        
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to create file: " << filename << std::endl;
            return false;
        }
        
        const Texture& texture = currentLayer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // PPM header
        file << "P3\n" << width << " " << height << "\n255\n";
        
        // Convert symbols to RGB values
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& pixel = texture.getPixel(x, y);
                
                // Simple conversion (this is just a placeholder)
                // In a real implementation, you'd extract the RGB values from the ANSI color
                if (pixel.getAnsiColor() == RESET) {
                    file << "255 255 255 "; // White for empty spaces
                } else if (pixel.getAnsiColor() == FG_RED) {
                    file << "255 0 0 "; // Red
                } else if (pixel.getAnsiColor() == FG_GREEN) {
                    file << "0 255 0 "; // Green
                } else if (pixel.getAnsiColor() == FG_BLUE) {
                    file << "0 0 255 "; // Blue
                } else if (pixel.getAnsiColor() == FG_YELLOW) {
                    file << "255 255 0 "; // Yellow
                } else if (pixel.getAnsiColor() == FG_MAGENTA) {
                    file << "255 0 255 "; // Magenta
                } else if (pixel.getAnsiColor() == FG_CYAN) {
                    file << "0 255 255 "; // Cyan
                } else {
                    file << "128 128 128 "; // Gray for unknown colors
                }
            }
            file << "\n";
        }
        
        std::cout << "Exported PPM image: " << filename << std::endl;
        return true;
    }
    
    // Export as an SVG vector graphic
    bool exportAsSVG(const std::string& filename) {
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) return false;
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to create file: " << filename << std::endl;
            return false;
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
        
        // Grid lines
        file << "  <!-- Grid lines -->\n";
        file << "  <g stroke=\"#ddd\" stroke-width=\"1\">\n";
        
        for (int x = 0; x <= width; x++) {
            int pos = x * cellSize;
            file << "    <line x1=\"" << pos << "\" y1=\"0\" x2=\"" << pos 
                 << "\" y2=\"" << svgHeight << "\" />\n";
        }
        
        for (int y = 0; y <= height; y++) {
            int pos = y * cellSize;
            file << "    <line x1=\"0\" y1=\"" << pos << "\" x2=\"" << svgWidth 
                 << "\" y2=\"" << pos << "\" />\n";
        }
        
        file << "  </g>\n\n";
        
        // Pixel data
        file << "  <!-- Pixel data -->\n";
        file << "  <g>\n";
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& pixel = texture.getPixel(x, y);
                
                // Skip empty pixels
                if (pixel.getSymbol() == " " || pixel.getAnsiColor() == RESET) continue;
                
                // Determine color
                std::string color = "#000000"; // Default
                
                if (pixel.getAnsiColor() == FG_RED) color = "#ff0000";
                else if (pixel.getAnsiColor() == FG_GREEN) color = "#00ff00";
                else if (pixel.getAnsiColor() == FG_BLUE) color = "#0000ff";
                else if (pixel.getAnsiColor() == FG_YELLOW) color = "#ffff00";
                else if (pixel.getAnsiColor() == FG_MAGENTA) color = "#ff00ff";
                else if (pixel.getAnsiColor() == FG_CYAN) color = "#00ffff";
                else if (pixel.getAnsiColor() == FG_WHITE) color = "#ffffff";
                
                // Draw rectangle for the pixel
                int px = x * cellSize;
                int py = y * cellSize;
                
                file << "    <rect x=\"" << px << "\" y=\"" << py 
                     << "\" width=\"" << cellSize << "\" height=\"" << cellSize 
                     << "\" fill=\"" << color << "\" />\n";
                
                // Add the symbol as text if it's not just a color block
                if (pixel.getSymbol() != "*" && pixel.getSymbol() != "#") {
                    file << "    <text x=\"" << (px + cellSize/2) << "\" y=\"" << (py + cellSize/2) 
                         << "\" font-size=\"" << (cellSize * 0.8) << "\" fill=\"black\" "
                         << "text-anchor=\"middle\" dominant-baseline=\"middle\">"
                         << pixel.getSymbol() << "</text>\n";
                }
            }
        }
        
        file << "  </g>\n";
        
        // Close SVG
        file << "</svg>\n";
        
        std::cout << "Exported SVG file: " << filename << std::endl;
        return true;
    }
    
    // Export as a 3D OBJ model
    bool exportAsOBJ(const std::string& objFilename, const std::string& mtlFilename) {
        Layer* currentLayer = project.getCurrentLayer();
        if (!currentLayer) return false;
        
        // Create the MTL file first
        std::ofstream mtlFile(mtlFilename);
        if (!mtlFile.is_open()) {
            std::cerr << "Failed to create file: " << mtlFilename << std::endl;
            return false;
        }
        
        // Write material definitions
        mtlFile << "# 3D Model Painter material file\n\n";
        
        mtlFile << "newmtl red\n";
        mtlFile << "Kd 1.0 0.0 0.0\n";
        
        mtlFile << "newmtl green\n";
        mtlFile << "Kd 0.0 1.0 0.0\n";
        
        mtlFile << "newmtl blue\n";
        mtlFile << "Kd 0.0 0.0 1.0\n";
        
        mtlFile << "newmtl yellow\n";
        mtlFile << "Kd 1.0 1.0 0.0\n";
        
        mtlFile << "newmtl magenta\n";
        mtlFile << "Kd 1.0 0.0 1.0\n";
        
        mtlFile << "newmtl cyan\n";
        mtlFile << "Kd 0.0 1.0 1.0\n";
        
        mtlFile << "newmtl white\n";
        mtlFile << "Kd 1.0 1.0 1.0\n";
        
        mtlFile << "newmtl black\n";
        mtlFile << "Kd 0.0 0.0 0.0\n";
        
        mtlFile.close();
        
        // Now create the OBJ file
        std::ofstream objFile(objFilename);
        if (!objFile.is_open()) {
            std::cerr << "Failed to create file: " << objFilename << std::endl;
            return false;
        }
        
        // Get the basename of the MTL file for include
        std::string mtlBasename = mtlFilename;
        size_t lastSlash = mtlBasename.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            mtlBasename = mtlBasename.substr(lastSlash + 1);
        }
        
        objFile << "# 3D Model Painter OBJ file\n";
        objFile << "mtllib " << mtlBasename << "\n\n";
        
        const Texture& texture = currentLayer->getTexture();
        int width = texture.getWidth();
        int height = texture.getHeight();
        
        // Create a simple height field based on the texture
        // First, define all vertices
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Color& pixel = texture.getPixel(x, y);
                
                // Skip empty pixels or give them lower height
                float z = 0.0f;
                if (pixel.getSymbol() != " " && pixel.getAnsiColor() != RESET) {
                    z = 0.5f; // Higher for non-empty pixels
                }
                
                // OBJ vertices are 1-indexed
                objFile << "v " << x << " " << (height - y - 1) << " " << z << "\n";
            }
        }
        
        // Add texture coordinates
        for (int y = 0; y < height; y++) {
            float v = y / static_cast<float>(height);
            for (int x = 0; x < width; x++) {
                float u = x / static_cast<float>(width);
                objFile << "vt " << u << " " << v << "\n";
            }
        }
        
        // Add faces with materials
        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                // Get the color of the current pixel
                const Color& pixel = texture.getPixel(x, y);
                
                // Skip empty pixels
                if (pixel.getSymbol() == " " || pixel.getAnsiColor() == RESET) continue;
                
                // Determine material
                std::string material = "white"; // Default
                
                if (pixel.getAnsiColor() == FG_RED) material = "red";
                else if (pixel.getAnsiColor() == FG_GREEN) material = "green";
                else if (pixel.getAnsiColor() == FG_BLUE) material = "blue";
                else if (pixel.getAnsiColor() == FG_YELLOW) material = "yellow";
                else if (pixel.getAnsiColor() == FG_MAGENTA) material = "magenta";
                else if (pixel.getAnsiColor() == FG_CYAN) material = "cyan";
                else if (pixel.getAnsiColor() == FG_WHITE) material = "white";
                
                // Use the material
                objFile << "usemtl " << material << "\n";
                
                // Calculate vertex indices
                int v1 = y * width + x + 1;          // Top left
                int v2 = y * width + (x + 1) + 1;    // Top right
                int v3 = (y + 1) * width + (x + 1) + 1; // Bottom right
                int v4 = (y + 1) * width + x + 1;    // Bottom left
                
                // Create two triangles to form a quad
                objFile << "f " << v1 << "/" << v1 << " " 
                        << v2 << "/" << v2 << " " 
                        << v3 << "/" << v3 << "\n";
                
                objFile << "f " << v1 << "/" << v1 << " " 
                        << v3 << "/" << v3 << " " 
                        << v4 << "/" << v4 << "\n";
            }
        }
        
        std::cout << "Exported OBJ file: " << objFilename << std::endl;
        std::cout << "Exported MTL file: " << mtlFilename << std::endl;
        return true;
    }
    
    // Draw a demo scene with various tools
    void drawDemoScene(Layer& layer) {
        std::cout << "⧗ Creating a fun demo scene..." << std::endl;
        
        // Create a colorful border
        LineArtTool borderTool;
        borderTool.setColor(Color("#", FG_BRIGHT_BLUE, "Border"));
        
        for (int i = 0; i < 4; i++) {
            HSV borderHsv(i * 90.0f, 0.8f, 0.9f);
            RGB borderRgb = HSVtoRGB(borderHsv);
            std::string borderColor = borderRgb.toAnsiColor();
            
            // Create a different color for each side
            borderTool.setColor(Color("#", borderColor, "Border" + std::to_string(i)));
            
            // Draw sides of a rectangle
            switch (i) {
                case 0: // Top
                    for (int x = 5; x < 25; x++) {
                        borderTool.apply(layer, x, 2);
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    break;
                case 1: // Right
                    for (int y = 2; y < 13; y++) {
                        borderTool.apply(layer, 24, y);
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    break;
                case 2: // Bottom
                    for (int x = 24; x >= 5; x--) {
                        borderTool.apply(layer, x, 12);
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    break;
                case 3: // Left
                    for (int y = 12; y >= 2; y--) {
                        borderTool.apply(layer, 5, y);
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    break;
            }
        }
        
        // Add some patterns and shapes in the center
        RainbowTool rainbowTool;
        rainbowTool.setSize(0.5f);
        
        // Create a sunburst pattern
        std::cout << "⧗ Adding colorful effects..." << std::endl;
        for (int i = 0; i < 8; i++) {
            float angle = i * (3.14159f / 4.0f);
            int targetX = 15 + static_cast<int>(cos(angle) * 5);
            int targetY = 7 + static_cast<int>(sin(angle) * 3);
            
            rainbowTool.apply(layer, targetX, targetY);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Add some stamps
        StampTool stampTool;
        stampTool.setColor(Color(ICON_HEART, FG_BRIGHT_RED, "Hearts"));
        stampTool.apply(layer, 10, 5);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        stampTool.nextShape();
        stampTool.setColor(Color(ICON_DIAMOND, FG_BRIGHT_CYAN, "Diamonds"));
        stampTool.apply(layer, 7, 9);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Add some special effects
        SpecialEffectsTool effectsTool;
        effectsTool.apply(layer, 20, 7);
        
        std::cout << "⧗ Adding final touches..." << std::endl;
        
        // Add some random rainbow dots
        for (int i = 0; i < 6; i++) {
            int x = 5 + std::rand() % 20;
            int y = 3 + std::rand() % 10;
            rainbowTool.apply(layer, x, y);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

int main() {
    // Set random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create and run the application
    Application app;
    app.run();
    
    return 0;
}