#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <map>
#include <iomanip>
#include <cmath>
#include <random>
#include <functional>
#include <fstream>
#include <sstream>

// Fun 3D Model Painter with improved UX and cool effects
// ANSI color and styling codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"
#define HIDDEN      "\033[8m"

// Foreground colors
#define FG_BLACK    "\033[30m"
#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"

// Bright foreground colors
#define FG_BRIGHT_BLACK    "\033[90m"
#define FG_BRIGHT_RED      "\033[91m"
#define FG_BRIGHT_GREEN    "\033[92m"
#define FG_BRIGHT_YELLOW   "\033[93m"
#define FG_BRIGHT_BLUE     "\033[94m"
#define FG_BRIGHT_MAGENTA  "\033[95m"
#define FG_BRIGHT_CYAN     "\033[96m"
#define FG_BRIGHT_WHITE    "\033[97m"

// Background colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

// Bright background colors
#define BG_BRIGHT_BLACK    "\033[100m"
#define BG_BRIGHT_RED      "\033[101m"
#define BG_BRIGHT_GREEN    "\033[102m"
#define BG_BRIGHT_YELLOW   "\033[103m"
#define BG_BRIGHT_BLUE     "\033[104m"
#define BG_BRIGHT_MAGENTA  "\033[105m"
#define BG_BRIGHT_CYAN     "\033[106m"
#define BG_BRIGHT_WHITE    "\033[107m"

// Fun characters and symbols
const std::vector<std::string> EFFECTS = {
    "✨", "✻", "★", "☆", "✸", "✿", "❀", "❁", "❂", "❃", "❄", "❆",
    "♦", "♣", "♠", "♥", "▲", "◆", "●", "■", "◉", "◊", "○", "□"
};

// Cool art brushes
const std::vector<std::string> ART_BRUSHES = {
    "█", "▓", "▒", "░", "▄", "▌", "▐", "▀", "▖", "▗", "▘", "▙", "▚", "▛", "▜", "▝", "▞", "▟"
};

// Helper function to clear screen
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// Helper function to animate text typing
void animateText(const std::string& text, int delayMs = 25) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << std::endl;
}

// Helper function to show a loading animation
void showLoadingAnimation(const std::string& message, int durationMs = 1000) {
    std::string dots = "";
    int iterations = durationMs / 200;
    
    std::cout << message;
    for (int i = 0; i < iterations; i++) {
        dots += ".";
        std::cout << FG_BRIGHT_CYAN << BOLD << dots << "\r" << message << RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << std::endl;
}

// Helper to get random color
std::string getRandomColor() {
    std::vector<std::string> colors = {
        FG_RED, FG_GREEN, FG_BLUE, FG_YELLOW, FG_MAGENTA, FG_CYAN,
        FG_BRIGHT_RED, FG_BRIGHT_GREEN, FG_BRIGHT_BLUE, 
        FG_BRIGHT_YELLOW, FG_BRIGHT_MAGENTA, FG_BRIGHT_CYAN
    };
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, colors.size() - 1);
    
    return colors[dis(gen)];
}

// Helper to get random effect symbol
std::string getRandomEffect() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, EFFECTS.size() - 1);
    
    return EFFECTS[dis(gen)];
}

// Helper to get random art brush
std::string getRandomArtBrush() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, ART_BRUSHES.size() - 1);
    
    return ART_BRUSHES[dis(gen)];
}

