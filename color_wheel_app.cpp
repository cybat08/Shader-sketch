#include "color_wheel.h"
#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

// Define ANSI color codes to ensure they're available regardless of the including file
#define RESET         "\033[0m"
#define BOLD          "\033[1m"
#define FG_RED        "\033[31m"
#define FG_GREEN      "\033[32m"
#define FG_YELLOW     "\033[33m"
#define FG_BLUE       "\033[34m"
#define FG_MAGENTA    "\033[35m"
#define FG_CYAN       "\033[36m"
#define FG_WHITE      "\033[37m"
#define FG_BRIGHT_RED     "\033[91m"
#define FG_BRIGHT_GREEN   "\033[92m"
#define FG_BRIGHT_YELLOW  "\033[93m"
#define FG_BRIGHT_BLUE    "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN    "\033[96m"
#define FG_BRIGHT_WHITE   "\033[97m"
#define BG_BLUE       "\033[44m"
#define BG_BRIGHT_BLUE    "\033[104m"

// Adapter functions to bridge between the application's Color/HSV and color_wheel::Color
namespace ColorWheelAdapter {
    // Convert from application HSV to color_wheel::Color
    color_wheel::Color fromAppHSV(float h, float s, float v) {
        return color_wheel::Color(h, s, v);
    }
    
    // Convert RGB from 0-1 float to 0-255 int
    int floatToByteColor(float component) {
        return static_cast<int>(component * 255.0f + 0.5f);
    }
    
    // Convert a color_wheel::Color to an ANSI color code
    std::string getAnsiColorCode(const color_wheel::Color& color) {
        return "\033[38;2;" + std::to_string(color.r) + ";" + 
               std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
    }
    
    // Simple color name categorization
    std::string getColorName(const color_wheel::Color& color) {
        // Normalize RGB to 0-1 range
        float r = color.r / 255.0f;
        float g = color.g / 255.0f;
        float b = color.b / 255.0f;

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
}

// Interactive color wheel for applications to use
class InteractiveColorWheel {
private:
    color_wheel::ColorWheel wheel;
    bool useSimplifiedMode = false;
    
    // Color history tracking (store up to 5 previous colors)
    std::vector<color_wheel::Color> colorHistory;
    int maxHistorySize = 5;
    
    // Usage tips for different harmony types
    std::map<std::string, std::pair<std::string, std::string>> harmonyTips = {
        {"Complementary", {
            "Colors opposite each other on the color wheel. Creates strong contrast and high visual impact.",
            "BEST FOR: Attention-grabbing designs, warnings, action buttons, sports logos."
        }},
        {"Analogous", {
            "Colors adjacent to each other on the color wheel. Creates a harmonious and seamless blend.",
            "BEST FOR: Natural themes, environmental designs, relaxing interfaces, peaceful imagery."
        }},
        {"Triadic", {
            "Three colors evenly spaced around the color wheel. Vibrant and balanced combination.",
            "BEST FOR: Playful designs, children's content, creative applications, diverse imagery."
        }},
        {"Split Complementary", {
            "Base color plus two colors adjacent to its complement. Less tension than complementary but still vibrant.",
            "BEST FOR: More nuanced designs, educational content, advertising, illustration."
        }},
        {"Tetradic", {
            "Two complementary color pairs. Rich but can be overwhelming if not balanced with neutrals.",
            "BEST FOR: Complex designs, fashion, artistic compositions, seasonal themes."
        }},
        {"Square", {
            "Four colors evenly spaced around the color wheel. Balanced and dynamic multi-color palette.",
            "BEST FOR: Comprehensive branding, interactive games, diverse content platforms."
        }},
        {"Monochromatic", {
            "Different shades, tones, and tints of a single hue. Subtle, sophisticated, and cohesive.",
            "BEST FOR: Elegant designs, corporate branding, minimalist interfaces, focusing attention."
        }}
    };
    
