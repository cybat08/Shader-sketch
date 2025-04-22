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
#include <algorithm> // For color wheel calculations
#include <unordered_map> // For model data storage

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

// RGB color structure for color wheel and harmony
struct RGB {
    int r, g, b;
    
    RGB() : r(0), g(0), b(0) {}
    RGB(int r, int g, int b) : r(r), g(g), b(b) {}
    
    // Convert RGB to ANSI terminal color (approximate match)
    std::string toAnsiColor() const {
        // Find the closest ANSI color
        if (r > 200 && g < 100 && b < 100) return FG_BRIGHT_RED;
        if (r > 150 && g < 100 && b < 100) return FG_RED;
        if (r < 100 && g > 200 && b < 100) return FG_BRIGHT_GREEN;
        if (r < 100 && g > 150 && b < 100) return FG_GREEN;
        if (r < 100 && g < 100 && b > 200) return FG_BRIGHT_BLUE;
        if (r < 100 && g < 100 && b > 150) return FG_BLUE;
        if (r > 200 && g > 200 && b < 100) return FG_BRIGHT_YELLOW;
        if (r > 150 && g > 150 && b < 100) return FG_YELLOW;
        if (r > 200 && g < 100 && b > 200) return FG_BRIGHT_MAGENTA;
        if (r > 150 && g < 100 && b > 150) return FG_MAGENTA;
        if (r < 100 && g > 200 && b > 200) return FG_BRIGHT_CYAN;
        if (r < 100 && g > 150 && b > 150) return FG_CYAN;
        if (r > 200 && g > 200 && b > 200) return FG_BRIGHT_WHITE;
        if (r > 150 && g > 150 && b > 150) return FG_WHITE;
        if (r < 50 && g < 50 && b < 50) return FG_BLACK;
        return FG_BRIGHT_WHITE; // Default
    }
    
    // Convert to terminal color name
    std::string getColorName() const {
        std::string colorCode = toAnsiColor();
        
        if (colorCode == FG_BRIGHT_RED) return "Bright Red";
        if (colorCode == FG_RED) return "Red";
        if (colorCode == FG_BRIGHT_GREEN) return "Bright Green";
        if (colorCode == FG_GREEN) return "Green";
        if (colorCode == FG_BRIGHT_BLUE) return "Bright Blue";
        if (colorCode == FG_BLUE) return "Blue";
        if (colorCode == FG_BRIGHT_YELLOW) return "Bright Yellow";
        if (colorCode == FG_YELLOW) return "Yellow";
        if (colorCode == FG_BRIGHT_MAGENTA) return "Bright Magenta";
        if (colorCode == FG_MAGENTA) return "Magenta";
        if (colorCode == FG_BRIGHT_CYAN) return "Bright Cyan";
        if (colorCode == FG_CYAN) return "Cyan";
        if (colorCode == FG_BRIGHT_WHITE) return "White";
        if (colorCode == FG_WHITE) return "Light Gray";
        if (colorCode == FG_BLACK) return "Black";
        
        return "Custom";
    }
};

// HSV color structure for color wheel calculations
struct HSV {
    float h; // Hue [0-360]
    float s; // Saturation [0-1]
    float v; // Value [0-1]
    
    HSV() : h(0), s(0), v(0) {}
    HSV(float h, float s, float v) : h(h), s(s), v(v) {}
    
    // Convert HSV to RGB
    RGB toRGB() const {
        float c = v * s;
        float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        
        float r = 0, g = 0, b = 0;
        
        if (h >= 0 && h < 60) {
            r = c; g = x; b = 0;
        } else if (h >= 60 && h < 120) {
            r = x; g = c; b = 0;
        } else if (h >= 120 && h < 180) {
            r = 0; g = c; b = x;
        } else if (h >= 180 && h < 240) {
            r = 0; g = x; b = c;
        } else if (h >= 240 && h < 300) {
            r = x; g = 0; b = c;
        } else {
            r = c; g = 0; b = x;
        }
        
        return RGB(
            static_cast<int>((r + m) * 255),
            static_cast<int>((g + m) * 255),
            static_cast<int>((b + m) * 255)
        );
    }
    