// Helper function to show a splash screen
void showSplashScreen() {
    clearScreen();
    
    std::string title = R"(
    ╔════════════════════════════════════════════════════════════╗
    ║                                                            ║
    ║     _____       __   __          __     _ ___              ║
    ║    |___ /  ___  \ \ / /___  ___ / _| __| | _ \__ _(_)_ _   ║
    ║      |_ \ / _ \  \ V // _ \/ __| |_ / _` |  _/ _` | | '_|  ║
    ║     ___) |  __/   | || (_) \__ \  _| (_| | || (_| | | | |  ║
    ║    |____/ \___|   |_| \___/|___/_|  \__,_|_| \__,_|_|_|    ║
    ║                                                            ║
    ║       ✨ The Most Colorful 3D Model Painter Ever! ✨        ║
    ║                                                            ║
    ╚════════════════════════════════════════════════════════════╝
    )";
    
    // Animate with random colors
    for (char c : title) {
        if (c != ' ' && c != '\n') {
            std::cout << getRandomColor() << c << RESET;
        } else {
            std::cout << c;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    std::cout << std::endl;
    
    // Show tagline
    std::string tagline = std::string(FG_BRIGHT_CYAN) + std::string(ITALIC) + "Creating beautiful 3D models has never been this fun!" + std::string(RESET);
    animateText(tagline, 30);
    
    std::cout << std::endl;
    
    // Show loading animation
    showLoadingAnimation("Loading your creative environment", 1500);
    
    // Clear after splash
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    clearScreen();
}

struct Color {
    std::string symbol;
    std::string code;
    std::string name;
    
    Color(const std::string& s = " ", const std::string& c = RESET, const std::string& n = "None") 
        : symbol(s), code(c), name(n) {}
};

// Enhanced pixel with color information and effects
struct Pixel {
    Color color;
    float opacity;
    bool hasEffect;
    std::string effect;
    
    Pixel() : opacity(0.0f), hasEffect(false) {}
    Pixel(const Color& c, float o = 1.0f, bool he = false, const std::string& e = "") 
        : color(c), opacity(o), hasEffect(he), effect(e) {}
};

class Texture {
public:
    Texture(int width, int height) : width(width), height(height), data(width * height) {
        std::cout << FG_BRIGHT_CYAN << "✓ " << BOLD << "Created texture: " << width << "x" << height << RESET << std::endl;
    }
    
    void clear() {
        std::fill(data.begin(), data.end(), Pixel());
        std::cout << FG_YELLOW << "✓ " << "Texture cleared" << RESET << std::endl;
    }
    
    void paint(int x, int y, const Color& color, float brushSize, float opacity = 1.0f, bool addEffect = false) {
        int r = static_cast<int>(brushSize);
        
        for (int i = -r; i <= r; i++) {
            for (int j = -r; j <= r; j++) {
                int px = x + i;
                int py = y + j;
                
                // Simple distance check for circular brush
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    float distance = std::sqrt(i*i + j*j);
                    if (distance <= r) {
                        // Apply falloff for smoother brush
                        float falloff = 1.0f - (distance / r);
                        float pixelOpacity = opacity * falloff * falloff;
                        
                        // Apply the pixel if it's more opaque than what's there
                        // or if we're using the eraser (space symbol)
                        Pixel& currentPixel = data[py * width + px];
                        
                        // Randomly decide whether to add an effect
                        bool useEffect = addEffect && (rand() % 100 < 15); // 15% chance
                        std::string effectSymbol = useEffect ? getRandomEffect() : "";
                        
                        if (color.symbol == " " || currentPixel.opacity < pixelOpacity) {
                            currentPixel = Pixel(color, pixelOpacity, useEffect, effectSymbol);
                        }
                    }
                }
            }
        }
    }
    
    void applyEffectAt(int x, int y, const std::string& effect, float size = 1.0f) {
        int r = static_cast<int>(size);
        
        // Only apply at the center for now
        if (x >= 0 && x < width && y >= 0 && y < height) {
            Pixel& pixel = data[y * width + x];
            pixel.hasEffect = true;
            pixel.effect = effect;
        }
    }
    
    void render(bool showEffects = true) const {
        // Create a top header with gradient
        std::cout << "   ";
        for (int x = 0; x < width; x++) {
            if (x % 5 == 0) {
                std::cout << FG_BRIGHT_CYAN << x / 10 << RESET;
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
        
        // Column numbers (ones)
        std::cout << "   ";
        for (int x = 0; x < width; x++) {
            std::cout << FG_CYAN << x % 10 << RESET;
        }
        std::cout << std::endl;
        
        // Draw top border with nice corners
        std::cout << "  " << FG_BRIGHT_WHITE << "+" << std::string(width, '-') << "+" << RESET << std::endl;
        
        // Draw texture content
        for (int y = 0; y < height; y++) {
            // Row numbers
            std::cout << FG_CYAN << std::setw(2) << y << FG_BRIGHT_WHITE << "|" << RESET;
            
            for (int x = 0; x < width; x++) {
                const Pixel& pixel = data[y * width + x];
                if (pixel.opacity > 0.1f) {
                    if (pixel.hasEffect && showEffects) {
                        std::cout << pixel.color.code << pixel.effect << RESET;
                    } else {
                        std::cout << pixel.color.code << pixel.color.symbol << RESET;
                    }
                } else {
                    std::cout << " ";
                }
            }
            std::cout << FG_BRIGHT_WHITE << "|" << RESET << std::endl;
        }
        
        // Draw bottom border with nice corners
        std::cout << "  " << FG_BRIGHT_WHITE << "+" << std::string(width, '-') << "+" << RESET << std::endl;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Export texture to a file in various formats
    bool exportToTxt(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return false;
        }
        
        // Write texture dimensions
        file << width << " " << height << std::endl;
        
        // Write pixel data
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Pixel& pixel = data[y * width + x];
                // Format: x y opacity symbol hasEffect effect
                file << x << " " << y << " " << pixel.opacity << " " 
                     << pixel.color.symbol << " " << (pixel.hasEffect ? "1" : "0") << " " 
                     << pixel.effect << std::endl;
            }
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported texture to TXT: " << filename << RESET << std::endl;
        return true;
    }
    
    bool exportToPpm(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return false;
        }
        
        // PPM header
        file << "P3\n" << width << " " << height << "\n255\n";
        
        // Convert each pixel to RGB
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Pixel& pixel = data[y * width + x];
                
                // Default to white background
                int r = 255, g = 255, b = 255;
                
                // Simple color mapping based on the symbol and code
                if (pixel.opacity > 0.1f) {
                    if (pixel.color.code.find("31") != std::string::npos || 
                        pixel.color.code.find("91") != std::string::npos) {
                        // Red
                        r = 255; g = 0; b = 0;
                    } else if (pixel.color.code.find("32") != std::string::npos ||
                               pixel.color.code.find("92") != std::string::npos) {
                        // Green
                        r = 0; g = 255; b = 0;
                    } else if (pixel.color.code.find("34") != std::string::npos ||
                               pixel.color.code.find("94") != std::string::npos) {
                        // Blue
                        r = 0; g = 0; b = 255;
                    } else if (pixel.color.code.find("33") != std::string::npos ||
                               pixel.color.code.find("93") != std::string::npos) {
                        // Yellow
                        r = 255; g = 255; b = 0;
                    } else if (pixel.color.code.find("35") != std::string::npos ||
                               pixel.color.code.find("95") != std::string::npos) {
                        // Magenta
                        r = 255; g = 0; b = 255;
                    } else if (pixel.color.code.find("36") != std::string::npos ||
                               pixel.color.code.find("96") != std::string::npos) {
                        // Cyan
                        r = 0; g = 255; b = 255;
                    } else if (pixel.color.code.find("30") != std::string::npos ||
                               pixel.color.code.find("90") != std::string::npos) {
                        // Black
                        r = 0; g = 0; b = 0;
                    }
                    
                    // Apply opacity
                    r = static_cast<int>(r * pixel.opacity);
                    g = static_cast<int>(g * pixel.opacity);
                    b = static_cast<int>(b * pixel.opacity);
                }
                
                file << r << " " << g << " " << b << " ";
            }
            file << "\n";
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported texture to PPM: " << filename << RESET << std::endl;
        return true;
    }
    
    bool exportToSvg(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open file for writing: " << filename << RESET << std::endl;
            return false;
        }
        
        // SVG header with proper dimensions
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
             << "<svg width=\"" << width * 10 << "\" height=\"" << height * 10 
             << "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
             << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
        
        // Draw each pixel as a rectangle
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Pixel& pixel = data[y * width + x];
                
                // Only draw non-transparent pixels
                if (pixel.opacity > 0.1f) {
                    std::string colorHex = "#FFFFFF"; // Default white
                    
                    // Map ANSI colors to hex
                    if (pixel.color.code.find("31") != std::string::npos || 
                        pixel.color.code.find("91") != std::string::npos) {
                        colorHex = "#FF0000"; // Red
                    } else if (pixel.color.code.find("32") != std::string::npos ||
                               pixel.color.code.find("92") != std::string::npos) {
                        colorHex = "#00FF00"; // Green
                    } else if (pixel.color.code.find("34") != std::string::npos ||
                               pixel.color.code.find("94") != std::string::npos) {
                        colorHex = "#0000FF"; // Blue
                    } else if (pixel.color.code.find("33") != std::string::npos ||
                               pixel.color.code.find("93") != std::string::npos) {
                        colorHex = "#FFFF00"; // Yellow
                    } else if (pixel.color.code.find("35") != std::string::npos ||
                               pixel.color.code.find("95") != std::string::npos) {
                        colorHex = "#FF00FF"; // Magenta
                    } else if (pixel.color.code.find("36") != std::string::npos ||
                               pixel.color.code.find("96") != std::string::npos) {
                        colorHex = "#00FFFF"; // Cyan
                    } else if (pixel.color.code.find("30") != std::string::npos ||
                               pixel.color.code.find("90") != std::string::npos) {
                        colorHex = "#000000"; // Black
                    }
                    
                    // Add a rectangle for this pixel
                    file << "<rect x=\"" << x * 10 << "\" y=\"" << y * 10 
                         << "\" width=\"10\" height=\"10\" fill=\"" << colorHex 
                         << "\" fill-opacity=\"" << pixel.opacity << "\"/>\n";
                    
                    // If pixel has an effect, add a circle or special shape
                    if (pixel.hasEffect) {
                        file << "<circle cx=\"" << (x * 10 + 5) << "\" cy=\"" << (y * 10 + 5) 
                             << "\" r=\"3\" fill=\"#FFFFFF\" fill-opacity=\"0.7\"/>\n";
                    }
                }
            }
        }
        
        // Close SVG
        file << "</svg>\n";
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported texture to SVG: " << filename << RESET << std::endl;
        return true;
    }
    
    bool exportToObjMtl(const std::string& baseFilename) const {
        // Create an OBJ file for the 3D model
        std::string objFilename = baseFilename + ".obj";
        std::string mtlFilename = baseFilename + ".mtl";
        
        std::ofstream objFile(objFilename);
        if (!objFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open OBJ file for writing: " << objFilename << RESET << std::endl;
            return false;
        }
        
        std::ofstream mtlFile(mtlFilename);
        if (!mtlFile.is_open()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Failed to open MTL file for writing: " << mtlFilename << RESET << std::endl;
            return false;
        }
        
        // Write OBJ header with material reference
        objFile << "# 3D Model Painter OBJ export\n";
        objFile << "mtllib " << mtlFilename << "\n";
        
        // Create a simple flat plane with width x height grid
        float scaleX = 1.0f / width;
        float scaleY = 1.0f / height;
        
        // Write vertices (points in 3D space)
        for (int y = 0; y <= height; y++) {
            for (int x = 0; x <= width; x++) {
                float xPos = (float)x * scaleX;
                float yPos = (float)(height - y) * scaleY; // Invert Y to match texture coords
                objFile << "v " << xPos << " " << yPos << " 0.0\n";
            }
        }
        
        // Write texture coordinates
        for (int y = 0; y <= height; y++) {
            for (int x = 0; x <= width; x++) {
                float u = (float)x / width;
                float v = (float)(height - y) / height; // Invert V
                objFile << "vt " << u << " " << v << "\n";
            }
        }
        
        // Write material
        mtlFile << "# 3D Model Painter MTL export\n";
        mtlFile << "newmtl TextureMaterial\n";
        mtlFile << "Ka 1.0 1.0 1.0\n"; // Ambient color
        mtlFile << "Kd 1.0 1.0 1.0\n"; // Diffuse color
        mtlFile << "Ks 0.0 0.0 0.0\n"; // Specular color
        mtlFile << "d 1.0\n"; // Transparency
        mtlFile << "illum 1\n"; // Illumination model
        
        // Reference the material in the OBJ file
        objFile << "usemtl TextureMaterial\n";
        objFile << "s 1\n"; // Smooth shading
        
        // Write faces with texture mapping
        int vertexOffset = 1; // OBJ indices are 1-based
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Calculate vertex indices for this grid cell
                int v1 = y * (width + 1) + x + vertexOffset;
                int v2 = y * (width + 1) + (x + 1) + vertexOffset;
                int v3 = (y + 1) * (width + 1) + (x + 1) + vertexOffset;
                int v4 = (y + 1) * (width + 1) + x + vertexOffset;
                
                // Write two triangles to form a quad
                objFile << "f " << v1 << "/" << v1 << " "
                        << v2 << "/" << v2 << " "
                        << v3 << "/" << v3 << "\n";
                objFile << "f " << v1 << "/" << v1 << " "
                        << v3 << "/" << v3 << " "
                        << v4 << "/" << v4 << "\n";
            }
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Exported texture to OBJ/MTL: " << objFilename << " and " << mtlFilename << RESET << std::endl;
        return true;
    }
    
