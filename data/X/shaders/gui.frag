// gui.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 texCoordVertex;                 // Vertex texture coordinate
in vec2 texCoordBackground;             // Background texture coordinate for sampling the background texture

uniform sampler2D texture0_blur;
uniform sampler2D texture1_colour;
uniform sampler2D texture2_glow;
uniform sampler2D texture3_normal;
uniform sampler2D texture4_reflection;
uniform sampler2D texture5_background;

uniform float fBlurAmount;
uniform float fGlowAmount;
uniform float fNormalAmount;
uniform float fReflectionAmount;

void main()
{
    // Convert background texture coordinate from vertex position to 0.0f to 1.0f range
    vec2 v2BGCoords = 1.0 / textureSize(texture5_background, 0);
    v2BGCoords.x *= texCoordBackground.x;
    v2BGCoords.y *= texCoordBackground.y;
    v2BGCoords.y = 1.0f - v2BGCoords.y;

    // Use those to compute reflection texture coordinates
    vec2 v2ReflectionCoords = 1.0 / textureSize(texture4_reflection, 0);
    v2ReflectionCoords.x *= texCoordBackground.x;
    v2ReflectionCoords.y *= texCoordBackground.y;

    vec4 v4Colour = texture(texture1_colour, texCoordVertex);
    vec4 v4BackgroundColour = texture(texture5_background, v2BGCoords);
    vec4 v4ReflectionColour = fReflectionAmount * texture(texture4_reflection, v2ReflectionCoords);
    

    FragColour = v4Colour + (v4Colour.a * v4ReflectionColour);

//    FragColour += v4BackgroundColour;
    FragColour *= colour;

}