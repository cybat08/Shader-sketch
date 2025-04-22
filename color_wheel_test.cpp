#include "color_wheel.h"
#include <iostream>

int main() {
    std::cout << "3D Model Painter - Color Wheel Test\n";
    std::cout << "===================================\n\n";
    
    // Create a color wheel with default red color
    color_wheel::ColorWheel wheel;
    
    // Run the interactive color wheel
    wheel.interactive();
    
    // After exiting, show the selected color
    color_wheel::Color selectedColor = wheel.getSelectedColor();
    std::cout << "\nFinal selected color: " << selectedColor.getHexCode() 
              << " " << selectedColor.getColoredChar(' ') << "\n";
    
    // Show all harmonies for the selected color
    std::cout << "\nAll harmonies for selected color:\n";
    auto allHarmonies = color_wheel::ColorHarmony::generateAllHarmonies(selectedColor);
    
    for (const auto& harmony : allHarmonies) {
        std::cout << "- " << color_wheel::ColorHarmony::getHarmonyName(harmony.first) << ": ";
        
        for (const auto& color : harmony.second) {
            std::cout << color.getColoredChar(' ');
        }
        std::cout << "\n";
    }
    
    std::cout << "\nThank you for using the Color Wheel!\n";
    return 0;
}