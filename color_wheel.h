#ifndef COLOR_WHEEL_H
#define COLOR_WHEEL_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <vector>
#include <string>
#include <cmath>
#include <array>
#include <utility>
#include <iostream>

// Use a namespace to avoid conflicts with the application's Color struct
namespace color_wheel {

// Color structure with RGB and HSV components
struct Color {
    // RGB components (0-255)
    int r, g, b;
    
    // HSV components (Hue: 0-360, Saturation/Value: 0.0-1.0)
    float h, s, v;
    
    // Default constructor
    Color() : r(255), g(0), b(0), h(0.0f), s(1.0f), v(1.0f) {}
    
    // RGB constructor
    Color(int r, int g, int b) : r(r), g(g), b(b) {
        RGBtoHSV();
    }
    
    // HSV constructor
    Color(float h, float s, float v) : h(h), s(s), v(v) {
        HSVtoRGB();
    }
    
    // Convert current RGB values to HSV
    void RGBtoHSV() {
        float r_norm = r / 255.0f;
        float g_norm = g / 255.0f;
        float b_norm = b / 255.0f;
        
        float c_max = std::max(std::max(r_norm, g_norm), b_norm);
        float c_min = std::min(std::min(r_norm, g_norm), b_norm);
        float delta = c_max - c_min;
        
        // Calculate hue
        if (delta == 0) {
            h = 0;
        } else if (c_max == r_norm) {
            h = 60.0f * fmodf(((g_norm - b_norm) / delta), 6.0f);
        } else if (c_max == g_norm) {
            h = 60.0f * (((b_norm - r_norm) / delta) + 2.0f);
        } else {
            h = 60.0f * (((r_norm - g_norm) / delta) + 4.0f);
        }
        
        if (h < 0) h += 360.0f;
        
        // Calculate saturation
        s = (c_max == 0) ? 0 : (delta / c_max);
        
        // Value is simply the max
        v = c_max;
    }
    
    // Convert current HSV values to RGB
    void HSVtoRGB() {
        float c = v * s;
        float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        
        float r_norm, g_norm, b_norm;
        
        if (h >= 0 && h < 60) {
            r_norm = c;
            g_norm = x;
            b_norm = 0;
        } else if (h >= 60 && h < 120) {
            r_norm = x;
            g_norm = c;
            b_norm = 0;
        } else if (h >= 120 && h < 180) {
            r_norm = 0;
            g_norm = c;
            b_norm = x;
        } else if (h >= 180 && h < 240) {
            r_norm = 0;
            g_norm = x;
            b_norm = c;
        } else if (h >= 240 && h < 300) {
            r_norm = x;
            g_norm = 0;
            b_norm = c;
        } else {
            r_norm = c;
            g_norm = 0;
            b_norm = x;
        }
        
        r = static_cast<int>((r_norm + m) * 255 + 0.5f);
        g = static_cast<int>((g_norm + m) * 255 + 0.5f);
        b = static_cast<int>((b_norm + m) * 255 + 0.5f);
    }
    
    // Update HSV values and convert to RGB
    void setHSV(float h_new, float s_new, float v_new) {
        h = h_new;
        s = s_new;
        v = v_new;
        HSVtoRGB();
    }
    
    // Update RGB values and convert to HSV
    void setRGB(int r_new, int g_new, int b_new) {
        r = r_new;
        g = g_new;
        b = b_new;
        RGBtoHSV();
    }
    