    // Generate complementary color (opposite on color wheel)
    HSV complementary() const {
        return HSV(std::fmod(h + 180, 360), s, v);
    }
    
    // Generate analogous colors (adjacent on color wheel)
    std::vector<HSV> analogous(int count = 2, float angle = 30) const {
        std::vector<HSV> colors;
        for (int i = 1; i <= count; i++) {
            colors.push_back(HSV(std::fmod(h + angle * i, 360), s, v));
            colors.push_back(HSV(std::fmod(h - angle * i + 360, 360), s, v));
        }
        return colors;
    }
    
    // Generate triadic colors (three equally spaced colors)
    std::vector<HSV> triadic() const {
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(h + 120, 360), s, v));
        colors.push_back(HSV(std::fmod(h + 240, 360), s, v));
        return colors;
    }
    
    // Generate tetradic colors (rectangle on color wheel)
    std::vector<HSV> tetradic() const {
        std::vector<HSV> colors;
        colors.push_back(HSV(std::fmod(h + 60, 360), s, v));
        colors.push_back(HSV(std::fmod(h + 180, 360), s, v));
        colors.push_back(HSV(std::fmod(h + 240, 360), s, v));
        return colors;
    }
    
    // Generate split complementary colors
    std::vector<HSV> splitComplementary() const {
        std::vector<HSV> colors;
        float compHue = std::fmod(h + 180, 360);
        colors.push_back(HSV(std::fmod(compHue - 30, 360), s, v));
        colors.push_back(HSV(std::fmod(compHue + 30, 360), s, v));
        return colors;
    }
};