private:
    int width;
    int height;
    std::vector<Pixel> data;
};

class Layer {
public:
    Layer(int width, int height, const std::string& name = "Layer") 
        : name(name), visible(true), opacity(1.0f), texture(width, height) {
        std::cout << FG_GREEN << "✓ " << BOLD << "Created layer: " << RESET << FG_BRIGHT_GREEN << name << RESET << std::endl;
    }
    
    void clear() {
        texture.clear();
        std::cout << FG_YELLOW << "✓ " << "Cleared layer: " << name << RESET << std::endl;
    }
    
    // Export functions for the layer
    bool exportToTxt(const std::string& filename) const {
        return texture.exportToTxt(filename);
    }
    
    bool exportToPpm(const std::string& filename) const {
        return texture.exportToPpm(filename);
    }
    
    bool exportToSvg(const std::string& filename) const {
        return texture.exportToSvg(filename);
    }
    
    bool exportToObjMtl(const std::string& baseFilename) const {
        return texture.exportToObjMtl(baseFilename);
    }
    
    void paint(int x, int y, const Color& color, float brushSize, float opacity = 1.0f, bool addEffect = false) {
        texture.paint(x, y, color, brushSize, opacity * this->opacity, addEffect);
        
        std::string effectStr = addEffect ? " with effects" : "";
        std::cout << FG_BLUE << "✓ " << "Painting at " 
                  << FG_BRIGHT_YELLOW << x << "," << y 
                  << RESET << " using " << FG_BRIGHT_MAGENTA << color.name 
                  << RESET << " (size: " << FG_BRIGHT_CYAN << brushSize << RESET << ")" 
                  << FG_BRIGHT_MAGENTA << effectStr << RESET << std::endl;
    }
    
