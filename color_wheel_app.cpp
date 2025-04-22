#include "color_wheel.h"
#include <iostream>
#include <string>
#include <limits>

// Constants for terminal output
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
    
    // Clear the screen for better visualization
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
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
        
        // Show the interactive color wheel display
        wheel.display();
        
        // Get user input for interactive color selection
        std::cout << "\nEnter command (or 'i' for fully interactive mode): ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            // Return with current color selected
            return wheel.getSelectedColor();
        } else if (input == "q" || input == "Q") {
            // Cancel and return current color without changes
            return wheel.getSelectedColor();
        } else if (input == "i" || input == "I") {
            // Enter fully interactive mode
            wheel.interactive();
            return wheel.getSelectedColor();
        } else if (input == "h" || input == "H") {
            // Change hue
            float h;
            std::cout << "Enter hue (0-360): ";
            std::cin >> h;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            wheel.setColorHSV(h, wheel.getSelectedColor().s, wheel.getSelectedColor().v);
            return run(); // Recursively call to show updated wheel
        } else if (input == "s" || input == "S") {
            // Change saturation
            float s;
            std::cout << "Enter saturation (0.0-1.0): ";
            std::cin >> s;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            wheel.setColorHSV(wheel.getSelectedColor().h, s, wheel.getSelectedColor().v);
            return run(); // Recursively call to show updated wheel
        } else if (input == "v" || input == "V") {
            // Change value/brightness
            float v;
            std::cout << "Enter value/brightness (0.0-1.0): ";
            std::cin >> v;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            wheel.setColorHSV(wheel.getSelectedColor().h, wheel.getSelectedColor().s, v);
            return run(); // Recursively call to show updated wheel
        } else if (input == "r" || input == "R") {
            // Enter RGB values
            int r, g, b;
            std::cout << "Enter RGB values (0-255):\n";
            std::cout << "R: ";
            std::cin >> r;
            std::cout << "G: ";
            std::cin >> g;
            std::cout << "B: ";
            std::cin >> b;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer
            
            wheel.setColorRGB(r, g, b);
            return run(); // Recursively call to show updated wheel
        } else if (input == "n" || input == "N") {
            // Next harmony type
            wheel.nextHarmony();
            return run(); // Recursively call to show updated wheel
        } else if (input == "p" || input == "P") {
            // Previous harmony type
            wheel.previousHarmony();
            return run(); // Recursively call to show updated wheel
        } else if (input == "1" || input == "2" || input == "3" || input == "4" || input == "5") {
            // Select a color from the current harmony
            int index = input[0] - '1';
            std::vector<color_wheel::Color> harmonyColors = wheel.getCurrentHarmonyColors();
            
            if (index >= 0 && index < harmonyColors.size()) {
                wheel.setColor(harmonyColors[index]);
                return run(); // Recursively call to show updated wheel
            } else {
                std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid color index" << RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                return run(); // Recursively call to show updated wheel
            }
        } else {
            // Invalid input
            std::cout << FG_BRIGHT_RED << "⚠ " << "Invalid command" << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return run(); // Invalid input, show again
        }
        
        // Should never reach here, but just in case
        return wheel.getSelectedColor();
    }
};