#include "color_wheel.h"
#include <iostream>
#include <string>
#include <limits>

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
    
    // Clear the screen for better visualization
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
    
    // Draw a simplified color wheel diagram
    void drawSimplifiedColorWheel() {
        const color_wheel::Color& selectedColor = wheel.getSelectedColor();
        
        std::cout << "                " << std::endl;
        std::cout << "     " << FG_YELLOW << "YELLOW" << RESET << "     " << std::endl;
        std::cout << "    " << FG_YELLOW << "/" << RESET << "       " << FG_RED << "\\" << RESET << std::endl;
        std::cout << "   " << FG_GREEN << "/" << RESET << "         " << FG_RED << "\\" << RESET << "   " << std::endl;
        std::cout << FG_GREEN << "GREEN" << RESET << "           " << FG_RED << "RED" << RESET << std::endl;
        std::cout << "   " << FG_GREEN << "\\" << RESET << "         " << FG_RED << "/" << RESET << "   " << std::endl;
        std::cout << "    " << FG_CYAN << "\\" << RESET << "       " << FG_MAGENTA << "/" << RESET << "    " << std::endl;
        std::cout << "     " << FG_BLUE << "BLUE" << RESET << "     " << std::endl;
        std::cout << "                " << std::endl;
        
        // Show selected color sample
        std::cout << std::endl;
        std::cout << "Selected Color: ";
        for (int i = 0; i < 10; i++) {
            std::cout << selectedColor.getColoredChar(' ');
        }
        std::cout << " " << selectedColor.getHexCode();
        std::cout << " (HSV: " << selectedColor.h << "°, " 
                  << (int)(selectedColor.s * 100) << "%, " 
                  << (int)(selectedColor.v * 100) << "%)" << std::endl;
        
        // Show color in RGB format
        std::cout << "RGB: (" << selectedColor.r << ", " 
                  << selectedColor.g << ", " 
                  << selectedColor.b << ")" << std::endl;
    }
    
    // Display the harmony information in a more visually appealing way
    void showHarmonyInfo() {
        std::string harmonyName = wheel.getCurrentHarmonyName();
        std::vector<color_wheel::Color> harmonyColors = wheel.getCurrentHarmonyColors();
        
        std::cout << BOLD << BG_BLUE << FG_BRIGHT_WHITE << " " << harmonyName << " Harmony " 
                  << RESET << std::endl << std::endl;
        
        // Show explanation of the harmony type
        std::string explanation;
        if (harmonyName == "Complementary") {
            explanation = "Colors opposite each other on the color wheel. Creates strong contrast.";
        } else if (harmonyName == "Analogous") {
            explanation = "Colors adjacent to each other on the color wheel. Creates a harmonious feel.";
        } else if (harmonyName == "Triadic") {
            explanation = "Three colors evenly spaced around the color wheel. Vibrant and balanced.";
        } else if (harmonyName == "Split Complementary") {
            explanation = "Base color plus two colors adjacent to its complement. Less tension than complementary.";
        } else if (harmonyName == "Tetradic") {
            explanation = "Two complementary color pairs. Rich but can be overwhelming if not balanced.";
        } else if (harmonyName == "Square") {
            explanation = "Four colors evenly spaced around the color wheel. Balanced and dynamic.";
        } else if (harmonyName == "Monochromatic") {
            explanation = "Different shades, tones, and tints of a single hue. Subtle and sophisticated.";
        }
        
        std::cout << explanation << std::endl << std::endl;
        
        // Show the harmony colors with better formatting
        std::cout << BOLD << "Color Suggestions:" << RESET << std::endl;
        for (size_t i = 0; i < harmonyColors.size(); i++) {
            const color_wheel::Color& color = harmonyColors[i];
            std::cout << "  " << FG_BRIGHT_YELLOW << (i+1) << RESET << " - ";
            
            // Color preview
            for (int j = 0; j < 5; j++) {
                std::cout << color.getColoredChar(' ');
            }
            
            // Color info
            std::cout << " " << color.getHexCode() 
                      << " HSV(" << (int)color.h << "°, " << (int)(color.s*100) << "%, " << (int)(color.v*100) << "%)"
                      << std::endl;
        }
    }
    
    // Show simplified help
    void showControls() {
        std::cout << std::endl;
        std::cout << BOLD << "Controls:" << RESET << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "h+/h-" << RESET << " - Adjust hue up/down" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "s+/s-" << RESET << " - Adjust saturation up/down" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "v+/v-" << RESET << " - Adjust brightness up/down" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "n/p" << RESET << "   - Next/previous harmony type" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "1-5" << RESET << "   - Select color from harmony" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "r" << RESET << "     - Enter RGB values" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "a" << RESET << "     - Toggle advanced visualization" << std::endl;
        std::cout << "  " << FG_BRIGHT_YELLOW << "q" << RESET << "     - Quit and use selected color" << std::endl;
    }
    