    // Get ANSI color code for terminal output
    std::string getANSIColor() const {
        return "\033[38;2;" + std::to_string(r) + ";" + 
               std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    
    // Get ANSI background color code
    std::string getANSIBgColor() const {
        return "\033[48;2;" + std::to_string(r) + ";" + 
               std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    
    // Get colored character representation (shows actual color)
    std::string getColoredChar(char ch = ' ') const {
        return getANSIBgColor() + ch + "\033[0m";
    }
    
    // Get hex color code (#RRGGBB)
    std::string getHexCode() const {
        char hex[8];
        snprintf(hex, sizeof(hex), "#%02X%02X%02X", r, g, b);
        return std::string(hex);
    }
    
    // String representation of color
    std::string toString() const {
        return "RGB(" + std::to_string(r) + "," + std::to_string(g) + "," + 
               std::to_string(b) + ") HSV(" + std::to_string(h) + "," + 
               std::to_string(s) + "," + std::to_string(v) + ")";
    }
};

// Color Harmony class - generates harmonious color schemes
class ColorHarmony {
public:
    // Harmony types
    enum class HarmonyType {
        COMPLEMENTARY,
        ANALOGOUS,
        TRIADIC,
        SPLIT_COMPLEMENTARY,
        TETRADIC,
        SQUARE,
        MONOCHROMATIC
    };
    
    // Get string name of harmony type
    static std::string getHarmonyName(HarmonyType type) {
        switch (type) {
            case HarmonyType::COMPLEMENTARY: return "Complementary";
            case HarmonyType::ANALOGOUS: return "Analogous";
            case HarmonyType::TRIADIC: return "Triadic";
            case HarmonyType::SPLIT_COMPLEMENTARY: return "Split Complementary";
            case HarmonyType::TETRADIC: return "Tetradic";
            case HarmonyType::SQUARE: return "Square";
            case HarmonyType::MONOCHROMATIC: return "Monochromatic";
            default: return "Unknown";
        }
    }
    
    // Generate a color harmony based on a base color
    static std::vector<Color> generateHarmony(const Color& baseColor, HarmonyType type) {
        std::vector<Color> harmony;
        harmony.push_back(baseColor); // Always include the base color
        
        float h = baseColor.h;
        float s = baseColor.s;
        float v = baseColor.v;
        
        switch (type) {
            case HarmonyType::COMPLEMENTARY:
                // Add the complementary color (opposite on the color wheel)
                harmony.push_back(Color(fmodf(h + 180.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::ANALOGOUS:
                // Add colors adjacent to the base color on the color wheel
                harmony.push_back(Color(fmodf(h - 30.0f + 360.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 30.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::TRIADIC:
                // Add two colors at 120° intervals from the base color
                harmony.push_back(Color(fmodf(h + 120.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 240.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::SPLIT_COMPLEMENTARY:
                // Add two colors adjacent to the complementary color
                harmony.push_back(Color(fmodf(h + 150.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 210.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::TETRADIC:
                // Add colors to form a tetradic (rectangle) harmony
                harmony.push_back(Color(fmodf(h + 60.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 180.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 240.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::SQUARE:
                // Add colors to form a square harmony
                harmony.push_back(Color(fmodf(h + 90.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 180.0f, 360.0f), s, v));
                harmony.push_back(Color(fmodf(h + 270.0f, 360.0f), s, v));
                break;
                
            case HarmonyType::MONOCHROMATIC:
                // Add variations of the same hue with different saturation/value
                // More pleasing gradations for better visual appeal
                harmony.push_back(Color(h, std::max(0.2f, s - 0.3f), std::min(1.0f, v + 0.1f))); // Lighter, less saturated
                harmony.push_back(Color(h, std::min(1.0f, s + 0.1f), std::max(0.4f, v - 0.2f))); // Darker, more saturated
                harmony.push_back(Color(h, std::max(0.1f, s - 0.5f), v)); // Much less saturated, same brightness
                harmony.push_back(Color(h, s, std::max(0.3f, v - 0.4f))); // Much darker, same saturation
                break;
        }
        
        return harmony;
    }
    
    // Generate all harmonies for a base color
    static std::vector<std::pair<HarmonyType, std::vector<Color>>> generateAllHarmonies(const Color& baseColor) {
        std::vector<std::pair<HarmonyType, std::vector<Color>>> allHarmonies;
        
        // For each harmony type, generate the corresponding color scheme
        allHarmonies.push_back({HarmonyType::COMPLEMENTARY, 
                             generateHarmony(baseColor, HarmonyType::COMPLEMENTARY)});
        allHarmonies.push_back({HarmonyType::ANALOGOUS, 
                             generateHarmony(baseColor, HarmonyType::ANALOGOUS)});
        allHarmonies.push_back({HarmonyType::TRIADIC, 
                             generateHarmony(baseColor, HarmonyType::TRIADIC)});
        allHarmonies.push_back({HarmonyType::SPLIT_COMPLEMENTARY, 
                             generateHarmony(baseColor, HarmonyType::SPLIT_COMPLEMENTARY)});
        allHarmonies.push_back({HarmonyType::TETRADIC, 
                             generateHarmony(baseColor, HarmonyType::TETRADIC)});
        allHarmonies.push_back({HarmonyType::SQUARE, 
                             generateHarmony(baseColor, HarmonyType::SQUARE)});
        allHarmonies.push_back({HarmonyType::MONOCHROMATIC, 
                             generateHarmony(baseColor, HarmonyType::MONOCHROMATIC)});
        
        return allHarmonies;
    }
};

// ColorWheel class - interactive color wheel visualization and selection
class ColorWheel {
private:
    Color selectedColor;
    std::vector<std::pair<ColorHarmony::HarmonyType, std::vector<Color>>> harmonies;
    int selectedHarmony = 0;
    
    // ASCII/ANSI visualization settings
    const int wheelRadius = 10;
    const int wheelWidth = wheelRadius * 2 + 1;
    const int wheelHeight = wheelRadius * 2 + 1;
    
public:
    // Initialize with default red color
    ColorWheel() : selectedColor(Color(0.0f, 1.0f, 1.0f)) {
        updateHarmonies();
    }
    
    // Initialize with specific color
    ColorWheel(const Color& color) : selectedColor(color) {
        updateHarmonies();
    }
    
    // Update the harmonies based on the current selected color
    void updateHarmonies() {
        harmonies = ColorHarmony::generateAllHarmonies(selectedColor);
    }
    
    // Set the selected color and update harmonies
    void setColor(const Color& color) {
        selectedColor = color;
        updateHarmonies();
    }
    
    // Set the selected color by HSV components
    void setColorHSV(float h, float s, float v) {
        selectedColor.setHSV(h, s, v);
        updateHarmonies();
    }
    
    // Set the selected color by RGB components
    void setColorRGB(int r, int g, int b) {
        selectedColor.setRGB(r, g, b);
        updateHarmonies();
    }
    
    // Get the currently selected color
    Color getSelectedColor() const {
        return selectedColor;
    }
    
    // Cycle through harmony types
    void nextHarmony() {
        selectedHarmony = (selectedHarmony + 1) % harmonies.size();
    }
    
    void previousHarmony() {
        selectedHarmony = (selectedHarmony - 1 + harmonies.size()) % harmonies.size();
    }
    
    // Get current harmony type
    ColorHarmony::HarmonyType getCurrentHarmonyType() const {
        return harmonies[selectedHarmony].first;
    }
    
    // Get current harmony colors
    std::vector<Color> getCurrentHarmonyColors() const {
        return harmonies[selectedHarmony].second;
    }
    
    // Get current harmony name
    std::string getCurrentHarmonyName() const {
        return ColorHarmony::getHarmonyName(getCurrentHarmonyType());
    }
    
    // Display the color wheel and harmonies as ASCII/ANSI art
    void display() const {
        // Terminal dimensions and aspect ratio correction
        const float aspectRatio = 0.5f; // Terminal characters are typically taller than wide
        
        // Get the current harmony's colors for display
        std::vector<Color> harmonyColors = getCurrentHarmonyColors();
        
        // Print the color wheel header
        std::cout << "\n==== Color Wheel - Selected: " << selectedColor.getHexCode() 
                  << " " << selectedColor.getColoredChar(' ') << " ====\n\n";
        
        // Print the color wheel
        for (int y = -wheelRadius; y <= wheelRadius; y++) {
            for (int x = -wheelRadius; x <= wheelRadius; x++) {
                // Apply aspect ratio correction
                float normalized_x = x / (float)wheelRadius;
                float normalized_y = y / (float)wheelRadius * aspectRatio;
                
                // Calculate distance from center
                float distance = sqrt(normalized_x * normalized_x + normalized_y * normalized_y);
                
                if (distance <= 1.0f) {
                    // Convert from cartesian to polar coordinates
                    float angle = atan2(normalized_y, normalized_x) * 180.0f / M_PI;
                    if (angle < 0) angle += 360.0f;
                    
                    // Use angle as hue, distance from center as saturation
                    Color wheelColor(angle, distance, 1.0f);
                    
                    // Check if this point is the selected color
                    bool isSelected = std::abs(angle - selectedColor.h) < 5.0f && 
                                    std::abs(distance - selectedColor.s) < 0.1f;
                    
                    // Check if this point is one of the harmony colors
                    bool isHarmonyColor = false;
                    for (const auto& color : harmonyColors) {
                        if (std::abs(angle - color.h) < 5.0f && 
                            std::abs(distance - color.s) < 0.1f) {
                            isHarmonyColor = true;
                            break;
                        }
                    }
                    
                    if (isSelected) {
                        // Mark selected color with an X
                        std::cout << wheelColor.getANSIBgColor() << "X\033[0m";
                    } else if (isHarmonyColor) {
                        // Mark harmony colors with an O
                        std::cout << wheelColor.getANSIBgColor() << "O\033[0m";
                    } else {
                        // Regular wheel point
                        std::cout << wheelColor.getColoredChar(' ');
                    }
                } else {
                    // Outside the wheel
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
        
        // Display harmony information with more visual appeal
        std::cout << "\n\033[1m" << getCurrentHarmonyName() << " Harmony\033[0m (";
        std::cout << selectedHarmony + 1 << "/" << harmonies.size() << ")\n\n";
        
        // Display harmony colors in a more visually appealing way
        std::cout << "Harmony Preview: ";
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            // Display color blocks next to each other
            const Color& color = harmonyColors[i];
            std::cout << color.getColoredChar(' ') << color.getColoredChar(' ') << color.getColoredChar(' ');
        }
        std::cout << "\n\n";
        
        // Display detailed color information
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            const Color& color = harmonyColors[i];
            std::cout << "Color " << (i + 1) << ": " << color.getHexCode() << " " 
                      << color.getColoredChar(' ') << " ";
            
            // Show whether this is the base color or a harmony color
            if (i == 0) {
                std::cout << "\033[1m(Base Color)\033[0m";
            } else {
                std::cout << "HSV(" << (int)color.h << "°, " 
                          << (int)(color.s * 100) << "%, " 
                          << (int)(color.v * 100) << "%)";
            }
            std::cout << "\n";
        }
        
        // Add a visual separator
        std::cout << "\n" << std::string(40, '-') << "\n";
        
        // Display help text
        std::cout << "\nControls:\n";
        std::cout << "  h - Change hue\n";
        std::cout << "  s - Change saturation\n";
        std::cout << "  v - Change value/brightness\n";
        std::cout << "  n - Next harmony type\n";
        std::cout << "  p - Previous harmony type\n";
        std::cout << "  r - Enter RGB values\n";
        std::cout << "  c - Copy current color to clipboard\n";
        std::cout << "  q - Exit color wheel\n";
    }
    
    // Interactive mode - let user manipulate the color wheel
    void interactive() {
        char command = ' ';
        bool running = true;
        
        while (running) {
            // Clear screen for better visualization
            std::cout << "\033[2J\033[H";
            
            // Display the wheel and current colors
            display();
            
            // Get user command
            std::cout << "\nCommand: ";
            std::cin >> command;
            
            switch (command) {
                case 'h': {
                    float h;
                    std::cout << "Enter hue (0-360): ";
                    std::cin >> h;
                    setColorHSV(h, selectedColor.s, selectedColor.v);
                    break;
                }
                case 's': {
                    float s;
                    std::cout << "Enter saturation (0.0-1.0): ";
                    std::cin >> s;
                    setColorHSV(selectedColor.h, s, selectedColor.v);
                    break;
                }
                case 'v': {
                    float v;
                    std::cout << "Enter value/brightness (0.0-1.0): ";
                    std::cin >> v;
                    setColorHSV(selectedColor.h, selectedColor.s, v);
                    break;
                }
                case 'r': {
                    int r, g, b;
                    std::cout << "Enter RGB values (0-255):\n";
                    std::cout << "R: ";
                    std::cin >> r;
                    std::cout << "G: ";
                    std::cin >> g;
                    std::cout << "B: ";
                    std::cin >> b;
                    setColorRGB(r, g, b);
                    break;
                }
                case 'n':
                    nextHarmony();
                    break;
                case 'p':
                    previousHarmony();
                    break;
                case 'c':
                    std::cout << "Color copied: " << selectedColor.getHexCode() << "\n";
                    break;
                case 'q':
                    running = false;
                    break;
                default:
                    std::cout << "Unknown command\n";
                    break;
            }
        }
    }
};

} // namespace color_wheel

#endif // COLOR_WHEEL_H
