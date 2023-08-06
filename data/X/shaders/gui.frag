// gui.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 textureCoordinate;

uniform sampler2D texture0_colour;
uniform sampler2D texture1_normal;
uniform sampler2D texture2_reflection;
uniform sampler2D texture3_background;

uniform float fNormalAmount;
uniform float fReflectionAmount;
uniform float fMouseCursorDistance;

uniform vec2 v2MousePos;    // Y axis should be inverted

void main()
{
    // Convert background texture coordinate from fragment position to 0.0f to 1.0f range
    vec2 v2BGDims = textureSize(texture3_background, 0);
    vec2 v2BGCoords = 1.0 / v2BGDims;
    v2BGCoords.x *= gl_FragCoord.x;
    v2BGCoords.y *= gl_FragCoord.y;

    // Compute reflection texture coordinates
    vec2 v2ReflectionTextureDims = textureSize(texture2_reflection, 0);
    vec2 v2RefScalar = v2ReflectionTextureDims / v2BGDims;
    vec2 v2ReflectionCoords = vec2((gl_FragCoord.x * v2RefScalar.x) - v2ReflectionTextureDims.x, (gl_FragCoord.y * v2RefScalar.y) - v2ReflectionTextureDims.y);
    v2ReflectionCoords /= v2ReflectionTextureDims;

    // Now get colour values from the textures
    vec4 v4Col = texture(texture0_colour, textureCoordinate);
    vec3 v3ColBG = texture(texture3_background, v2BGCoords).rgb;
    vec3 v3ColReflection = texture(texture2_reflection, v2ReflectionCoords).rgb;
    vec3 v3Normal = texture(texture1_normal, textureCoordinate).rgb;

    // Convert normal from 0.0f-1.0f to -1.0f to 1.0f
    v3Normal.x *= 2.0f;
    v3Normal.x -= 1.0f;
    v3Normal.y *= 2.0f;
    v3Normal.y -= 1.0f;

    // Compute direction from mouse cursor to fragment
    vec3 vDir = vec3(v2MousePos.x, v2MousePos.y, fMouseCursorDistance) - vec3(gl_FragCoord.x, gl_FragCoord.y, 0.0);
    vDir = normalize(vDir);
    float fDot = dot(vDir, v3Normal);

    // Compute background blur
    // Compute texel size
    vec2 v2TexelSize = 1.0f / textureSize(texture3_background, 0);

    // Add everything together to produce final fragment colour
    FragColour = v4Col;                                             // Colour map
    FragColour.rgb += fReflectionAmount * v3ColReflection;          // Reflection
    FragColour.rgb += fNormalAmount * fDot;                         // Normal
    FragColour *= colour;                                           // Vertex colour
}