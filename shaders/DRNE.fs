// DRNE: Diffuse Roughness Normal Emission
#version 330 core
out vec4 FragColour;    // Final output colour of fragment

// Inputs from vertex shader
in vec3 normal;                 // Vertex normal
in vec4 colour;                 // Vertex colour
in vec2 textureCoordinate;      // Texture coordinates
in vec3 vertexPosWorld;         // Vertex position in world space (for lighting calculations)

// Textures
uniform sampler2D texture0;     // Diffuse
uniform sampler2D texture1;     // Roughness
uniform sampler2D texture2;     // Normal
uniform sampler2D texture3;     // Emission

// Lights
uniform vec3 v3LightDirectionalColour;      // Colour of directional light
uniform vec3 v3LightDirectionalDirection;   // Direction of directional light
uniform vec3 v3LightPointColour0;           // Colour of point light 0
uniform vec3 v3LightPointPosition0;         // Position in world space of point light 0

void main()
{
    // We're doing all the lighting calculations in world space
    vec3 v3Normal = normalize(normal);  // Normalize the normal
    // The direction vector between the light and the fragment's position.
    // The light's direction vector is the difference vector between the light's position vector and the fragment's position vector.
    // We calculate this difference by subtracting both vectors from each other.
    vec3 v3LightDir = normalize(v3LightPointPosition0 - vertexPosWorld);
    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float diffComponent = max(dot(v3Normal, v3LightDir), 0.0);
    vec3 diffuse = diffComponent * v3LightPointColour0;

    FragColour = texture(texture3, textureCoordinate);  // Emission texture
    FragColour += texture(texture0, textureCoordinate); // Diffuse
//    FragColour = mix(texture(texture0, textureCoordinate), texture(texture1, textureCoordinate), 0.5);
    FragColour *= colour;   // Multiply be vertex colours
    FragColour *= vec4(diffuse, 1.0f);
}