    // Examples of real-world usage for each harmony type
    std::map<std::string, std::vector<std::string>> harmonyExamples = {
        {"Complementary", {
            "Firefox logo (blue and orange)",
            "FedEx (purple and orange)",
            "Sports teams (often use opposite colors)"
        }},
        {"Analogous", {
            "Instagram logo (warm gradient)",
            "Landscapes (green-blue transitions)",
            "Facebook's blue palette"
        }},
        {"Triadic", {
            "Burger King (red, yellow, blue)",
            "Google Drive (red, yellow, green)",
            "Microsoft logo (all primary colors)"
        }},
        {"Split Complementary", {
            "Subway (yellow with green and red)",
            "Mastercard (red-orange and yellow)",
            "Various Pixar movie posters"
        }},
        {"Tetradic", {
            "eBay's colorful logo",
            "Google's multicolored G",
            "Comprehensive UI color systems"
        }},
        {"Square", {
            "NBC peacock logo",
            "Microsoft Windows classic logo",
            "Multi-brand product packaging"
        }},
        {"Monochromatic", {
            "Apple's minimal designs",
            "LinkedIn's blue palette",
            "Black and white photography"
        }}
    };
    
    // Clear the screen for better visualization
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
    
    // Draw a beautiful simplified color wheel diagram
    void drawSimplifiedColorWheel() {
        const color_wheel::Color& selectedColor = wheel.getSelectedColor();
        
        // Draw a more visually appealing simplified color wheel
        std::cout << "\n\033[1;38;5;213m╔═══════ SIMPLIFIED COLOR WHEEL ═══════╗\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m                                    \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m      " << FG_BRIGHT_YELLOW << "YELLOW (60°)" << RESET << "              \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m    " << FG_BRIGHT_YELLOW << "⬤" << RESET << "         " << FG_BRIGHT_RED << "⬤" << RESET << "              \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m " << FG_BRIGHT_GREEN << "GREEN" << RESET << " " << FG_BRIGHT_GREEN << "⬤" << RESET << "           " << FG_BRIGHT_RED << "⬤" << RESET << " " << FG_BRIGHT_RED << "RED (0°)" << RESET << "  \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m " << FG_BRIGHT_GREEN << "(120°)" << RESET << "          " << FG_BRIGHT_MAGENTA << "⬤" << RESET << " " << FG_BRIGHT_MAGENTA << "MAGENTA" << RESET << " \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m    " << FG_BRIGHT_CYAN << "⬤" << RESET << "         " << FG_BRIGHT_MAGENTA << "⬤" << RESET << "  " << FG_BRIGHT_MAGENTA << "(300°)" << RESET << "  \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m " << FG_BRIGHT_CYAN << "CYAN" << RESET << "                              \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m " << FG_BRIGHT_CYAN << "(180°)" << RESET << " " << FG_BRIGHT_BLUE << "⬤" << RESET << "     " << FG_BRIGHT_BLUE << "⬤" << RESET << " " << FG_BRIGHT_BLUE << "BLUE (240°)" << RESET << " \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m                                    \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m╚════════════════════════════════════╝\033[0m\n\n";
        
        // Create a beautiful color sample display
        std::cout << "\033[1;38;5;213m╔═══════ SELECTED COLOR ═══════╗\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m ";
        
        // Display a larger color sample with the selected color
        for (int i = 0; i < 30; i++) {
            std::cout << selectedColor.getColoredChar('#');
        }
        std::cout << " \033[1;38;5;213m║\033[0m\n";
        
        // Display color information in a formatted box
        std::cout << "\033[1;38;5;213m║\033[0m HEX: " << selectedColor.getHexCode() 
                  << "                      \033[1;38;5;213m║\033[0m\n";
        
        std::cout << "\033[1;38;5;213m║\033[0m HSV: " << std::setw(3) << (int)selectedColor.h << "°, " 
                  << std::setw(3) << (int)(selectedColor.s * 100) << "%, " 
                  << std::setw(3) << (int)(selectedColor.v * 100) << "%              \033[1;38;5;213m║\033[0m\n";
        
        std::cout << "\033[1;38;5;213m║\033[0m RGB: " << std::setw(3) << selectedColor.r << ", " 
                  << std::setw(3) << selectedColor.g << ", " 
                  << std::setw(3) << selectedColor.b << "                \033[1;38;5;213m║\033[0m\n";
                  
        std::cout << "\033[1;38;5;213m╚════════════════════════════════════╝\033[0m\n";
    }
    
