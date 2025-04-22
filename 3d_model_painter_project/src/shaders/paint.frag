#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D layerTexture;
uniform int layerIndex;
uniform float layerOpacity;

void main() {
    // Get the paint color from the layer texture
    vec4 paintColor = texture(layerTexture, TexCoords);
    
    // Apply layer opacity
    paintColor.a *= layerOpacity;
    
    // Output the paint color
    FragColor = paintColor;
}
