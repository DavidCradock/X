// DRNE.frag
// Diffuse Roughness Normal Emission
#version 330 core
out vec4 FragColour;    // Final output colour of fragment

// Inputs from vertex shader
in VS_OUT{
    //vec3 vec3VertexNormal;    // Don't need this as we're using the TBN matrix
    vec4 vec4VertexColour;
    vec2 vec2TextureCoordinate;
    mat3 matTBN;
    vec3 vec3VertexPosWorld;
    vec4 FragPosLightSpace;
} fs_in;

// Texture samplers
uniform sampler2D texture0_diffuse;     // Texture holding diffuse colour
uniform sampler2D texture1_roughness;   // Texture holding roughness
uniform sampler2D texture2_normal;      // Texture holding Normals
uniform sampler2D texture3_emission;    // Texture holding emission
uniform sampler2D texture4_depthmap;    // Texture holding depthmap from directional light for shadows

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
float computeShadows(vec3 vNormal, vec3 vLightDir);   // Computes whether the fragmant is or isn't in shadow, returning 1.0 when in shadow, 0.0 when not

void main()
{
    // Initially set fragment out colour to that of the emission texture
    // v3Output will accumulate all lights' contribution to set final output colour
    vec3 v3Output = texture(texture3_emission, fs_in.vec2TextureCoordinate).rgb;

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
    // Normal/Bump mapping
    // We need to adjust the normal from normal space using the TBN matrix from the vertex shader, to world space
//    vec3 vNormal = vec3VertexNormal;  // (Don't need this as using TBN)
    // Get normal in tangent space from the bump/normal map and expand it 
    vec3 vNormalImage = vec3(texture(texture2_normal, fs_in.vec2TextureCoordinate));  // Get normal from colour (will be in range of 0.0 to 1.0)
    vNormalImage *= 2.0;    // Range is now 0.0 to 2.0
    vNormalImage -= 1.0;    // Range is now -1.0 to 1.0
    // Multiply the normal from tangent space to world space
    vec3 vNormal = normalize(fs_in.matTBN * vNormalImage);

    vec3 lightDir = normalize(-v3LightDirectionalDirection);
    // Diffuse
    // Compute the angle (dot product) between the normal and light direction
    // Then use max() function, which keeps the value between 0 and 1
    float diff = max(dot(vNormal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDir, vNormal);
    vec3 viewDir = normalize(v3CameraPositionWorld - fs_in.vec3VertexPosWorld);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularStrength * 256.0);

    // Combine results
    vec3 ambient  = fAmbientStrength * vec3(texture(texture0_diffuse, fs_in.vec2TextureCoordinate));
    vec3 diffuse  = v3LightDirectionalColour  * diff * vec3(texture(texture0_diffuse, fs_in.vec2TextureCoordinate));
    vec3 specular = v3LightDirectionalColour * spec * vec3(texture(texture1_roughness, fs_in.vec2TextureCoordinate));

    // Compute shadow (Returns 1 when in shadow, 0 when not)
    float shadow = 1.0 - computeShadows(vNormal, lightDir);
    // Now we modify diffuse and specular
    diffuse *= shadow;
    specular *= shadow;

    return (ambient + diffuse + specular);
}

vec3 computePositionalLight(int iLightNumber)
{
    // Normal/Bump mapping
    // We need to adjust the normal from normal space using the TBN matrix from the vertex shader, to world space
//    vec3 vNormal = vec3VertexNormal;  // (Don't need this as using TBN)
    // Get normal in tangent space from the bump/normal map and expand it 
    vec3 vNormalImage = vec3(texture(texture2_normal, fs_in.vec2TextureCoordinate));  // Get normal from colour (will be in range of 0.0 to 1.0)
    vNormalImage *= 2.0;    // Range is now 0.0 to 2.0
    vNormalImage -= 1.0;    // Range is now -1.0 to 1.0
    // Multiply the normal from tangent space to world space
    vec3 vNormal = normalize(fs_in.matTBN * vNormalImage);

    vec3 lightDir = normalize(pointLights[iLightNumber].v3Position - fs_in.vec3VertexPosWorld);
    // Diffuse shading
    float diff = max(dot(vNormal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, vNormal);
    vec3 viewDir = normalize(v3CameraPositionWorld - fs_in.vec3VertexPosWorld);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularStrength * 128.0);
    // Specular attenuation
    float distance = length(pointLights[iLightNumber].v3Position - fs_in.vec3VertexPosWorld);
    float attenuation = 1.0 / (1.0 + pointLights[iLightNumber].fSpecularAttenuationLinear * distance + pointLights[iLightNumber].fSpecularAttenuationQuadratic * (distance * distance));    
    // combine results
    vec3 ambient  = fAmbientStrength  * vec3(texture(texture0_diffuse, fs_in.vec2TextureCoordinate));
    vec3 diffuse  = pointLights[iLightNumber].v3Colour * diff * vec3(texture(texture0_diffuse, fs_in.vec2TextureCoordinate));
    vec3 specular = pointLights[iLightNumber].v3Colour * spec * vec3(texture(texture1_roughness, fs_in.vec2TextureCoordinate));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

float computeShadows(vec3 vNormal, vec3 vLightDir)
{
    // Transform the light-space fragment position in clip-space to normalized device coordinates
    // When we output a clip-space vertex position to gl_Position in the vertex shader, OpenGL automatically does a perspective divide
    // e.g. transform clip-space coordinates in the range [-w,w] to [-1,1] by dividing the x, y and z component by the vector's w component.
    // As the clip-space FragPosLightSpace is not passed to the fragment shader through gl_Position, we have to do this perspective divide ourselves...
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;  // Will be in range -1, 1
    // NOTE: When using an orthographic projection matrix, we don't have to do this, but keeping this in ensures everything works when we render the depthmap using a projection matrix.
    projCoords = projCoords * 0.5 + 0.5;    // Convert from -1, 1 to 0,1 as the values in depth map are in range of 0,1
 //   float closestDepth = texture(texture4_depthmap, projCoords.xy).r;   // Get depth value from depth map
    float closestDepth = 0.0f;
    // Now closestDepth holds the depth value and we're done. But, this produces hard shadows.
    // Instead, let's sample the depth map multiple times from slightly offset coordinates, average the sum to create softer shadows.
    vec2 texelSize = 1.0 / textureSize(texture4_depthmap, 0);
    texelSize *= 1.5f;
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            vec2 vTextureCoords = vec2(x, y);
            vTextureCoords.x *= texelSize.x;
            vTextureCoords.y *= texelSize.y;
            vTextureCoords += projCoords.xy;
            float pcfDepth = texture(texture4_depthmap, vTextureCoords).r; 
            closestDepth += pcfDepth;
        }    
    }
    closestDepth /= 25.0;

    float currentDepth = projCoords.z;  // Get depth of fragment

    // Introduce a little bias to help with "shadow acne"
    // We use a very small value to offset the fragment's depth value to eliminate "shadow acne", but we
    // wish to keep this as small as possible.
    // A small value is fine if the fragment's normal is perpendicular, but as the angle increases, we need a larger value.
    // We can compute the bias based on the angle from the light to the normal using dot product, so instead of setting a setting a single bias value like...
    // float bias = 0.001;
    // We can increase the bias based on the angle like this, keeping the bias value within a certain range with max()
    float bias = max(0.01 * (1.0 - dot(vNormal, vLightDir)), 0.001);
    currentDepth -= bias;  
    float fShadow = currentDepth > closestDepth ? 1.0 : 0.0;

    // Fix when texture coordinates are out of range
    if(projCoords.z > 1.0)
        fShadow = 0.0;
    return fShadow;
}