    // Display the harmony information in a visually appealing way with enhanced descriptions
    void showHarmonyInfo() {
        std::string harmonyName = wheel.getCurrentHarmonyName();
        std::vector<color_wheel::Color> harmonyColors = wheel.getCurrentHarmonyColors();
        
        // Title box with harmony name
        std::cout << "\033[1;38;5;213m╔═══════ " << harmonyName << " Harmony ";
        std::cout << "(" << wheel.getSelectedHarmony() + 1 << "/" << wheel.getHarmoniesCount() << ") ═══════╗\033[0m\n";
        
        // Get harmony description and usage tip
        std::string description = harmonyTips[harmonyName].first;
        std::string usage = harmonyTips[harmonyName].second;
        
        // Show description
        std::cout << "\033[1;38;5;213m║\033[0m \033[1m🎨 Description:\033[0m " << description << std::endl;
        
        // Show usage tip
        std::cout << "\033[1;38;5;213m║\033[0m \033[1m💡 " << usage << "\033[0m" << std::endl;
        
        // Show real-world examples if available
        if (harmonyExamples.find(harmonyName) != harmonyExamples.end() && 
            !harmonyExamples[harmonyName].empty()) {
            std::cout << "\033[1;38;5;213m║\033[0m \033[1m🌎 Examples:\033[0m ";
            for (size_t i = 0; i < harmonyExamples[harmonyName].size() && i < 2; i++) {
                std::cout << harmonyExamples[harmonyName][i];
                if (i < std::min(size_t(2), harmonyExamples[harmonyName].size() - 1)) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        
        // Create a visual color palette
        std::cout << "\033[1;38;5;213m║\033[0m \033[1mPalette:\033[0m ";
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            // Display color blocks with number indicators
            const color_wheel::Color& color = harmonyColors[i];
            std::cout << "\033[1m" << (i+1) << ":\033[0m";
            for (int j = 0; j < 3; j++) {
                std::cout << color.getColoredChar('#');
            }
            std::cout << " ";
        }
        std::cout << std::endl;
        
        // Display harmony color details
        std::cout << "\033[1;38;5;213m║\033[0m \033[1mDetails:\033[0m" << std::endl;
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            const color_wheel::Color& color = harmonyColors[i];
            
            std::cout << "\033[1;38;5;213m║\033[0m   " << (i + 1) << ": " << color.getHexCode() << " ";
            
            // Color preview
            for (int j = 0; j < 3; j++) {
                std::cout << color.getColoredChar('#');
            }
            
            // Position on wheel
            std::cout << " (";
            if (i == 0) {
                std::cout << "Base";
            } else if (harmonyName == "Complementary" && i == 1) {
                std::cout << "Opposite";
            } else if (harmonyName == "Analogous") {
                if (i == 1) std::cout << "-30°";
                if (i == 2) std::cout << "+30°";
            } else if (harmonyName == "Triadic") {
                std::cout << "+" << (i * 120) << "°";
            } else if (harmonyName == "Split Complementary") {
                if (i == 1) std::cout << "+150°";
                if (i == 2) std::cout << "+210°";
            } else {
                std::cout << "Harmony " << (i+1);
            }
            std::cout << ")" << std::endl;
        }
        
        // Close the box
        std::cout << "\033[1;38;5;213m╚";
        int lineLength = 11 + harmonyName.length() + 15;
        for (int i = 0; i < lineLength; i++) {
            std::cout << "═";
        }
        std::cout << "╝\033[0m\n";
    }
    
    // Display color history
    void showColorHistory() {
        if (colorHistory.empty()) {
            return;
        }
        
        std::cout << "\033[1;38;5;213m╔═══════ COLOR HISTORY ═══════╗\033[0m\n";
        std::cout << "\033[1;38;5;213m║\033[0m ";
        
        for (size_t i = 0; i < colorHistory.size(); i++) {
            // Display color blocks
            std::cout << "\033[1m" << (i+1) << ":\033[0m";
            for (int j = 0; j < 3; j++) {
                std::cout << colorHistory[i].getColoredChar('#');
            }
            std::cout << " ";
        }
        
        std::cout << " \033[1;38;5;213m║\033[0m\n";
        std::cout << "\033[1;38;5;213m╚════════════════════════════════════╝\033[0m\n";
    }
    
