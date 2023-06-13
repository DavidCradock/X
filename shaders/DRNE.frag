// DRNE: Diffuse Roughness Normal Emission
#version 330 core
out vec4 FragColour;    // Final output colour of fragment

// Inputs from vertex shader
in vec3 vec3VertexNormal;       // Vertex normal
in vec4 vec4VertexColour;       // Vertex colour
in vec2 vec2TextureCoordinate;  // Texture coordinates
in vec3 vec3VertexPosWorld;     // Vertex position in world space (for lighting calculations)

// Texture samplers
uniform sampler2D texture0;     // Diffuse
uniform sampler2D texture1;     // Roughness
uniform sampler2D texture2;     // Normal
uniform sampler2D texture3;     // Emission

// Light uniforms
uniform vec3 v3LightDirectionalColour;      // Colour of directional light
uniform vec3 v3LightDirectionalDirection;   // Direction of directional light
uniform vec3 v3LightPointColour0;           // Colour of point light 0
uniform vec3 v3LightPointPosition0;         // Position in world space of point light 0

// Camera uniforms
uniform vec3 v3CameraPositionWorld;         // Camera position in world space

// Specular uniforms
uniform float fSpecularStrength;            // Specular strength

// Function declarations to help make the code more readable
vec3 computeDirectionalLight(void); // Computes and returns the colour contribution of the directional light


void main()
{
    // Initially set fragment out colour to that of the emission texture
    FragColour = texture(texture3, vec2TextureCoordinate);  // Emission texture

    // We're doing all the lighting calculations in world space
    // Normalize the normal
    vec3 v3Normal = normalize(vec3VertexNormal);
    
    // Compute the directional light's contribution
    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float fLightDirectionalContribution = max(dot(v3Normal, v3LightDirectionalDirection), 0.0);


    // Compute the point lights

 
    // Compute the direction vector between the light position and the fragment's position.
    // The light's direction vector is the difference vector between the light's position vector and the fragment's position vector.
    // We calculate this difference by subtracting both vectors from each other.
    // We also normalize just incase the vector isn't
    vec3 v3PointLightDir = normalize(v3LightPointPosition0 - vec3VertexPosWorld);

    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float fLightPointContribution= max(dot(v3Normal, v3PointLightDir), 0.0);

    // Now that we have fLightDirectionalContribution and fLightPointComponent, we can calculate the diffuse colour
    vec4 v4Diffuse = vec4(fLightPointContribution * v3LightPointColour0, 1.0f);
    v4Diffuse += vec4(fLightDirectionalContribution * v3LightDirectionalColour, 1.0f);
 
    // Specular
 //   vec4 vRoughness = texture(texture1, textureCoordinate);
 //   float fSpecularPower = vRoughness.x + vRoughness.y + vRoughness.z;
 //   fSpecularPower /= 3.0f;
  //  fSpecularPower *= 256;
//    vec3 viewDir = normalize(v3CameraPositionWorld - vertexPosWorld);
//    vec3 reflectDir = reflect(-v3PointLightDir, v3Normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularPower);
//    vec3 specular = fSpecularStrength * spec * v3LightPointColour0;

    // Multiple diffuse contribution by diffuse texture
    v4Diffuse *= texture(texture0, vec2TextureCoordinate); // Diffuse texture

    // Add the diffuse lighting to the fragments output colour
    FragColour += v4Diffuse;

    // Multiply by vertex colours
    FragColour *= vec4VertexColour;
}

vec3 computeDirectionalLight(void)
{
    vec3 colour;
    return colour;
}