public:
    // Initialize with a base HSV color
    InteractiveColorWheel(float h, float s, float v) : 
        wheel(color_wheel::Color(h, s, v)) {}
    
    // Run the interactive color wheel and return the selected color
    color_wheel::Color run() {
        clearScreen();
        
        std::cout << BOLD << BG_BRIGHT_BLUE << FG_BRIGHT_WHITE 
                  << "               ENHANCED COLOR WHEEL WITH HARMONY               " 
                  << RESET << std::endl << std::endl;
        
        // Show either the simple diagram or the full color wheel visualization
        if (useSimplifiedMode) {
            drawSimplifiedColorWheel();
        } else {
            // Show the interactive color wheel display
            wheel.display();
        }
        
        // Show harmony information in a more visually appealing way
        std::cout << std::endl;
        showHarmonyInfo();
        
        // Show controls
        showControls();
        
        // Get user input for interactive color selection
        std::cout << "\nEnter command: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty() || input == "q" || input == "Q") {
            // Return with current color selected
            return wheel.getSelectedColor();
        } else if (input == "a" || input == "A") {
            // Toggle between simplified and advanced visualization
            useSimplifiedMode = !useSimplifiedMode;
            std::cout << "Switched to " << (useSimplifiedMode ? "simplified" : "advanced") 
                      << " visualization mode" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return run(); // Show updated display
        } else if (input == "h+" || input == "H+") {
            // Increase hue
            float h = wheel.getSelectedColor().h;
            h = std::fmod(h + 15.0f, 360.0f); // Add 15 degrees
            wheel.setColorHSV(h, wheel.getSelectedColor().s, wheel.getSelectedColor().v);
            return run(); // Show updated wheel
        } else if (input == "h-" || input == "H-") {
            // Decrease hue
            float h = wheel.getSelectedColor().h;
            h = std::fmod(h - 15.0f + 360.0f, 360.0f); // Subtract 15 degrees
            wheel.setColorHSV(h, wheel.getSelectedColor().s, wheel.getSelectedColor().v);
            return run(); // Show updated wheel
        } else if (input == "s+" || input == "S+") {
            // Increase saturation
            float s = std::min(1.0f, wheel.getSelectedColor().s + 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, s, wheel.getSelectedColor().v);
            return run(); // Show updated wheel
        } else if (input == "s-" || input == "S-") {
            // Decrease saturation
            float s = std::max(0.0f, wheel.getSelectedColor().s - 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, s, wheel.getSelectedColor().v);
            return run(); // Show updated wheel
        } else if (input == "v+" || input == "V+") {
            // Increase value/brightness
            float v = std::min(1.0f, wheel.getSelectedColor().v + 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, wheel.getSelectedColor().s, v);
            return run(); // Show updated wheel
        } else if (input == "v-" || input == "V-") {
            // Decrease value/brightness
            float v = std::max(0.0f, wheel.getSelectedColor().v - 0.1f);
            wheel.setColorHSV(wheel.getSelectedColor().h, wheel.getSelectedColor().s, v);
            return run(); // Show updated wheel
        } else if (input == "r" || input == "R") {
            // Enter RGB values
            int r, g, b;
            std::cout << "Enter RGB values (0-255):" << std::endl;
            std::cout << "R: ";
            std::cin >> r;
            std::cout << "G: ";
            std::cin >> g;
            std::cout << "B: ";
            std::cin >> b;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));
            
            wheel.setColorRGB(r, g, b);
            return run(); // Show updated wheel
        } else if (input == "n" || input == "N") {
            // Next harmony type
            wheel.nextHarmony();
            return run(); // Show updated wheel
        } else if (input == "p" || input == "P") {
            // Previous harmony type
            wheel.previousHarmony();
            return run(); // Show updated wheel
        } else if (input == "1" || input == "2" || input == "3" || input == "4" || input == "5") {
            // Select a color from the current harmony
            int index = input[0] - '1';
            std::vector<color_wheel::Color> harmonyColors = wheel.getCurrentHarmonyColors();
            
            if (index >= 0 && index < harmonyColors.size()) {
                wheel.setColor(harmonyColors[index]);
                return run(); // Show updated wheel
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid color index" << RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                return run(); // Show updated wheel
            }
        } else if (input == "i" || input == "I") {
            // Enter classic interactive mode
            wheel.interactive();
            return wheel.getSelectedColor();
        } else {
            // Invalid input
            std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid command" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return run(); // Show updated wheel
        }
        
        // Should never reach here, but just in case
        return wheel.getSelectedColor();
    }
};