    // Show controls with improved visuals
    void showControls() {
        std::cout << std::endl;
        std::cout << "\033[1;38;5;213m╔═══════ CONTROLS ═══════╗\033[0m\n";
        
        // Color adjustment
        std::cout << "\033[1;38;5;213m║\033[0m \033[1;38;5;220mColor Adjustment:\033[0m" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "h+/h-" << RESET << " - Adjust hue up/down" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "s+/s-" << RESET << " - Adjust saturation up/down" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "v+/v-" << RESET << " - Adjust brightness up/down" << std::endl;
        
        // Harmony navigation
        std::cout << "\033[1;38;5;213m║\033[0m \033[1;38;5;220mHarmony Navigation:\033[0m" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "n/p" << RESET << "   - Next/previous harmony type" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "1-5" << RESET << "   - Select color from harmony" << std::endl;
        
        // History controls
        std::cout << "\033[1;38;5;213m║\033[0m \033[1;38;5;220mHistory & Input:\033[0m" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "h1-h5" << RESET << " - Select from color history" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "r" << RESET << "     - Enter RGB values" << std::endl;
        
        // Display options
        std::cout << "\033[1;38;5;213m║\033[0m \033[1;38;5;220mDisplay Options:\033[0m" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "a" << RESET << "     - Toggle advanced/simple view" << std::endl;
        std::cout << "\033[1;38;5;213m║\033[0m  " << FG_BRIGHT_YELLOW << "q" << RESET << "     - Quit and use selected color" << std::endl;
        
        std::cout << "\033[1;38;5;213m╚════════════════════════════╝\033[0m\n";
    }
    
    // Add a color to history
    void addToColorHistory(const color_wheel::Color& color) {
        // Check if color already exists in history
        for (auto it = colorHistory.begin(); it != colorHistory.end(); ++it) {
            if (it->r == color.r && it->g == color.g && it->b == color.b) {
                colorHistory.erase(it);
                break;
            }
        }
        
        // Add new color at the beginning
        colorHistory.insert(colorHistory.begin(), color);
        
        // Trim history if needed
        if (colorHistory.size() > maxHistorySize) {
            colorHistory.resize(maxHistorySize);
        }
    }
    
public:
    // Initialize with a base HSV color
    InteractiveColorWheel(float h, float s, float v) : 
        wheel(color_wheel::Color(h, s, v)) {
        // Add initial color to history
        addToColorHistory(wheel.getSelectedColor());
    }
    