// Convert RGB to HSV
HSV rgbToHsv(const RGB& rgb) {
    float r = rgb.r / 255.0f;
    float g = rgb.g / 255.0f;
    float b = rgb.b / 255.0f;
    
    float cmax = std::max(std::max(r, g), b);
    float cmin = std::min(std::min(r, g), b);
    float delta = cmax - cmin;
    
    float h = 0;
    if (delta == 0) {
        h = 0;
    } else if (cmax == r) {
        h = 60 * std::fmod(((g - b) / delta), 6);
    } else if (cmax == g) {
        h = 60 * (((b - r) / delta) + 2);
    } else {
        h = 60 * (((r - g) / delta) + 4);
    }
    
    if (h < 0) h += 360;
    
    float s = (cmax == 0) ? 0 : (delta / cmax);
    float v = cmax;
    
    return HSV(h, s, v);
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
        
        // In a real implementation, we would parse the OBJ/FBX file here
        // Example code for real OBJ parsing would be:
        /*
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open model file: " << path << std::endl;
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "v") {
                // Parse vertex
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(Vertex(x, y, z));
            } 
            else if (prefix == "vt") {
                // Parse UV coordinate
                float u, v;
                iss >> u >> v;
                uvs.push_back(UV(u, v));
            }
            else if (prefix == "f") {
                // Parse face
                Face face;
                std::string vertexData;
                while (iss >> vertexData) {
                    std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                    std::istringstream viss(vertexData);
                    int v, t, n;
                    viss >> v;
                    if (viss >> t) {
                        if (!(viss >> n)) n = 0;
                        face.addVertex(v-1, t-1, n-1); // OBJ is 1-indexed
                    } else {
                        face.addVertex(v-1, 0, 0);
                    }
                }
                faces.push_back(face);
            }
        }
        */
        
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
                drawLine(texture, x1, y1, x2, y2, faceColor);
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
        
        // Calculate face normal (simplified)
        Vertex edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        Vertex edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        Vertex normal(
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        );
        
        // Normalize the normal
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
        
        // Determine the primary projection plane based on the normal
        char primaryAxis = 'y'; // Default to y-axis
        float absX = std::abs(normal.x);
        float absY = std::abs(normal.y);
        float absZ = std::abs(normal.z);
        
        if (absX >= absY && absX >= absZ) {
            primaryAxis = 'x';
        } else if (absY >= absX && absY >= absZ) {
            primaryAxis = 'y';
        } else {
            primaryAxis = 'z';
        }
        
        // Create new UVs based on the projection
        for (size_t i = 0; i < face.vertexIndices.size(); i++) {
            int vertIdx = face.vertexIndices[i];
            const Vertex& vert = vertices[vertIdx];
            
            float u = 0.0f, v = 0.0f;
            
            // Project based on primary axis
            switch (primaryAxis) {
                case 'x':
                    u = vert.z;
                    v = vert.y;
                    break;
                case 'y':
                    u = vert.x;
                    v = vert.z;
                    break;
                case 'z':
                    u = vert.x;
                    v = vert.y;
                    break;
            }
            
            // Apply scaling and offset
            u = u * scaleU + offsetU;
            v = v * scaleV + offsetV;
            
            // Create a new UV or update existing
            if (i < face.uvIndices.size() && face.uvIndices[i] < uvs.size()) {
                // Update existing UV
                uvs[face.uvIndices[i]] = UV(u, v);
            } else {
                // Create new UV
                uvs.push_back(UV(u, v));
                if (i < face.uvIndices.size()) {
                    face.uvIndices[i] = uvs.size() - 1;
                } else {
                    face.uvIndices.push_back(uvs.size() - 1);
                }
            }
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Applied planar unwrap to face " << faceIndex << RESET << std::endl;
    }
    
    // Draw a line between two points using Bresenham's algorithm
    void drawLine(Texture& texture, int x1, int y1, int x2, int y2, const Color& color) const {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            texture.paint(x1, y1, color, 0.5f, 0.7f);
            
            if (x1 == x2 && y1 == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
    
    // Run a simple unwrapping algorithm for all faces
    void unwrapUVs() {
        float uScale = 0.1f;
        float vScale = 0.1f;
        float uOffset = 0.25f;
        float vOffset = 0.25f;
        
        for (size_t i = 0; i < faces.size(); i++) {
            // Apply a slightly different offset for each face
            float faceUOffset = uOffset + (i % 3) * 0.2f;
            float faceVOffset = vOffset + (i / 3) * 0.2f;
            
            applyPlanarUnwrap(i, uScale, vScale, faceUOffset, faceVOffset);
        }
        
        std::cout << FG_BRIGHT_GREEN << "✓ " << "Applied UV unwrapping to all faces" << RESET << std::endl;
    }
    
    // Getters for model data
    bool isLoaded() const { return loaded; }
    const std::string& getName() const { return modelName; }
    int getVertexCount() const { return vertices.size(); }
    int getFaceCount() const { return faces.size(); }
    int getUVCount() const { return uvs.size(); }
    
private:
    bool loaded;
    std::string modelName;
    std::vector<Vertex> vertices;
    std::vector<UV> uvs;
    std::vector<Face> faces;
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
    
    // Interactive color wheel with harmony suggestions
    void showColorWheel() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "           INTERACTIVE COLOR WHEEL           " 
                  << RESET << std::endl << std::endl;
        
        // Convert current color to HSV for the wheel
        HSV currentColor(0, 1.0f, 1.0f); // Default: Red
        
        // If we have a current tool with color, use that
        if (currentTool) {
            // Extract ANSI code and try to determine HSV values
            std::string colorCode = currentTool->getColor().code;
            
            // Map common ANSI colors to HSV values
            if (colorCode == FG_BRIGHT_RED || colorCode == FG_RED)
                currentColor = HSV(0, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_GREEN || colorCode == FG_GREEN)
                currentColor = HSV(120, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_BLUE || colorCode == FG_BLUE)
                currentColor = HSV(240, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_YELLOW || colorCode == FG_YELLOW)
                currentColor = HSV(60, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_MAGENTA || colorCode == FG_MAGENTA)
                currentColor = HSV(300, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_CYAN || colorCode == FG_CYAN)
                currentColor = HSV(180, 1.0f, 1.0f);
            else if (colorCode == FG_BRIGHT_WHITE || colorCode == FG_WHITE)
                currentColor = HSV(0, 0.0f, 1.0f);
        }
        
        bool colorUpdated = false;
        
        // Interactive loop for color wheel navigation
        do {
            colorUpdated = false;
            
            // Current color info
            RGB rgbColor = currentColor.toRGB();
            std::string ansiColor = rgbColor.toAnsiColor();
            std::string colorName = rgbColor.getColorName();
            
            std::cout << BOLD << "Current Color: " << ansiColor << "███" << RESET << " (HSV: " 
                      << std::fixed << std::setprecision(0) << currentColor.h << "°, " 
                      << std::setprecision(2) << currentColor.s * 100 << "%, " 
                      << currentColor.v * 100 << "%)" << std::endl;
            std::cout << "Name: " << colorName << std::endl << std::endl;
            
            // ASCII color wheel representation
            const int radius = 10;
            
            // Draw the color wheel
            for (int y = -radius; y <= radius; y++) {
                std::cout << "  ";
                for (int x = -radius; x <= radius; x++) {
                    // Calculate distance from center
                    float distance = std::sqrt(x*x + y*y);
                    
                    if (distance <= radius) {
                        // Calculate angle (hue) and convert to degrees
                        float angle = std::atan2(y, x) * 180.0f / M_PI;
                        if (angle < 0) angle += 360.0f;
                        
                        // Calculate saturation based on distance from center
                        float saturation = distance / radius;
                        
                        // Create HSV color and convert to RGB
                        HSV pointColor(angle, saturation, 1.0f);
                        RGB rgb = pointColor.toRGB();
                        std::string color = rgb.toAnsiColor();
                        
                        // Mark the current selected color
                        float angleDiff = std::abs(angle - currentColor.h);
                        if (angleDiff > 180) angleDiff = 360 - angleDiff;
                        
                        float satDiff = std::abs(saturation - currentColor.s);
                        
                        if (angleDiff < 15 && satDiff < 0.2) {
                            std::cout << color << "◉" << RESET;
                        } else {
                            std::cout << color << "●" << RESET;
                        }
                    } else {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }
            
            std::cout << std::endl;
            
            // Color harmony suggestions
            std::cout << BOLD << FG_GREEN << "Color Harmony Suggestions:" << RESET << std::endl;
            
            // Complementary
            HSV complementary = currentColor.complementary();
            RGB compRgb = complementary.toRGB();
            std::cout << "  " << FG_BRIGHT_YELLOW << "1" << RESET << " - Complementary: " 
                      << compRgb.toAnsiColor() << "███" << RESET << " (" 
                      << std::fixed << std::setprecision(0) << complementary.h << "°)" << std::endl;
            
            // Analogous
            std::vector<HSV> analogous = currentColor.analogous();
            std::cout << "  " << FG_BRIGHT_YELLOW << "2" << RESET << " - Analogous: ";
            for (const auto& color : analogous) {
                RGB rgb = color.toRGB();
                std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
            }
            std::cout << std::endl;
            
            // Triadic
            std::vector<HSV> triadic = currentColor.triadic();
            std::cout << "  " << FG_BRIGHT_YELLOW << "3" << RESET << " - Triadic: ";
            for (const auto& color : triadic) {
                RGB rgb = color.toRGB();
                std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
            }
            std::cout << std::endl;
            
            // Split Complementary
            std::vector<HSV> splitComp = currentColor.splitComplementary();
            std::cout << "  " << FG_BRIGHT_YELLOW << "4" << RESET << " - Split Complementary: ";
            for (const auto& color : splitComp) {
                RGB rgb = color.toRGB();
                std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
            }
            std::cout << std::endl;
            
            // Tetradic (Rectangle)
            std::vector<HSV> tetradic = currentColor.tetradic();
            std::cout << "  " << FG_BRIGHT_YELLOW << "5" << RESET << " - Tetradic: ";
            for (const auto& color : tetradic) {
                RGB rgb = color.toRGB();
                std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
            }
            std::cout << std::endl;
            
            // Controls
            std::cout << std::endl;
            std::cout << BOLD << FG_GREEN << "Controls:" << RESET << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "1-5" << RESET << "      Select a harmony color" << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "w/a/s/d" << RESET << "  Navigate the color wheel" << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "+/-" << RESET << "      Adjust saturation" << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "<, >" << RESET << "     Adjust brightness" << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "enter" << RESET << "    Select current color" << std::endl;
            std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "        Return without changes" << std::endl;
            
            // Get user input for color wheel navigation
            std::cout << std::endl;
            std::cout << "Enter command: ";
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty() || input == "enter") {
                // Apply the selected color to the current tool
                RGB selectedRgb = currentColor.toRGB();
                std::string ansiCode = selectedRgb.toAnsiColor();
                std::string name = selectedRgb.getColorName();
                
                if (currentTool) {
                    // Create a new color with the selected ANSI code but keep the current symbol
                    std::string symbol = currentTool->getColor().symbol;
                    Color newColor(symbol, ansiCode, name);
                    currentTool->setColor(newColor);
                    
                    std::cout << FG_BRIGHT_GREEN << "✓ " << "Applied new color: " 
                              << ansiCode << "███" << RESET << " to " 
                              << currentTool->getName() << std::endl;
                }
                
                return; // Exit the color wheel
            }
            
            if (input == "q") {
                return; // Cancel and return
            }
            
            // Handle numeric inputs for harmony selections
            if (input == "1") {
                // Complementary
                currentColor = complementary;
                colorUpdated = true;
            } else if (input == "2" && !analogous.empty()) {
                // First analogous color
                currentColor = analogous[0];
                colorUpdated = true;
            } else if (input == "3" && !triadic.empty()) {
                // First triadic color
                currentColor = triadic[0];
                colorUpdated = true;
            } else if (input == "4" && !splitComp.empty()) {
                // First split complementary color
                currentColor = splitComp[0];
                colorUpdated = true;
            } else if (input == "5" && !tetradic.empty()) {
                // First tetradic color
                currentColor = tetradic[0];
                colorUpdated = true;
            } else if (input == "w") {
                // Move up (decrease hue)
                currentColor.h = std::fmod(currentColor.h - 15 + 360, 360);
                colorUpdated = true;
            } else if (input == "s") {
                // Move down (increase hue)
                currentColor.h = std::fmod(currentColor.h + 15, 360);
                colorUpdated = true;
            } else if (input == "a") {
                // Move left (decrease saturation)
                currentColor.s = std::max(0.0f, currentColor.s - 0.1f);
                colorUpdated = true;
            } else if (input == "d") {
                // Move right (increase saturation)
                currentColor.s = std::min(1.0f, currentColor.s + 0.1f);
                colorUpdated = true;
            } else if (input == "+") {
                // Increase saturation
                currentColor.s = std::min(1.0f, currentColor.s + 0.1f);
                colorUpdated = true;
            } else if (input == "-") {
                // Decrease saturation
                currentColor.s = std::max(0.0f, currentColor.s - 0.1f);
                colorUpdated = true;
            } else if (input == "<") {
                // Decrease value
                currentColor.v = std::max(0.0f, currentColor.v - 0.1f);
                colorUpdated = true;
            } else if (input == ">") {
                // Increase value
                currentColor.v = std::min(1.0f, currentColor.v + 0.1f);
                colorUpdated = true;
            }
            
            // Clear screen for next iteration
            if (colorUpdated) {
                clearScreen();
            }
            
        } while (colorUpdated);
    }
    
    // UV Unwrapping and editing tools UI
    void showUVEditor() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "            UV UNWRAPPING TOOLS             " 
                  << RESET << std::endl << std::endl;
        
        if (!project.getModel().isLoaded()) {
            std::cout << FG_BRIGHT_RED << "⚠ " << "No model loaded. Please load a 3D model first." << RESET << std::endl;
            std::cout << std::endl << "Press any key to continue..." << std::endl;
            std::cin.get();
            return;
        }
        
        // Display model info
        const Model& model = project.getModel();
        std::cout << BOLD << "Model: " << FG_BRIGHT_CYAN << model.getName() << RESET << std::endl;
        std::cout << "Vertices: " << FG_BRIGHT_YELLOW << model.getVertexCount() << RESET 
                  << " | Faces: " << FG_BRIGHT_YELLOW << model.getFaceCount() << RESET
                  << " | UVs: " << FG_BRIGHT_YELLOW << model.getUVCount() << RESET << std::endl << std::endl;
        
        // Create a new layer for displaying UV map if needed
        Layer* uvLayer = nullptr;
        for (int i = 0; i < project.getLayerCount(); i++) {
            Layer* layer = project.getLayer(i);
            if (layer && layer->getName() == "UV Map") {
                uvLayer = layer;
                break;
            }
        }
        
        if (!uvLayer) {
            uvLayer = project.addLayer("UV Map");
            std::cout << FG_BRIGHT_GREEN << "✓ " << "Created new layer for UV Map" << RESET << std::endl;
        }
        
        // Generate UV map visualization
        std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Generating UV map visualization..." << RESET << std::endl;
        model.generateUVMap(*uvLayer);
        
        // Display the UV map
        std::cout << BOLD << FG_BRIGHT_CYAN << "+================ UV MAP ================+" << RESET << std::endl;
        uvLayer->render();
        std::cout << BOLD << FG_BRIGHT_CYAN << "+=======================================+" << RESET << std::endl << std::endl;
        
        // Show unwrapping options
        std::cout << BOLD << FG_GREEN << "Unwrapping Options:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1" << RESET << " - Apply planar unwrap to selected face" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "2" << RESET << " - Apply automatic unwrap to all faces" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "3" << RESET << " - Reset unwrap to default" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << " - Return to main menu" << std::endl;
        
        bool stayInUVEditor = true;
        while (stayInUVEditor) {
            std::cout << std::endl << "Enter command: ";
            std::string input;
            std::getline(std::cin, input);
            
            if (input == "q") {
                stayInUVEditor = false;
            } else if (input == "1") {
                // Apply planar unwrap to a face
                std::cout << "Enter face index (0-" << model.getFaceCount() - 1 << "): ";
                std::string faceIndexStr;
                std::getline(std::cin, faceIndexStr);
                
                try {
                    int faceIndex = std::stoi(faceIndexStr);
                    
                    // Apply unwrap
                    const_cast<Model&>(model).applyPlanarUnwrap(faceIndex, 0.1f, 0.1f, 0.25f, 0.25f);
                    
                    // Regenerate UV map
                    std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Regenerating UV map..." << RESET << std::endl;
                    model.generateUVMap(*uvLayer);
                    
                    // Show updated UV map
                    std::cout << BOLD << FG_BRIGHT_CYAN << "+================ UPDATED UV MAP ================+" << RESET << std::endl;
                    uvLayer->render();
                    std::cout << BOLD << FG_BRIGHT_CYAN << "+=======================================+" << RESET << std::endl;
                } catch (const std::exception& e) {
                    std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid face index. Please try again." << RESET << std::endl;
                }
            } else if (input == "2") {
                // Apply automatic unwrap to all faces
                std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Applying automatic UV unwrapping..." << RESET << std::endl;
                
                // Apply unwrap
                const_cast<Model&>(model).unwrapUVs();
                
                // Regenerate UV map
                model.generateUVMap(*uvLayer);
                
                // Show updated UV map
                std::cout << BOLD << FG_BRIGHT_CYAN << "+================ UPDATED UV MAP ================+" << RESET << std::endl;
                uvLayer->render();
                std::cout << BOLD << FG_BRIGHT_CYAN << "+=======================================+" << RESET << std::endl;
            } else if (input == "3") {
                // Reset unwrap to default
                std::cout << FG_BRIGHT_MAGENTA << "⧗ " << "Resetting UV unwrapping..." << RESET << std::endl;
                
                // Reset by recreating the demo model
                const_cast<Model&>(model).createDemoModel();
                
                // Regenerate UV map
                model.generateUVMap(*uvLayer);
                
                // Show updated UV map
                std::cout << BOLD << FG_BRIGHT_CYAN << "+================ UPDATED UV MAP ================+" << RESET << std::endl;
                uvLayer->render();
                std::cout << BOLD << FG_BRIGHT_CYAN << "+=======================================+" << RESET << std::endl;
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << "Unknown command. Please try again." << RESET << std::endl;
            }
        }
    }
    
    // Display the interactive color wheel
    void showColorWheel() {
        clearScreen();
        bool colorUpdated = false;
        HSV currentColor = currentHsvColor;
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "               COLOR WHEEL WITH HARMONY               " 
                  << RESET << std::endl << std::endl;
        
        // Current color info
        RGB rgbColor = currentColor.toRGB();
        std::string ansiColor = rgbColor.toAnsiColor();
        std::string colorName = rgbColor.getColorName();
        
        std::cout << BOLD << "Current Color: " << ansiColor << "███" << RESET << " (HSV: " 
                  << std::fixed << std::setprecision(0) << currentColor.h << "°, " 
                  << std::setprecision(2) << currentColor.s * 100 << "%, " 
                  << currentColor.v * 100 << "%)" << std::endl;
        std::cout << "Name: " << colorName << std::endl << std::endl;
        
        // ASCII color wheel representation
        const int radius = 10;
        const int wheelSize = radius * 2 + 1;
        
        // Draw the color wheel
        for (int y = -radius; y <= radius; y++) {
            std::cout << "  ";
            for (int x = -radius; x <= radius; x++) {
                // Calculate distance from center
                float distance = std::sqrt(x*x + y*y);
                
                if (distance <= radius) {
                    // Calculate angle (hue) and convert to degrees
                    float angle = std::atan2(y, x) * 180.0f / M_PI;
                    if (angle < 0) angle += 360.0f;
                    
                    // Calculate saturation based on distance from center
                    float saturation = distance / radius;
                    
                    // Create HSV color and convert to RGB
                    HSV pointColor(angle, saturation, 1.0f);
                    RGB rgb = pointColor.toRGB();
                    std::string color = rgb.toAnsiColor();
                    
                    // Mark the current selected color
                    float angleDiff = std::abs(angle - currentColor.h);
                    if (angleDiff > 180) angleDiff = 360 - angleDiff;
                    
                    float satDiff = std::abs(saturation - currentColor.s);
                    
                    if (angleDiff < 15 && satDiff < 0.2) {
                        std::cout << color << "◉" << RESET;
                    } else {
                        std::cout << color << "●" << RESET;
                    }
                } else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
        
        // Color harmony suggestions
        std::cout << BOLD << FG_GREEN << "Color Harmony Suggestions:" << RESET << std::endl;
        
        // Complementary
        HSV complementary = currentColor.complementary();
        RGB compRgb = complementary.toRGB();
        std::cout << "  " << FG_BRIGHT_YELLOW << "1" << RESET << " - Complementary: " 
                  << compRgb.toAnsiColor() << "███" << RESET << " (" 
                  << std::fixed << std::setprecision(0) << complementary.h << "°)" << std::endl;
        
        // Analogous
        std::vector<HSV> analogous = currentColor.analogous();
        std::cout << "  " << FG_BRIGHT_YELLOW << "2" << RESET << " - Analogous: ";
        for (const auto& color : analogous) {
            RGB rgb = color.toRGB();
            std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
        }
        std::cout << std::endl;
        
        // Triadic
        std::vector<HSV> triadic = currentColor.triadic();
        std::cout << "  " << FG_BRIGHT_YELLOW << "3" << RESET << " - Triadic: ";
        for (const auto& color : triadic) {
            RGB rgb = color.toRGB();
            std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
        }
        std::cout << std::endl;
        
        // Split Complementary
        std::vector<HSV> splitComp = currentColor.splitComplementary();
        std::cout << "  " << FG_BRIGHT_YELLOW << "4" << RESET << " - Split Complementary: ";
        for (const auto& color : splitComp) {
            RGB rgb = color.toRGB();
            std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
        }
        std::cout << std::endl;
        
        // Tetradic (Rectangle)
        std::vector<HSV> tetradic = currentColor.tetradic();
        std::cout << "  " << FG_BRIGHT_YELLOW << "5" << RESET << " - Tetradic: ";
        for (const auto& color : tetradic) {
            RGB rgb = color.toRGB();
            std::cout << rgb.toAnsiColor() << "███" << RESET << " ";
        }
        std::cout << std::endl;
        
        // Controls
        std::cout << std::endl;
        std::cout << BOLD << FG_GREEN << "Controls:" << RESET << std::endl;
        std::cout << "  Type " << FG_BRIGHT_YELLOW << "up/down/left/right" << RESET << " to move around the color wheel" << std::endl;
        std::cout << "  Type " << FG_BRIGHT_YELLOW << "1-5" << RESET << " to select a harmony color" << std::endl;
        std::cout << "  Type " << FG_BRIGHT_YELLOW << "+/-" << RESET << " to adjust saturation" << std::endl;
        std::cout << "  Type " << FG_BRIGHT_YELLOW << "</>" << RESET << " to adjust value (brightness)" << std::endl;
        std::cout << "  Press " << FG_BRIGHT_YELLOW << "Enter" << RESET << " to select current color and return" << std::endl;
        std::cout << "  Type " << FG_BRIGHT_YELLOW << "q" << RESET << " to cancel and return" << std::endl;
        
        // Get user input for color wheel navigation
        std::cout << std::endl;
        std::cout << "Use arrow keys to navigate, or enter command: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            // Return with current color selected
            currentHsvColor = currentColor;
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, colorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << colorName << std::endl;
            }
            return;
        } else if (input == "q" || input == "Q") {
            // Cancel and return
            return;
        }
        
        // Handle user input
        if (input == "1") {
            // Complementary
            currentHsvColor = complementary;
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentHsvColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                std::string newColorName = toolRgb.getColorName();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, newColorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << newColorName << std::endl;
            }
        } else if (input == "2" && !analogous.empty()) {
            // First analogous color
            currentHsvColor = analogous[0];
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentHsvColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                std::string newColorName = toolRgb.getColorName();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, newColorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << newColorName << std::endl;
            }
        } else if (input == "3" && !triadic.empty()) {
            // First triadic color
            currentHsvColor = triadic[0];
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentHsvColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                std::string newColorName = toolRgb.getColorName();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, newColorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << newColorName << std::endl;
            }
        } else if (input == "4" && !splitComp.empty()) {
            // First split complementary color
            currentHsvColor = splitComp[0];
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentHsvColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                std::string newColorName = toolRgb.getColorName();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, newColorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << newColorName << std::endl;
            }
        } else if (input == "5" && !tetradic.empty()) {
            // First tetradic color
            currentHsvColor = tetradic[0];
            
            // Update the current tool color
            if (currentTool) {
                RGB toolRgb = currentHsvColor.toRGB();
                std::string ansiCode = toolRgb.toAnsiColor();
                std::string newColorName = toolRgb.getColorName();
                currentTool->setColor(Color(currentTool->getColor().symbol, ansiCode, newColorName));
                std::cout << FG_BRIGHT_GREEN << "✓ " << "Color updated to " << ansiCode 
                          << "███" << RESET << " " << newColorName << std::endl;
            }
        } else if (input == "up" || input == "w") {
            // Move up (decrease hue)
            currentColor.h = std::fmod(currentColor.h - 15 + 360, 360);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "down" || input == "s") {
            // Move down (increase hue)
            currentColor.h = std::fmod(currentColor.h + 15, 360);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "left" || input == "a") {
            // Move left (decrease saturation)
            currentColor.s = std::max(0.0f, currentColor.s - 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "right" || input == "d") {
            // Move right (increase saturation)
            currentColor.s = std::min(1.0f, currentColor.s + 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "+") {
            // Increase saturation
            currentColor.s = std::min(1.0f, currentColor.s + 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "-") {
            // Decrease saturation
            currentColor.s = std::max(0.0f, currentColor.s - 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == "<") {
            // Decrease value
            currentColor.v = std::max(0.0f, currentColor.v - 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else if (input == ">") {
            // Increase value
            currentColor.v = std::min(1.0f, currentColor.v + 0.1f);
            showColorWheel(); // Recursively call to show updated wheel
        } else {
            // Unknown command - show the wheel again
            showColorWheel();
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
        std::cout << "  " << FG_BRIGHT_YELLOW << "w" << RESET << "        Open interactive color wheel with harmony suggestions" << std::endl;
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
        
        // Model Operations
        std::cout << BOLD << FG_BRIGHT_BLUE << "+========== MODEL OPERATIONS ==========+" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "m" << RESET << "        Load 3D model from file" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "u" << RESET << "        Open UV unwrapping and editing tools" << std::endl;
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
                case 'w':
                case 'W': {
                    showColorWheel();
                    break;
                }
                case 'u':
                case 'U': {
                    showUVEditor();
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