    void applyEffectAt(int x, int y, const std::string& effect, float size = 1.0f) {
        texture.applyEffectAt(x, y, effect, size);
        std::cout << FG_MAGENTA << "✓ " << "Applied effect at " 
                  << FG_BRIGHT_YELLOW << x << "," << y << RESET << std::endl;
    }
    
    void render(bool isSelected = false) const {
        if (visible) {
            if (isSelected) {
                std::cout << FG_BRIGHT_GREEN << "● " << BOLD << UNDERLINE << name << RESET 
                      << FG_BRIGHT_YELLOW << " (SELECTED)" << RESET << std::endl;
            } else {
                std::cout << FG_GREEN << "● " << BOLD << name << RESET << std::endl;
            }
            texture.render();
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
    
private:
    std::string name;
    bool visible;
    float opacity;
    Texture texture;
};

class Model {
public:
    Model() : loaded(false) {
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
        
        std::cout << FG_BRIGHT_GREEN << "✓" << RESET << std::endl;
        loaded = true;
        return true;
    }
    
    bool isLoaded() const { return loaded; }
    
private:
    bool loaded;
};

class Project {
public:
    Project() : currentLayerIndex(0) {
        std::cout << FG_YELLOW << "✓ " << "Project created" << RESET << std::endl;
    }
    
    bool loadModel(const std::string& path) {
        return model.loadModel(path);
    }
    
    Layer* addLayer(const std::string& name = "") {
        static int layerCounter = 1;
        std::string layerName = name;
        
        if (layerName.empty()) {
            layerName = "Layer " + std::to_string(layerCounter++);
        }
        
        layers.push_back(new Layer(30, 15, layerName));
        currentLayerIndex = layers.size() - 1;
        
        return layers.back();
    }
    
    void renderLayers() const {
        std::cout << BOLD << FG_BRIGHT_CYAN << "+================ CANVAS ================+" << RESET << std::endl;
        
        if (layers.empty()) {
            std::cout << FG_BRIGHT_RED << "  No layers available. Use 'n' to create one!" << RESET << std::endl;
        } else {
            for (size_t i = 0; i < layers.size(); i++) {
                layers[i]->render(i == currentLayerIndex);
                
                if (i < layers.size() - 1) {
                    std::cout << std::endl;
                }
            }
        }
        
        std::cout << BOLD << FG_BRIGHT_CYAN << "+========================================+" << RESET << std::endl;
    }
    
    Layer* getCurrentLayer() {
        if (layers.empty() || currentLayerIndex >= layers.size()) {
            return nullptr;
        }
        
        return layers[currentLayerIndex];
    }
    
    size_t getCurrentLayerIndex() const { return currentLayerIndex; }
    
    void setCurrentLayerIndex(size_t index) { 
        if (index < layers.size()) {
            currentLayerIndex = index;
        }
    }
    
    void toggleLayerVisibility(size_t index) {
        if (index < layers.size()) {
            layers[index]->setVisible(!layers[index]->isVisible());
        }
    }
    
    void renameLayer(size_t index, const std::string& newName) {
        if (index < layers.size()) {
            layers[index]->setName(newName);
        }
    }
    
    size_t getLayerCount() const {
        return layers.size();
    }
    
    bool hasModel() const { return model.isLoaded(); }
    const Model& getModel() const { return model; }
    
    // Export functions for the project
    bool exportCurrentLayerToFormat(const std::string& baseFilename, const std::string& format) {
        if (layers.empty() || currentLayerIndex >= layers.size()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "No active layer to export!" << RESET << std::endl;
            return false;
        }
        
        Layer* layer = layers[currentLayerIndex];
        std::string filename = baseFilename;
        
        // Add layer name to filename
        std::string layerName = layer->getName();
        std::replace(layerName.begin(), layerName.end(), ' ', '_'); // Replace spaces with underscores
        
        // Format filename with layer name
        if (filename.find(".") != std::string::npos) {
            // Insert layer name before extension
            size_t dotPos = filename.find_last_of(".");
            filename.insert(dotPos, "_" + layerName);
        } else {
            filename += "_" + layerName;
        }
        
        // Export to the selected format
        if (format == "txt") {
            return layer->exportToTxt(filename + ".txt");
        } else if (format == "ppm") {
            return layer->exportToPpm(filename + ".ppm");
        } else if (format == "svg") {
            return layer->exportToSvg(filename + ".svg");
        } else if (format == "obj") {
            return layer->exportToObjMtl(filename);
        } else {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "Unknown export format: " << format << RESET << std::endl;
            return false;
        }
    }
    
    bool exportAllLayersToFormat(const std::string& baseFilename, const std::string& format) {
        if (layers.empty()) {
            std::cerr << FG_BRIGHT_RED << "⚠ " << "No layers to export!" << RESET << std::endl;
            return false;
        }
        
        bool allSuccess = true;
        
        // Export each layer with its own filename
        for (size_t i = 0; i < layers.size(); i++) {
            if (!layers[i]->isVisible()) {
                continue; // Skip hidden layers
            }
            
            std::string filename = baseFilename;
            std::string layerName = layers[i]->getName();
            std::replace(layerName.begin(), layerName.end(), ' ', '_');
            
            // Format filename with layer name
            if (filename.find(".") != std::string::npos) {
                size_t dotPos = filename.find_last_of(".");
                filename.insert(dotPos, "_" + layerName);
            } else {
                filename += "_" + layerName;
            }
            
            bool success = false;
            
            // Export to the selected format
            if (format == "txt") {
                success = layers[i]->exportToTxt(filename + ".txt");
            } else if (format == "ppm") {
                success = layers[i]->exportToPpm(filename + ".ppm");
            } else if (format == "svg") {
                success = layers[i]->exportToSvg(filename + ".svg");
            } else if (format == "obj") {
                success = layers[i]->exportToObjMtl(filename);
            } else {
                std::cerr << FG_BRIGHT_RED << "⚠ " << "Unknown export format: " << format << RESET << std::endl;
                return false;
            }
            
            if (!success) {
                allSuccess = false;
            }
        }
        
        return allSuccess;
    }
    
    // Displays a menu of available export formats
    void showExportFormats() const {
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "+========== EXPORT FORMATS ==========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "txt" << RESET << "      Raw texture data format" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "ppm" << RESET << "      Portable Pixmap image format" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "svg" << RESET << "      Scalable Vector Graphics format" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "obj" << RESET << "      Wavefront 3D model format (with MTL)" << std::endl;
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "+==================================+" << RESET << std::endl;
    }
    
    ~Project() {
        for (auto* layer : layers) {
            delete layer;
        }
    }
    
private:
    Model model;
    std::vector<Layer*> layers;
    size_t currentLayerIndex;
};

class PaintTool {
public:
    PaintTool(const std::string& name, const Color& color) 
        : name(name), size(2.0f), color(color), useEffects(false) {
        std::cout << FG_BLUE << "✓ " << "Created paint tool: " << BOLD << name << RESET << std::endl;
    }
    
    virtual void use(Layer* layer, int x, int y) {
        if (layer) {
            layer->paint(x, y, color, size, 1.0f, useEffects);
        }
    }
    
    const std::string& getName() const { return name; }
    float getSize() const { return size; }
    void setSize(float size) { this->size = size; }
    Color getColor() const { return color; }
    void setColor(const Color& color) { this->color = color; }
    bool getUseEffects() const { return useEffects; }
    void setUseEffects(bool use) { useEffects = use; }
    
protected:
    std::string name;
    float size;
    Color color;
    bool useEffects;
};

// Tool implementations
class BrushTool : public PaintTool {
public:
    BrushTool() : PaintTool("Brush", Color("*", FG_BRIGHT_RED, "Red Brush")) {}
};

class EraserTool : public PaintTool {
public:
    EraserTool() : PaintTool("Eraser", Color(" ", RESET, "Eraser")) {}
};

class SprayTool : public PaintTool {
public:
    SprayTool() : PaintTool("Spray", Color(":", FG_BRIGHT_CYAN, "Cyan Spray")) {
        setUseEffects(true);
    }
    
    void use(Layer* layer, int x, int y) override {
        if (layer) {
            // Spray creates multiple random points in an area
            int sprayArea = static_cast<int>(size * 1.5);
            
            for (int i = 0; i < size * 10; i++) {
                int offsetX = rand() % (sprayArea * 2) - sprayArea;
                int offsetY = rand() % (sprayArea * 2) - sprayArea;
                
                if (std::sqrt(offsetX*offsetX + offsetY*offsetY) <= sprayArea) {
                    float randomOpacity = (float)(rand() % 100) / 100.0f;
                    
                    // Use a small brush size for individual dots
                    layer->paint(x + offsetX, y + offsetY, color, 0.5f, randomOpacity, useEffects);
                }
            }
        }
    }
};

class RainbowTool : public PaintTool {
public:
    RainbowTool() : PaintTool("Rainbow", Color("*", FG_BRIGHT_MAGENTA, "Rainbow")) {
        setUseEffects(true);
    }
    
    void use(Layer* layer, int x, int y) override {
        if (layer) {
            // Use a random color for each stroke
            Color randomColor = color;
            randomColor.code = getRandomColor();
            randomColor.symbol = getRandomArtBrush();
            
            layer->paint(x, y, randomColor, size, 1.0f, useEffects);
        }
    }
};

class StampTool : public PaintTool {
public:
    StampTool() : PaintTool("Stamp", Color("●", FG_BRIGHT_GREEN, "Stamp")), 
                  stampIndex(0) {
        // Initialize with some fun stamps
        stamps = {"★", "♥", "♦", "♠", "♣", "⚡", "☀", "☁", "☂", "✿", "❀", "❄", "⚓", "⚔", "⚕"};
    }
    
    void use(Layer* layer, int x, int y) override {
        if (layer) {
            Color stampColor = color;
            stampColor.symbol = stamps[stampIndex];
            layer->paint(x, y, stampColor, size, 1.0f, useEffects);
        }
    }
    
    void nextStamp() {
        stampIndex = (stampIndex + 1) % stamps.size();
        color.symbol = stamps[stampIndex];
    }
    
    std::string getCurrentStamp() const {
        return stamps[stampIndex];
    }
    
private:
    std::vector<std::string> stamps;
    size_t stampIndex;
};

class PatternTool : public PaintTool {
public:
    PatternTool() : PaintTool("Pattern", Color("#", FG_BRIGHT_BLUE, "Pattern")), 
                   patternIndex(0) {
        // Different patterns to draw
        patterns = {
            // Simple line
            [](Layer* layer, int x, int y, const Color& color, float size, bool effects) {
                for (int i = -3; i <= 3; i++) {
                    layer->paint(x + i, y, color, size * 0.5f, 1.0f, effects);
                }
            },
            // Cross
            [](Layer* layer, int x, int y, const Color& color, float size, bool effects) {
                for (int i = -2; i <= 2; i++) {
                    layer->paint(x + i, y, color, size * 0.5f, 1.0f, effects);
                    layer->paint(x, y + i, color, size * 0.5f, 1.0f, effects);
                }
            },
            // Square
            [](Layer* layer, int x, int y, const Color& color, float size, bool effects) {
                for (int i = -2; i <= 2; i++) {
                    layer->paint(x - 2, y + i, color, size * 0.5f, 1.0f, effects);
                    layer->paint(x + 2, y + i, color, size * 0.5f, 1.0f, effects);
                    layer->paint(x + i, y - 2, color, size * 0.5f, 1.0f, effects);
                    layer->paint(x + i, y + 2, color, size * 0.5f, 1.0f, effects);
                }
            },
            // Circle
            [](Layer* layer, int x, int y, const Color& color, float size, bool effects) {
                for (int angle = 0; angle < 360; angle += 30) {
                    float rad = angle * 3.14159f / 180.0f;
                    int px = x + static_cast<int>(2 * std::cos(rad));
                    int py = y + static_cast<int>(2 * std::sin(rad));
                    layer->paint(px, py, color, size * 0.5f, 1.0f, effects);
                }
            }
        };
    }
    
    void use(Layer* layer, int x, int y) override {
        if (layer) {
            patterns[patternIndex](layer, x, y, color, size, useEffects);
        }
    }
    
    void nextPattern() {
        patternIndex = (patternIndex + 1) % patterns.size();
    }
    
    std::string getCurrentPatternName() const {
        const std::vector<std::string> names = {"Line", "Cross", "Square", "Circle"};
        return names[patternIndex];
    }
    
private:
    std::vector<std::function<void(Layer*, int, int, const Color&, float, bool)>> patterns;
    size_t patternIndex;
};

class Application {
public:
    Application() : currentTool(nullptr), running(true) {
        // Show splash screen
        showSplashScreen();
        
        std::cout << FG_BRIGHT_GREEN << BOLD << "=== 3D Model Painter Initialized ===" << RESET << std::endl;
        
        // Initialize color palette
        initColorPalette();
        
        // Initialize tools
        tools.push_back(new BrushTool());
        tools.push_back(new EraserTool());
        tools.push_back(new SprayTool());
        tools.push_back(new RainbowTool());
        tools.push_back(new StampTool());
        tools.push_back(new PatternTool());
        
        // Set default tool
        currentTool = tools[0];
        
        // Load a sample model and add a default layer
        project.loadModel("cube.obj");
        project.addLayer("Background");
    }
    
    void initColorPalette() {
        colorPalette['1'] = Color("*", FG_BRIGHT_RED, "Red");
        colorPalette['2'] = Color("*", FG_BRIGHT_GREEN, "Green");
        colorPalette['3'] = Color("*", FG_BRIGHT_BLUE, "Blue");
        colorPalette['4'] = Color("*", FG_BRIGHT_YELLOW, "Yellow");
        colorPalette['5'] = Color("*", FG_BRIGHT_MAGENTA, "Magenta");
        colorPalette['6'] = Color("*", FG_BRIGHT_CYAN, "Cyan");
        colorPalette['7'] = Color("#", FG_BRIGHT_WHITE, "White");
        colorPalette['8'] = Color(".", FG_GREEN, "Dots");
        colorPalette['9'] = Color("=", FG_YELLOW, "Lines");
        colorPalette['0'] = Color(" ", RESET, "Eraser");
    }
    
    void showHelp() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "                3D MODEL PAINTER HELP                " 
                  << RESET << std::endl << std::endl;
        
        // Tool Selection Section
        std::cout << BOLD << FG_BRIGHT_CYAN << "+========== TOOL SELECTION ==========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-6" << RESET << "      Select tools:" << std::endl;
        for (size_t i = 0; i < tools.size(); i++) {
            std::string selected = (tools[i] == currentTool) ? std::string(" ") + std::string(FG_BRIGHT_GREEN) + "✓" + std::string(RESET) : "";
            std::cout << "  " << FG_BRIGHT_YELLOW << (i+1) << RESET << "        " 
                      << tools[i]->getColor().code << tools[i]->getName() << RESET << selected << std::endl;
        }
        std::cout << BOLD << FG_BRIGHT_CYAN << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Brush Settings
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "+========== BRUSH SETTINGS ==========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "+, -" << RESET << "      Increase/decrease brush size" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "e" << RESET << "        Toggle special effects" << std::endl;
        
        // StampTool and PatternTool specific controls
        if (dynamic_cast<StampTool*>(currentTool)) {
            std::cout << "  " << FG_BRIGHT_YELLOW << "x" << RESET << "        Cycle through stamps" << std::endl;
        }
        if (dynamic_cast<PatternTool*>(currentTool)) {
            std::cout << "  " << FG_BRIGHT_YELLOW << "x" << RESET << "        Cycle through patterns" << std::endl;
        }
        
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-9, 0" << RESET << "   Select color (0 = eraser)" << std::endl;
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Painting Commands
        std::cout << BOLD << FG_BRIGHT_YELLOW << "+========== PAINTING COMMANDS ===========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "p x y" << RESET << "    Paint at position (x,y)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "f x y" << RESET << "    Fill from position (x,y)" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "c" << RESET << "        Clear current layer" << std::endl;
        std::cout << BOLD << FG_BRIGHT_YELLOW << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Layer Management
        std::cout << BOLD << FG_BRIGHT_GREEN << "+========== LAYER MANAGEMENT ===========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "n [name]" << RESET << " New layer with optional name" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "r index [name]" << RESET << " Rename layer" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "l" << RESET << "        List layers" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s index" << RESET << "  Select layer by index" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "t index" << RESET << "  Toggle layer visibility" << std::endl;
        std::cout << BOLD << FG_BRIGHT_GREEN << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Export Controls
        std::cout << BOLD << FG_BRIGHT_BLUE << "+========== EXPORT COMMANDS ==========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "xf" << RESET << "       Show available export formats" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "xs format filename" << RESET << " Export current layer to format" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "xa format filename" << RESET << " Export all visible layers to format" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "Format options: txt, ppm, svg, obj" << RESET << std::endl;
        std::cout << BOLD << FG_BRIGHT_BLUE << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Application Controls
        std::cout << BOLD << FG_BRIGHT_RED << "+========== APPLICATION CONTROLS =========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "v" << RESET << "        View canvas" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "h" << RESET << "        Show this help" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "d" << RESET << "        Run demo animation" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "        Quit" << std::endl;
        std::cout << BOLD << FG_BRIGHT_RED << "+=======================================+" << RESET << std::endl;
        
        std::cout << std::endl;
        std::cout << FG_BRIGHT_CYAN << "Press any key to continue..." << RESET << std::endl;
        std::cin.get(); // Wait for key press
        clearScreen();
    }
    
    void showStatus() {
        std::cout << BOLD << BG_BRIGHT_BLACK << FG_BRIGHT_WHITE 
                  << " STATUS: " << RESET << " ";
        
        // Tool info
        std::cout << BOLD << "Tool: " << FG_BRIGHT_CYAN << currentTool->getName() << RESET;
        
        // Size info
        std::cout << " | Size: " << FG_BRIGHT_YELLOW << currentTool->getSize() << RESET;
        
        // Color info
        std::cout << " | Color: " << currentTool->getColor().code 
                  << currentTool->getColor().name 
                  << " " << currentTool->getColor().symbol << RESET;
        
        // Effects
        std::cout << " | Effects: " << (currentTool->getUseEffects() ? 
                  std::string(FG_BRIGHT_GREEN) + "ON" : std::string(FG_BRIGHT_RED) + "OFF") << RESET;
        
        // Special tool properties
        if (StampTool* stampTool = dynamic_cast<StampTool*>(currentTool)) {
            std::cout << " | Stamp: " << stampTool->getColor().code 
                      << stampTool->getCurrentStamp() << RESET;
        }
        if (PatternTool* patternTool = dynamic_cast<PatternTool*>(currentTool)) {
            std::cout << " | Pattern: " << FG_BRIGHT_MAGENTA 
                      << patternTool->getCurrentPatternName() << RESET;
        }
        
        // Layer info
        if (project.getCurrentLayer()) {
            std::cout << " | Layer: " << FG_BRIGHT_GREEN << project.getCurrentLayer()->getName() << RESET 
                      << " (" << project.getCurrentLayerIndex() << "/" << (project.getLayerCount()-1) << ")";
        } else {
            std::cout << " | " << FG_BRIGHT_RED << "No active layer!" << RESET;
        }
        
        std::cout << std::endl;
    }
    
    void run() {
        std::cout << BOLD << FG_BRIGHT_MAGENTA << "=== 3D Model Painter Started ===" << RESET << std::endl;
        showHelp();
        
        // Draw a sample
        auto* layer = project.getCurrentLayer();
        if (layer && currentTool) {
            currentTool->use(layer, 15, 7);
        }
        
        // Main application loop
        std::string command;
        while (running) {
            std::cout << std::endl;
            showStatus();
            
            std::cout << BOLD << FG_BRIGHT_CYAN << "\nCommand" << RESET 
                      << " (h for help): ";
            
            std::getline(std::cin, command);
            
            if (command.empty()) {
                continue;
            }
            
            char cmd = command[0];
            switch (cmd) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6': {
                    int toolIndex = cmd - '1';
                    if (toolIndex >= 0 && toolIndex < tools.size()) {
                        currentTool = tools[toolIndex];
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Selected tool: " << BOLD << currentTool->getName() << RESET << std::endl;
                    }
                    break;
                }
                case 'C':
                case 'c': {
                    if (command.length() > 1) {
                        char colorKey = command[1];
                        if (colorPalette.find(colorKey) != colorPalette.end()) {
                            currentTool->setColor(colorPalette[colorKey]);
                            std::cout << FG_BRIGHT_GREEN << "✓ " << "Selected color: " << currentTool->getColor().name << RESET << std::endl;
                        }
                    } else {
                        if (project.getCurrentLayer()) {
                            clearScreen();
                            std::cout << FG_BRIGHT_YELLOW << "⚠ " << "Are you sure you want to clear the current layer? (y/n): " << RESET;
                            std::string confirm;
                            std::getline(std::cin, confirm);
                            if (confirm == "y" || confirm == "Y") {
                                project.getCurrentLayer()->clear();
                                std::cout << FG_BRIGHT_GREEN << "✓ " << "Layer cleared" << RESET << std::endl;
                            } else {
                                std::cout << FG_BRIGHT_BLUE << "ℹ " << "Clear operation cancelled" << RESET << std::endl;
                            }
                        }
                    }
                    break;
                }
                case '+': {
                    if (currentTool) {
                        float newSize = currentTool->getSize() + 0.5f;
                        currentTool->setSize(newSize);
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Increased brush size to " << newSize << RESET << std::endl;
                    }
                    break;
                }
                case '-': {
                    if (currentTool) {
                        float newSize = std::max(0.5f, currentTool->getSize() - 0.5f);
                        currentTool->setSize(newSize);
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Decreased brush size to " << newSize << RESET << std::endl;
                    }
                    break;
                }
                case 'E':
                case 'e': {
                    if (currentTool) {
                        currentTool->setUseEffects(!currentTool->getUseEffects());
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Special effects " 
                                  << (currentTool->getUseEffects() ? "enabled" : "disabled") << RESET << std::endl;
                    }
                    break;
                }
                case 'X':
                case 'x': {
                    // Check for export commands (xs, xa, xf)
                    if (command.length() > 1) {
                        char exportCmd = command[1];
                        
                        // Show export formats
                        if (exportCmd == 'f' || exportCmd == 'F') {
                            project.showExportFormats();
                            break;
                        }
                        
                        // Export single layer (xs)
                        if ((exportCmd == 's' || exportCmd == 'S') && command.length() > 3) {
                            std::istringstream iss(command.substr(3));
                            std::string format, filename;
                            
                            if (iss >> format >> filename) {
                                project.exportCurrentLayerToFormat(filename, format);
                            } else {
                                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid export command. Format: xs format filename" << RESET << std::endl;
                                project.showExportFormats();
                            }
                            break;
                        }
                        
                        // Export all layers (xa)
                        if ((exportCmd == 'a' || exportCmd == 'A') && command.length() > 3) {
                            std::istringstream iss(command.substr(3));
                            std::string format, filename;
                            
                            if (iss >> format >> filename) {
                                project.exportAllLayersToFormat(filename, format);
                            } else {
                                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid export command. Format: xa format filename" << RESET << std::endl;
                                project.showExportFormats();
                            }
                            break;
                        }
                    }
                    
                    // Otherwise handle stamps and patterns
                    if (StampTool* stampTool = dynamic_cast<StampTool*>(currentTool)) {
                        stampTool->nextStamp();
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Changed to next stamp: " 
                                  << stampTool->getColor().code << stampTool->getCurrentStamp() << RESET << std::endl;
                    }
                    else if (PatternTool* patternTool = dynamic_cast<PatternTool*>(currentTool)) {
                        patternTool->nextPattern();
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Changed to pattern: " 
                                  << patternTool->getCurrentPatternName() << RESET << std::endl;
                    }
                    break;
                }
                case 'p':
                case 'P': {
                    int x, y;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d %d", &x, &y) == 2) {
                        if (currentTool && project.getCurrentLayer()) {
                            currentTool->use(project.getCurrentLayer(), x, y);
                            std::cout << FG_BRIGHT_CYAN << "✨ " << "Canvas after painting:" << RESET << std::endl;
                            project.getCurrentLayer()->render();
                        }
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid paint command. Format: p x y" << RESET << std::endl;
                    }
                    break;
                }
                case 'f':
                case 'F': {
                    int x, y;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d %d", &x, &y) == 2) {
                        // Simple fill - just fill a rectangle around the point
                        if (currentTool && project.getCurrentLayer()) {
                            std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Filling area..." << RESET << std::endl;
                            
                            int fillSize = 5;
                            for (int i = -fillSize; i <= fillSize; i++) {
                                for (int j = -fillSize; j <= fillSize; j++) {
                                    if (std::sqrt(i*i + j*j) <= fillSize) {
                                        project.getCurrentLayer()->paint(x + i, y + j, currentTool->getColor(), 1.0f);
                                    }
                                }
                            }
                            
                            std::cout << FG_BRIGHT_CYAN << "✨ " << "Canvas after filling:" << RESET << std::endl;
                            project.getCurrentLayer()->render();
                        }
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid fill command. Format: f x y" << RESET << std::endl;
                    }
                    break;
                }
                case 'n':
                case 'N': {
                    std::string name;
                    if (command.length() > 2) {
                        name = command.substr(2);
                    }
                    Layer* newLayer = project.addLayer(name);
                    std::cout << FG_BRIGHT_GREEN << "✓ " << "Created new layer: " << BOLD << newLayer->getName() << RESET << std::endl;
                    break;
                }
                case 'r':
                case 'R': {
                    int index;
                    if (command.length() > 2) {
                        std::istringstream iss(command.substr(2));
                        if (iss >> index) {
                            std::string name;
                            std::getline(iss >> std::ws, name);
                            
                            if (!name.empty()) {
                                project.renameLayer(index, name);
                                std::cout << FG_BRIGHT_GREEN << "✓ " << "Renamed layer " << index 
                                          << " to '" << name << "'" << RESET << std::endl;
                            } else {
                                std::cout << FG_BRIGHT_RED << "⚠ " << "No new name provided. Format: r index new_name" << RESET << std::endl;
                            }
                        } else {
                            std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid rename command. Format: r index new_name" << RESET << std::endl;
                        }
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid rename command. Format: r index new_name" << RESET << std::endl;
                    }
                    break;
                }
                case 'l':
                case 'L': {
                    clearScreen();
                    std::cout << BOLD << FG_BRIGHT_CYAN << "=== Layers ===" << RESET << std::endl;
                    project.renderLayers();
                    break;
                }
                case 's':
                case 'S': {
                    int index;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d", &index) == 1) {
                        project.setCurrentLayerIndex(index);
                        std::cout << FG_BRIGHT_GREEN << "✓ " << "Selected layer " << index << RESET << std::endl;
                    } else {
                        std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid select command. Format: s index" << RESET << std::endl;
                    }
                    break;
                }
                case 't':
                case 'T': {
                    int index;
                    if (command.length() > 2 && sscanf(command.c_str() + 2, "%d", &index) == 1) {
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
                    std::cout << FG_BRIGHT_RED << "⚠ " << "Unknown command: " << cmd << ". Type 'h' for help." << RESET << std::endl;
                    break;
                }
            }
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
};

int main(int argc, char* argv[]) {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
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