    // Run the interactive color wheel and return the selected color
    color_wheel::Color run() {
        clearScreen();
        
        // Header with enhanced styling
        std::cout << "\033[1;48;5;57;38;5;255m";
        
        // Create a centered banner with padding
        std::string title = "ENHANCED COLOR WHEEL WITH HARMONY SUGGESTIONS";
        int paddingSize = (80 - title.length()) / 2;
        std::string padding(paddingSize, ' ');
        
        std::cout << padding << title << padding;
        if ((title.length() % 2) != 0) std::cout << " "; // Handle odd lengths
        std::cout << RESET << std::endl << std::endl;
        
        // Show either the simple diagram or the full color wheel visualization
        if (useSimplifiedMode) {
            drawSimplifiedColorWheel();
        } else {
            // Show the interactive color wheel display
            wheel.display();
        }
        
        // Show color history
        showColorHistory();
        
        // Show harmony information in a more visually appealing way
        std::cout << std::endl;
        showHarmonyInfo();
        
        // Show controls
        showControls();
        
        // Get user input for interactive color selection
        std::cout << "\n\033[1;38;5;213m❯\033[0m Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        
        // Save current color before any changes for history tracking
        color_wheel::Color currentColor = wheel.getSelectedColor();
        bool colorChanged = false;
        
        if (input.empty() || input == "q" || input == "Q") {
            // Return with current color selected
            return currentColor;
        } else if (input == "a" || input == "A") {
            // Toggle between simplified and advanced visualization
            useSimplifiedMode = !useSimplifiedMode;
            std::cout << FG_BRIGHT_GREEN << "✓ " << RESET << "Switched to " 
                      << (useSimplifiedMode ? "simplified" : "advanced") 
                      << " visualization mode" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return run(); // Show updated display
        } else if (input == "h+" || input == "H+") {
            // Increase hue
            float h = wheel.getSelectedColor().h;
            h = std::fmod(h + 15.0f, 360.0f); // Add 15 degrees
            wheel.setColorHSV(h, wheel.getSelectedColor().s, wheel.getSelectedColor().v);
            colorChanged = true;
        } else if (input == "h-" || input == "H-") {
            // Decrease hue
            float h = wheel.getSelectedColor().h;
            h = std::fmod(h - 15.0f + 360.0f, 360.0f); // Subtract 15 degrees
            wheel.setColorHSV(h, wheel.getSelectedColor().s, wheel.getSelectedColor().v);
            colorChanged = true;
        } else if (input == "s+" || input == "S+") {
            // Increase saturation
            float s = std::min(1.0f, wheel.getSelectedColor().s + 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, s, wheel.getSelectedColor().v);
            colorChanged = true;
        } else if (input == "s-" || input == "S-") {
            // Decrease saturation
            float s = std::max(0.0f, wheel.getSelectedColor().s - 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, s, wheel.getSelectedColor().v);
            colorChanged = true;
        } else if (input == "v+" || input == "V+") {
            // Increase value/brightness
            float v = std::min(1.0f, wheel.getSelectedColor().v + 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, wheel.getSelectedColor().s, v);
            colorChanged = true;
        } else if (input == "v-" || input == "V-") {
            // Decrease value/brightness
            float v = std::max(0.0f, wheel.getSelectedColor().v - 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, wheel.getSelectedColor().s, v);
            colorChanged = true;
        } else if (input == "r" || input == "R") {
            // Enter RGB values
            int r, g, b;
            std::cout << FG_BRIGHT_CYAN << "🎨 " << RESET << "Enter RGB values (0-255):" << std::endl;
            std::cout << FG_BRIGHT_RED << "R: " << RESET;
            std::cin >> r;
            std::cout << FG_BRIGHT_GREEN << "G: " << RESET;
            std::cin >> g;
            std::cout << FG_BRIGHT_BLUE << "B: " << RESET;
            std::cin >> b;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));
            
            wheel.setColorRGB(r, g, b);
            colorChanged = true;
        } else if (input == "n" || input == "N") {
            // Next harmony type
            wheel.nextHarmony();
        } else if (input == "p" || input == "P") {
            // Previous harmony type
            wheel.previousHarmony();
        } else if (input == "1" || input == "2" || input == "3" || input == "4" || input == "5") {
            // Select a color from the current harmony
            int index = input[0] - '1';
            std::vector<color_wheel::Color> harmonyColors = wheel.getCurrentHarmonyColors();
            
            if (index >= 0 && index < harmonyColors.size()) {
                wheel.setColor(harmonyColors[index]);
                colorChanged = true;
                std::cout << FG_BRIGHT_GREEN << "✓ " << RESET 
                          << "Selected harmony color " << (index + 1) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << RESET << "Invalid color index" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        } else if (input.length() == 2 && input[0] == 'h' && input[1] >= '1' && input[1] <= '5') {
            // Select from color history
            int historyIndex = input[1] - '1';
            if (historyIndex >= 0 && historyIndex < colorHistory.size()) {
                wheel.setColor(colorHistory[historyIndex]);
                colorChanged = true;
                std::cout << FG_BRIGHT_GREEN << "✓ " << RESET 
                          << "Selected color from history" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << RESET << "Invalid history index" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        } else if (input == "i" || input == "I") {
            // Enter classic interactive mode
            wheel.interactive();
            colorChanged = true;
        } else {
            // Invalid input
            std::cout << FG_BRIGHT_RED << "⚠ " << RESET << "Invalid command" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
        // If color changed, add to history
        if (colorChanged) {
            color_wheel::Color newColor = wheel.getSelectedColor();
            // Only add if it's different from before
            if (newColor.r != currentColor.r || 
                newColor.g != currentColor.g || 
                newColor.b != currentColor.b) {
                addToColorHistory(newColor);
            }
        }
        
        // Continue the color wheel interaction
        return run();
    }
};