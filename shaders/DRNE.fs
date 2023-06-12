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
    // Set fragment out colour to that of the emission texture
    FragColour = texture(texture3, textureCoordinate);  // Emission texture

    // We're doing all the lighting calculations in world space
    // Normalize the normal
    vec3 v3Normal = normalize(normal);
    
    // Compute the directional light's contribution
    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float fLightDirectionalContribution = max(dot(v3Normal, v3LightDirectionalDirection), 0.0);


    // Compute the point lights

 
    // Compute the direction vector between the light position and the fragment's position.
    // The light's direction vector is the difference vector between the light's position vector and the fragment's position vector.
    // We calculate this difference by subtracting both vectors from each other.
    // We also normalize just incase the vector isn't
    vec3 v3PointLightDir = normalize(v3LightPointPosition0 - vertexPosWorld);

    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float fLightPointContribution= max(dot(v3Normal, v3PointLightDir), 0.0);

    // Now that we have fLightDirectionalContribution and fLightPointComponent, we can calculate the diffuse colour
    vec4 v4Diffuse = vec4(fLightPointContribution * v3LightPointColour0, 1.0f);
    v4Diffuse += vec4(fLightDirectionalContribution * v3LightDirectionalColour, 1.0f);
    v4Diffuse = normalize(v4Diffuse);

    // Multiple diffuse contribution by diffuse texture
    v4Diffuse *= texture(texture0, textureCoordinate); // Diffuse texture

    // Add the diffuse lighting to the fragments output colour
    FragColour += v4Diffuse;

    // Multiply by vertex colours
    FragColour *= colour;
}