// DRNE: Diffuse Roughness Normal Emission
#version 330 core
out vec4 FragColour;    // Final output colour of fragment

// Inputs from vertex shader
in vec3 vec3VertexNormal;       // Vertex normal
in vec4 vec4VertexColour;       // Vertex colour
in vec2 vec2TextureCoordinate;  // Texture coordinates
in vec3 vec3VertexPosWorld;     // Vertex position in world space (for lighting calculations)

// Texture samplers
uniform sampler2D texture0_diffuse;     // Texture holding diffuse colour
uniform sampler2D texture1_roughness;   // Texture holding roughness
uniform sampler2D texture2_normal;      // Texture holding Normals
uniform sampler2D texture3_emission;    // Texture holding emission

// Light uniforms
uniform vec3 v3LightDirectionalColour;      // Colour of directional light
uniform vec3 v3LightDirectionalDirection;   // Direction of directional light

// Point lights
uniform int iLightPointNumber;              // Number of positional lights to use. Range 0-4.
#define NUM_POINT_LIGHTS_MAX 4              // Maximum number of point lights we can use
struct PointLight
{
    vec3 v3Colour;           // Colour of point light
    vec3 v3Position;         // Position in world space
    float fSpecularAttenuationLinear;       // Affects attenuation of specular lighting
    float fSpecularAttenuationQuadratic;    // Affects attenuation of specular lighting
};
uniform PointLight pointLights[NUM_POINT_LIGHTS_MAX];   // Array holding each point light

// Camera uniforms
uniform vec3 v3CameraPositionWorld;         // Camera position in world space

// Ambient/Specular uniforms
uniform float fAmbientStrength;             // Ambient strength, default is 0.05f
uniform float fSpecularStrength;            // Specular strength, default is 0.5f

// Function declarations to help make the code more readable
vec3 computeDirectionalLight(void); // Computes and returns the colour contribution of the directional light
vec3 computePositionalLight(int iLightNumber);  // Computes and returns the colour contribution of a point light

void main()
{
    // Initially set fragment out colour to that of the emission texture
    // v3Output will accumulate all lights' contribution to set final output colour
    vec3 v3Output = texture(texture3_emission, vec2TextureCoordinate).rgb;

    // Add the directional light's colour contribution
    v3Output += computeDirectionalLight();

    // Add each point light's colour contribution
    for (int i=0; i<iLightPointNumber; i++)
    {
        v3Output += computePositionalLight(i);
    }

    // Set final output colour
    FragColour = vec4(v3Output, 1.0);
}

vec3 computeDirectionalLight(void)
{
    vec3 lightDir = normalize(-v3LightDirectionalDirection);
    // Diffuse
    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float diff = max(dot(vec3VertexNormal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDir, vec3VertexNormal);
    vec3 viewDir = normalize(v3CameraPositionWorld - vec3VertexPosWorld);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularStrength * 128.0);
    // Combine results
    vec3 ambient  = fAmbientStrength * vec3(texture(texture0_diffuse, vec2TextureCoordinate));
    vec3 diffuse  = v3LightDirectionalColour  * diff * vec3(texture(texture0_diffuse, vec2TextureCoordinate));
    vec3 specular = v3LightDirectionalColour * spec * vec3(texture(texture1_roughness, vec2TextureCoordinate));
    return (ambient + diffuse + specular);
}

vec3 computePositionalLight(int iLightNumber)
{
    vec3 lightDir = normalize(pointLights[iLightNumber].v3Position - vec3VertexPosWorld);
    // Diffuse shading
    float diff = max(dot(vec3VertexNormal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, vec3VertexNormal);
    vec3 viewDir = normalize(v3CameraPositionWorld - vec3VertexPosWorld);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularStrength * 128.0);
    // Specular attenuation
    float distance = length(pointLights[iLightNumber].v3Position - vec3VertexPosWorld);
    float attenuation = 1.0 / (1.0 + pointLights[iLightNumber].fSpecularAttenuationLinear * distance + pointLights[iLightNumber].fSpecularAttenuationQuadratic * (distance * distance));    
    // combine results
    vec3 ambient  = fAmbientStrength  * vec3(texture(texture0_diffuse, vec2TextureCoordinate));
    vec3 diffuse  = pointLights[iLightNumber].v3Colour * diff * vec3(texture(texture0_diffuse, vec2TextureCoordinate));
    vec3 specular = pointLights[iLightNumber].v3Colour * spec * vec3(texture(texture1_roughness, vec2TextureCoordinate));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
